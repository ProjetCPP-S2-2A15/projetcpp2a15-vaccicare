#ifndef VACCIN_H
#define VACCIN_H

#include <QString>
#include <QDate>
#include <QMap>
#include <QSqlQuery>
#include <QSqlQueryModel>
#include "QMessageBox"
#include "date.h"

class Vaccin {
private:


public:

    int id;
    QString nom;
    int idTypeV;
    QString agentCible;
    QString statutDev;
    QDate dateDev;
    QString paysOrigine;
    int tempConservation;
    int stockDisponible;
    QDate datePeremption;
    QString autorisation;
    Vaccin();
    Vaccin(int id, QString nom, int idTypeV, QString agentCible, QString statutDev, QDate dateDev, QString paysOrigine, float tempConservation, int stockDisponible, QDate datePeremption, QString autorisation);
    bool idExists(int id);
    bool ajouter();
    static std::vector<Vaccin> afficher();
    static std::vector<Vaccin> afficherTriParDateDev();
    static std::vector<Vaccin> afficherTriParDatePrem();
    static std::vector<Vaccin> rechercherParNom(const QString &nomRecherche);
    static std::vector<Vaccin> rechercherParPays(const QString &paysRecherche);
    static int getTypeVaccinId(const QString& typeName);
    static QString getTypeVaccinName(int idTypeV);
    static int GetLastID();

    static Vaccin GetVaccinFromDb(int Id_Vaccin);
    static bool supprimer(int id);
    bool modifier();
    bool existe(int id);
    bool verifierSaisie(QString &messageErreur);

    int getStock(int id);


};

#endif
