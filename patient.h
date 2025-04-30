#ifndef PATIENT_H
#define PATIENT_H

#include <QString>
#include <QSqlQuery>
#include <QSqlQueryModel>
#include <QTableView>
#include <QDate>

class Patient
{
public:
    Patient();
    Patient(int ID_PATIENT, QString NOM, QString PRENOM, QDate DATE_NAISSANCE, QString SEXE, QString ADRESSE, int TELEPHONE, QString EMAIL, QString GROUPE_SANGUIN, QString STATUT_VACCINAL, QDate DATE_VACCIN, int ID_PROJET_TESTER);

    int getID_PATIENT() const { return ID_PATIENT; }
        QString getNOM() const { return NOM; }
        QString getPRENOM() const { return PRENOM; }
        QDate getDATE_NAISSANCE() const { return DATE_NAISSANCE; }
        QString getSEXE() const { return SEXE; }
        QString getADRESSE() const { return ADRESSE; }
        int getTELEPHONE() const { return TELEPHONE; }
        QString getEMAIL() const { return EMAIL; }
        QString getGROUPE_SANGUIN() const { return GROUPE_SANGUIN; }
        QString getSTATUT_VACCINAL() const { return STATUT_VACCINAL; }
        QDate getDATE_VACCIN() const { return DATE_VACCIN; }
        int getID_PROJET_TESTER() const { return ID_PROJET_TESTER; }

        // Setters
        void setID_PATIENT(int id) { ID_PATIENT = id; }
        void setNOM(const QString &nom) { NOM = nom; }
        void setPRENOM(const QString &prenom) { PRENOM = prenom; }
        void setDATE_NAISSANCE(const QDate &dateNaissance) { DATE_NAISSANCE = dateNaissance; }
        void setSEXE(const QString &sexe) { SEXE = sexe; }
        void setADRESSE(const QString &adresse) { ADRESSE = adresse; }
        void setTELEPHONE(int telephone) { TELEPHONE = telephone; }
        void setEMAIL(const QString &email) { EMAIL = email; }
        void setGROUPE_SANGUIN(const QString &groupeSanguin) { GROUPE_SANGUIN = groupeSanguin; }
        void setSTATUT_VACCINAL(const QString &statutVaccinal) { STATUT_VACCINAL = statutVaccinal; }
        void setDATE_VACCIN(const QDate &dateVaccin) { DATE_VACCIN = dateVaccin; }
        void setID_PROJET_TESTER(int idProjetTester) { ID_PROJET_TESTER = idProjetTester; }

    bool Ajouter();
    bool Modifier();
    bool Supprimer(int ID_PATIENT);
    QSqlQueryModel* Afficher();
    QSqlQueryModel* Afficher_ID();
    QSqlQueryModel* Afficher_Tri_NOM();
    QSqlQueryModel* Afficher_Tri_STATUT_VACCINAL();
    QSqlQueryModel* Afficher_Tri_GROUPE_SANGUIN();
    void clearTable(QTableView *table);
    void Recherche(QTableView *table, QString x);

private:
    int ID_PATIENT;
    QString NOM;
    QString PRENOM;
    QDate DATE_NAISSANCE;
    QString SEXE;
    QString ADRESSE;
    int TELEPHONE;
    QString EMAIL;
    QString GROUPE_SANGUIN;
    QString STATUT_VACCINAL;
    QDate DATE_VACCIN;
    int ID_PROJET_TESTER;
};

#endif // PATIENT_H
