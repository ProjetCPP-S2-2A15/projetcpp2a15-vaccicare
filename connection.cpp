#include "connection.h"
#include <QSqlError>
#include <QDebug>
#include <QMessageBox>

Connection::Connection() {
    //zakaria
    //db = QSqlDatabase::addDatabase("QODBC");
    //db.setDatabaseName("Source_Projet2A");
    //db.setUserName("System");
    //db.setPassword("1234");

    //takwa
//    db = QSqlDatabase::addDatabase("QODBC");
//    db.setDatabaseName("VACCICARE");
//    db.setUserName("SYSTEM");
//    db.setPassword("1234");

    //islem
//    db = QSqlDatabase::addDatabase("QODBC");
//    db.setDatabaseName("Source_Projet2A");
//    db.setUserName("System");
//    db.setPassword("1234");

    //sarah
    //db = QSqlDatabase::addDatabase("QODBC");
    //db.setDatabaseName("Source_Projet2A");
    //db.setUserName("System");
    //db.setPassword("123");

    //khaled!
    db = QSqlDatabase::addDatabase("QODBC");
    db.setDatabaseName("Source_Projet2A");
    db.setUserName("VacciDB");
    db.setPassword("1234");


}

bool Connection::CreateConnexion() {
    bool test = false;
    if (db.open()) {
        test = true;
    } else {
        qDebug() << "Database Error: " << db.lastError().text();
    }
    return test;
}

void Connection::FermerConnexion() {
    if (db.isOpen()) {
        db.close();
    }
}

bool Connection::OuvrirConnexion() {
    return db.open();
}

