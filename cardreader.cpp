#include "cardreader.h"
#include <QDebug>

CardReader::CardReader()
{
    serial = new QSerialPort();        // For Arduino
    serialReader = new QSerialPort();  // For Card Reader
}

CardReader::~CardReader()
{
    if (serial && serial->isOpen()) serial->close();
        if (serialReader && serialReader->isOpen()) serialReader->close();
        delete serial;
        delete serialReader;
}

bool CardReader::InitArduino()
{
    // Select your COM port here (adapt to your machine)
    serial->setPortName("COM7"); // Replace "COM3" by the right one
    serial->setBaudRate(QSerialPort::Baud9600);
    serial->setDataBits(QSerialPort::Data8);
    serial->setParity(QSerialPort::NoParity);
    serial->setStopBits(QSerialPort::OneStop);
    serial->setFlowControl(QSerialPort::NoFlowControl);

    if (!serial->open(QIODevice::ReadWrite)) {
        qDebug() << "Failed to open serial port:" << serial->errorString();
        return false;
    }

    qDebug() << "Connected to Arduino successfully.";
    return true;
}

bool CardReader::InitReader()
{
    // Select your COM port here (adapt to your machine)
    serialReader->setPortName("COM6"); // Replace "COM3" by the right one
    serialReader->setBaudRate(QSerialPort::Baud9600);
    serialReader->setDataBits(QSerialPort::Data8);
    serialReader->setParity(QSerialPort::NoParity);
    serialReader->setStopBits(QSerialPort::OneStop);
    serialReader->setFlowControl(QSerialPort::NoFlowControl);

    if (!serialReader->open(QIODevice::ReadWrite)) {
        qDebug() << "Failed to open serial port:" << serial->errorString();
        return false;
    }

    qDebug() << "Connected to Reader successfully.";
    return true;
}


QString CardReader::ReadCard()
{
    if (!serial || !serial->isOpen()) {
        qDebug() << "Card reader serial port is not open.";
        return "";
    }

    // Wait for card reader to send data
    if (!serial->waitForReadyRead(3000)) { // Wait max 3 seconds
        qDebug() << "No card data received.";
        return "";
    }

    QByteArray data = serial->readAll();
    while (serial->waitForReadyRead(100)) {
        data += serial->readAll();
    }

    QString cardData = QString::fromUtf8(data).trimmed();
    qDebug() << "Received Card Data:" << cardData;
    return cardData;
}


void CardReader::ReturnResultToArduino(QString Command)
{
    if (!serial->isOpen()) {
        qDebug() << "Serial port is not open.";
        return;
    }

    if (Command == "GREEN" || Command == "RED" || Command == "YELLOW" || Command == "DISABLE") {
        serial->write((Command + "\n").toUtf8());
        qDebug() << "Sent command to Arduino:" << Command;
    } else {
        qDebug() << "Invalid command to Arduino:" << Command;
    }
}

bool CardReader::CompareCardDataInDB(QString CardData)
{

}
