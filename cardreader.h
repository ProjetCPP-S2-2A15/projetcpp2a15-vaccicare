#ifndef CARDREADER_H
#define CARDREADER_H

#include <QString>
#include "connection.h"
#include <QSerialPort>
#include <QSerialPortInfo>
#include <QDebug>

class CardReader
{
public:
    CardReader();
     ~CardReader();
    bool InitArduino();
    bool InitReader();
    QString ReadCard();
    void ReturnResultToArduino(QString Command);
    bool CompareCardDataInDB(QString CardData);
private:
    QSerialPort *serial;
    QSerialPort *serialReader;

};

#endif // CARDREADER_H
