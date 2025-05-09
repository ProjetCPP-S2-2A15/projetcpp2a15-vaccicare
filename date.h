#ifndef DATE_H
#define DATE_H

#include <QString>
#include <QDate>

class Date
{

public:
    Date();
    static int ConvertDateToInt(QString Date);
    static int ConvertDateToInt(QDate Date);
    static QDate ConvertIntToDate(int daysSince2000);
    static int GetTodayDate();
};

#endif // DATE_H

//
