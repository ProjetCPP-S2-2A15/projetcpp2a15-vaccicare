#ifndef VACCIN_H
#define VACCIN_H

#include <QString>
#include <QDate>
#include <QSqlQuery>
#include <QSqlQueryModel>
#include "QMessageBox"

class Vaccin {
private:
    int id;
    QString nom;
    int idTypeVaccin;
    QString agentCible;
    QString statutDev;
    int dateDev;
    QString paysOrigine;
    float tempConservation;
    int stockDisponible;
    int datePeremption;
    QString autorisation;

public:
    Vaccin();
    Vaccin(int id, QString nom, int idTypeVaccin, QString agentCible, QString statutDev, int deteDev, QString paysOrigine, float tempConservation, int stockDisponible, int datePeremption, QString autorisation);

    bool ajouter();
    QSqlQueryModel* afficher();
    bool supprimer(int);
    bool modifier();
    bool existe(int id);
    // Méthode de validation
    bool verifierSaisie(QString &messageErreur);
};

#endif
