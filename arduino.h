#ifndef ARDUINO_H
#define ARDUINO_H

#include <QSerialPort>
#include <QByteArray>

class Arduino {
public:
    Arduino();
    ~Arduino();
    int connect_arduino();
    int close_arduino();
    void write_to_arduino(const QString &data);
    QByteArray read_from_arduino();
    QString get_arduino_port_name() const;
    QSerialPort* getSerial(); // Getter for serial port

private:
    QSerialPort *serial;
    QString arduino_port_name;
    bool arduino_is_available;
    QByteArray data;

    static const quint16 arduino_uno_vendor_id = 0x2341;
    static const quint16 arduino_uno_product_id = 0x0043;
};

#endif // ARDUINO_H
