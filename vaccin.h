#ifndef VACCIN_H
#define VACCIN_H

#include <QString>
#include <QDate>
#include <QSqlQuery>
#include <QSqlQueryModel>
#include "QMessageBox"
#include "date.h"

class Vaccin {
private:
    int id;
    QString nom;
    int idTypeVaccin;
    QString agentCible;
    QString statutDev;
    QDate dateDev;
    QString paysOrigine;
    float tempConservation;
    int stockDisponible;
    QDate datePeremption;
    QString autorisation;

public:
    Vaccin();
    Vaccin(int id, QString nom, int idTypeVaccin, QString agentCible, QString statutDev, QDate dateDev, QString paysOrigine, float tempConservation, int stockDisponible, QDate datePeremption, QString autorisation);

    bool ajouter();
    QSqlQueryModel* afficher();
    bool supprimer(int);
    bool modifier();
    bool existe(int id);
    bool verifierSaisie(QString &messageErreur);
};

#endif
