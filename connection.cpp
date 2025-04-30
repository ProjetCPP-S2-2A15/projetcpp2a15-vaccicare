#include "connection.h"
#include <QSqlError>
#include <QDebug>
#include <QMessageBox>

Connection::Connection() {
    //zakaria
    //QSqlDatabase db = QSqlDatabase::addDatabase("QODBC");
    //db.setDatabaseName("Source_Projet2A");//inserer le nom de la source de données
    //db.setUserName("System");//inserer nom de l'utilisateur
    //db.setPassword("1234");//inserer mot de passe de cet utilisateur

//takwa
     db = QSqlDatabase::addDatabase("QODBC");
    //QSqlDatabase db = QSqlDatabase::addDatabase("QODBC");// Initialize the database connection only once
     db.setDatabaseName("VACCICARE");
     db.setUserName("SYSTEM");
     db.setPassword("1234");
     if (!db.open()) {
         //QMessageBox::critical(this, "Erreur", "Connexion échouée : " + db.lastError().text());
         return;
     }

//islem
    //QSqlDatabase db = QSqlDatabase::addDatabase("QODBC");
    //db.setDatabaseName("Source_Projet2A");//inserer le nom de la source de données
    //db.setUserName("System");//inserer nom de l'utilisateur
    //db.setPassword("1234");//inserer mot de passe de cet utilisateur

//sarah
    //QSqlDatabase db = QSqlDatabase::addDatabase("QODBC");
    //db.setDatabaseName("Source_Projet2A");//inserer le nom de la source de données
    //db.setUserName("System");//inserer nom de l'utilisateur
    //db.setPassword("123");//inserer mot de passe de cet utilisateur

//khaled
    //QSqlDatabase db = QSqlDatabase::addDatabase("QODBC");
    //db.setDatabaseName("Source_Projet2A");//inserer le nom de la source de données
    //db.setUserName("VacciDB");//inserer nom de l'utilisateur
    //db.setPassword("1234");//inserer mot de passe de cet utilisateur


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

