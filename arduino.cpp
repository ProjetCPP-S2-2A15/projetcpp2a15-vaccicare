#include "arduino.h"
#include <QSerialPortInfo>
#include <QDebug>

Arduino::Arduino() {
    arduino_port_name = "";
    arduino_is_available = false;
    serial = new QSerialPort;
}

Arduino::~Arduino() {
    if (serial->isOpen()) {
        serial->close();
    }
    delete serial;
}

QString Arduino::get_arduino_port_name() const {
    return arduino_port_name;
}

QSerialPort* Arduino::getSerial() {
    return serial;
}

int Arduino::connect_arduino() {
    foreach (const QSerialPortInfo &serial_port_info, QSerialPortInfo::availablePorts()) {
        if (serial_port_info.hasVendorIdentifier() && serial_port_info.hasProductIdentifier()) {
            if (serial_port_info.vendorIdentifier() == arduino_uno_vendor_id &&
                serial_port_info.productIdentifier() == arduino_uno_product_id) {
                arduino_is_available = true;
                arduino_port_name = serial_port_info.portName();
            }
        }
    }
    qDebug() << "Arduino port name is:" << arduino_port_name;
    if (arduino_is_available) {
        serial->setPortName(arduino_port_name);
        if (serial->open(QSerialPort::ReadWrite)) {
            serial->setBaudRate(QSerialPort::Baud9600);
            serial->setDataBits(QSerialPort::Data8);
            serial->setParity(QSerialPort::NoParity);
            serial->setStopBits(QSerialPort::OneStop);
            serial->setFlowControl(QSerialPort::NoFlowControl);
            return 0;
        }
        return 1;
    }
    return -1;
}

int Arduino::close_arduino() {
    if (serial->isOpen()) {
        serial->close();
        return 0;
    }
    return 1;
}

void Arduino::write_to_arduino(const QString &data) {
    if (serial->isWritable()) {
        QByteArray byteData = data.toUtf8();
        serial->write(byteData);
        serial->flush();
    } else {
        qDebug() << "Couldn't write to serial!";
    }
}

QByteArray Arduino::read_from_arduino() {
    if (serial->isReadable()) {
        data = serial->readAll();
        return data;
    }
    return QByteArray();
}
