#ifndef PATIENT_H
#define PATIENT_H

#include <QString>
#include <QSqlQuery>
#include <QSqlQueryModel>

class Patient {
public:
    Patient();
    Patient(int id, QString nom, QString prenom, QString dateNaissance, QString sexe, QString adresse,
            QString email, QString telephone, QString cin, QString groupeSanguin, QString statutVaccinal, QString dateVaccin);

    // Getters
    int getId() const;
    QString getNom() const;
    QString getPrenom() const;
    QString getDateNaissance() const;
    QString getSexe() const;
    QString getAdresse() const;
    QString getEmail() const;
    QString getTelephone() const;
    QString getCin() const;
    QString getGroupeSanguin() const;
    QString getStatutVaccinal() const;
    QString getDateVaccin() const;

    // Setters
    void setId(int);
    void setNom(QString);
    void setPrenom(QString);
    void setDateNaissance(QString);
    void setSexe(QString);
    void setAdresse(QString);
    void setEmail(QString);
    void setTelephone(QString);
    void setCin(QString);
    void setGroupeSanguin(QString);
    void setStatutVaccinal(QString);
    void setDateVaccin(QString);

    // CRUD
    bool ajouter();
    bool supprimer(int);
    bool modifier(int);
    QSqlQueryModel* afficher();

private:
    int id;
    QString nom, prenom, dateNaissance, sexe, adresse, email, telephone, cin, groupeSanguin, statutVaccinal, dateVaccin;
};

#endif // PATIENT_H
