
#ifndef ARDUINO_H
#define ARDUINO_H

#include <QSerialPort>
#include <QString>
#include <QByteArray>

    class QSqlDatabase;

class arduino {
public:
    arduino();
    ~arduino();
    int connect_arduino();
    int close_arduino();
    QString getarduino_port_name();
    QSerialPort* getserial();
    QByteArray read_from_arduino();
    void write_to_arduino(const QString &data);
    QString verifyPatientID(const QString &patientID, QSqlDatabase &db);

private:
    QSerialPort* serial;
    QString arduino_port_name;
    bool arduino_is_available;
    QByteArray data;
    static const quint16 arduino_uno_vendor_id = 0x2341;  // Mettre à jour si nécessaire
    static const quint16 arduino_uno_product_id = 0x0043; // Mettre à jour si nécessaire
};

#endif // ARDUINO_H
