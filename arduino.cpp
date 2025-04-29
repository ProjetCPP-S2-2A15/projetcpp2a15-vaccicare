#include "arduino.h"
#include <QSerialPortInfo>
#include <QDebug>
#include <QSqlQuery>
#include <QSqlError>
#include <QSqlDatabase>

arduino::arduino() {
    serial = new QSerialPort();
    arduino_port_name = "";
    arduino_is_available = false;
    data = "";
}

arduino::~arduino() {
    if (serial->isOpen()) {
        serial->close();
    }
    delete serial;
}

QString arduino::getarduino_port_name() {
    return arduino_port_name;
}

QSerialPort* arduino::getserial() {
    return serial;
}

int arduino::connect_arduino() {
    qDebug() << "Analyse des ports disponibles...";
    QList<QSerialPortInfo> ports = QSerialPortInfo::availablePorts();
    if (ports.isEmpty()) {
        qDebug() << "Aucun port série détecté !";
    }
    foreach (const QSerialPortInfo &serial_port_info, ports) {
        qDebug() << "Port :" << serial_port_info.portName()
        << "VendorID :" << (serial_port_info.hasVendorIdentifier() ? QString::number(serial_port_info.vendorIdentifier(), 16) : "Aucun")
        << "ProductID :" << (serial_port_info.hasProductIdentifier() ? QString::number(serial_port_info.productIdentifier(), 16) : "Aucun");
        if (serial_port_info.hasVendorIdentifier() && serial_port_info.hasProductIdentifier()) {
            if (serial_port_info.vendorIdentifier() == arduino_uno_vendor_id &&
                serial_port_info.productIdentifier() == arduino_uno_product_id) {
                arduino_is_available = true;
                arduino_port_name = serial_port_info.portName();
                qDebug() << "Arduino trouvé sur le port :" << arduino_port_name;
            }
        }
    }
    qDebug() << "Nom du port Arduino :" << arduino_port_name;
    if (arduino_is_available) {
        serial->setPortName(arduino_port_name);
        if (serial->open(QSerialPort::ReadWrite)) {
            serial->setBaudRate(QSerialPort::Baud9600);
            serial->setDataBits(QSerialPort::Data8);
            serial->setParity(QSerialPort::NoParity);
            serial->setStopBits(QSerialPort::OneStop);
            serial->setFlowControl(QSerialPort::NoFlowControl);
            qDebug() << "Arduino connecté avec succès";
            return 0;
        }
        qDebug() << "Échec de l'ouverture du port :" << serial->errorString();
        return 1;
    }
    qDebug() << "Aucun Arduino correspondant trouvé";
    return -1;
}

int arduino::close_arduino() {
    if (serial->isOpen()) {
        serial->close();
        arduino_is_available = false;
        return 0;
    }
    return 1;
}

QByteArray arduino::read_from_arduino() {
    if (serial->isReadable()) {
        data = serial->readAll();
        if (!data.isEmpty()) {
            static QByteArray buffer;
            buffer.append(data);
            if (buffer.contains("\n")) {
                QByteArray completeTag = buffer;
                buffer.clear();
                return completeTag;
            }
        }
    }
    return QByteArray();
}

void arduino::write_to_arduino(const QString &d) {
    if (serial->isWritable()) {
        QByteArray data = d.toUtf8();
        serial->write(data);
        if (serial->waitForBytesWritten(1000)) {
            qDebug() << "Données écrites sur l'Arduino :" << data;
        } else {
            qDebug() << "Échec de l'écriture sur l'Arduino : Timeout";
        }
    } else {
        qDebug() << "Port série non accessible en écriture !";
    }
}

QString arduino::verifyPatientID(const QString &patientID, QSqlDatabase &db) {
    if (!db.isOpen()) {
        qDebug() << "Base de données non ouverte";
        return "Erreur : Base de données non connectée";
    }

    QSqlQuery query(db);
    query.prepare("SELECT NOM, PRENOM FROM PATIENT WHERE ID_PATIENT = :id");
    query.bindValue(":id", patientID.toInt());

    if (!query.exec()) {
        qDebug() << "Erreur de requête :" << query.lastError().text();
        return "Erreur : Échec de la requête";
    }

    if (query.next()) {
        QString nom = query.value(0).toString();
        QString prenom = query.value(1).toString();
        return "Bienvenue " + prenom + " " + nom + " !";
    }

    return "Erreur : ID patient invalide";
}
