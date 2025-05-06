#ifndef MEDECIN_H
#define MEDECIN_H

#include <QString>
#include <QSqlQueryModel>

class Medecin
{
private:


public:
    Medecin();
    Medecin(int id, QString n, QString s, QString g, QString e, QString t, int exp,
            QString stat, QString disp, QString log, QString mdp);

    bool ajouter();
    bool modifier(int id);
    static bool supprimer(int id);
    QSqlQueryModel* afficher(const QString &searchText, const QString &criterion, const QString &sortCriterion = "Id");
    bool fetchMedecinById(int id, QString &nom, QString &specialite, QString &grade, QString &email,
                          QString &telephone, QString &disponibilite, int &experience,
                          QString &statut, QString &login, QString &mot_de_passe);
    bool verifierLogin(const QString &login, const QString &mot_de_passe);

    int id_medecin;
    QString nom;
    QString specialite;
    QString grade;
    QString email;
    QString telephone;
    QString disponibilite;
    int experience;
    QString statut;
    QString login;
    QString mot_de_passe;
};

#endif // MEDECIN_H
