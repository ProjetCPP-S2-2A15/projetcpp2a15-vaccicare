#include "date.h"

Date::Date()
{

}

int Date::ConvertDateToInt(QString Date){
    // Define the reference date (1st January 2000)
    QDate baseDate(2000, 1, 1);

    // Convert the input date string to QDate (assuming format "yyyy-MM-dd")
    QDate givenDate = QDate::fromString(Date, "dd/MM/yyyy");

    // Validate the date conversion
    if (!givenDate.isValid()) {
        return -1;  // Error case (invalid date)
    }

    // Calculate the difference in days
    return baseDate.daysTo(givenDate);
}

int Date::ConvertDateToInt(QDate Date){
    // Define the reference date (1st January 2000)
    QDate baseDate(2000, 1, 1);

    // Validate the date conversion
    if (!Date.isValid()) {
        return -1;  // Error case (invalid date)
    }

    // Calculate the difference in days
    return baseDate.daysTo(Date);
}

QDate Date::ConvertIntToDate(int daysSince2000) {
    // Define the reference date (1st January 2000)
    QDate baseDate(2000, 1, 1);

    // Add the given number of days to the base date
    QDate resultDate = baseDate.addDays(daysSince2000);

    // Convert the QDate to a string in SQL format ("yyyy-MM-dd")
    return resultDate;
}

int Date::GetTodayDate(){
    return Date::ConvertDateToInt(QDate::currentDate().toString("dd/MM/yyyy"));
}
//
