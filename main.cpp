#include <QApplication>
#include <QMessageBox>
#include <QDebug>
#include <QSqlQuery>
#include <QSqlError>
#include "connection.h"
#include "mainwindow.h"
#include "arduino.h"
#include<Qbytearray>
#include <Qstring>


int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    // Initialize database connection
    Connection c;
    bool test = c.CreateConnexion();

    if (test) {
        QMessageBox::information(nullptr, QObject::tr("Database Connection"),
                                 QObject::tr("Connection successful.\nClick Cancel to exit."),
                                 QMessageBox::Cancel);
    } else {
        QMessageBox::critical(nullptr, QObject::tr("Database Connection"),
                              QObject::tr("Connection failed.\nClick Cancel to exit."),
                              QMessageBox::Cancel);
    }

    // Initialize Arduino
    Arduino arduino;
    if (arduino.connect_arduino() != 0) {
        qDebug() << "Failed to connect to Arduino";
        QMessageBox::warning(nullptr, QObject::tr("Arduino Connection"),
                             QObject::tr("Failed to connect to Arduino.\nCheck connection and try again."),
                             QMessageBox::Ok);
    } else {
        qDebug() << "Arduino connected successfully on port:" << arduino.get_arduino_port_name();
    }

    // Buffer for serial data
    static QString serialBuffer = "";

    // Set up Arduino serial communication
    QObject::connect(arduino.getSerial(), &QSerialPort::readyRead, [=, &arduino, &c]() {
        QByteArray data = arduino.read_from_arduino();
        serialBuffer += QString::fromUtf8(data);

        // Process complete messages (ending with \n)
        int newlineIndex;
        while ((newlineIndex = serialBuffer.indexOf('\n')) != -1) {
            QString message = serialBuffer.left(newlineIndex).trimmed();
            serialBuffer.remove(0, newlineIndex + 1); // Remove processed message

            qDebug() << "Received complete message from Arduino:" << message;

            if (message.startsWith("ID_PATIENT:")) {
                QString id_patient = message.mid(11).trimmed(); // Extract and trim ID
                qDebug() << "Extracted ID_PATIENT:" << id_patient;

                // Query the database
                QSqlQuery query;
                // Use PATIENT table; adjust schema if needed (e.g., HR.PATIENT)
                query.prepare("SELECT NOM, PRENOM FROM PATIENT WHERE UPPER(ID_PATIENT) = UPPER(:id)");
                query.bindValue(":id", id_patient);
                qDebug() << "Executing query for ID:" << id_patient;
                if (query.exec()) {
                    if (query.next()) {
                        QString nom = query.value("NOM").toString();
                        QString prenom = query.value("PRENOM").toString();
                        qDebug() << "Found patient - NOM:" << nom << ", PRENOM:" << prenom;
                        QString response = QString("FOUND:%1,%2\n").arg(nom, prenom);
                        qDebug() << "Sending to Arduino:" << response;
                        arduino.write_to_arduino(response);
                    } else {
                        qDebug() << "No patient found for ID:" << id_patient;
                        QString response = "NOT_FOUND\n";
                        qDebug() << "Sending to Arduino:" << response;
                        arduino.write_to_arduino(response);
                    }
                } else {
                    qDebug() << "Query failed:" << query.lastError().text();
                    QString response = "NOT_FOUND\n";
                    qDebug() << "Sending to Arduino:" << response;
                    arduino.write_to_arduino(response);
                }
            } else {
                qDebug() << "Ignoring invalid message:" << message;
            }
        }
    });

    // Show main window
    MainWindow w;
    w.show();

    // Execute application
    int result = a.exec();

    // Clean up
    c.FermerConnexion(); // Close database connection
    arduino.close_arduino(); // Close Arduino serial connection

    return result;
}

