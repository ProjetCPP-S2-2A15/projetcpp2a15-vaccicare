#include "patient.h"
#include <QSqlQuery>
#include <QSqlError>
#include <QVariant>

// Constructeurs
Patient::Patient() {}

Patient::Patient(int id, QString nom, QString prenom, QString dateNaissance, QString sexe, QString adresse,
                 QString email, QString telephone, QString cin, QString groupeSanguin, QString statutVaccinal, QString dateVaccin)
{
    this->id = id;
    this->nom = nom;
    this->prenom = prenom;
    this->dateNaissance = dateNaissance;
    this->sexe = sexe;
    this->adresse = adresse;
    this->email = email;
    this->telephone = telephone;
    this->cin = cin;
    this->groupeSanguin = groupeSanguin;
    this->statutVaccinal = statutVaccinal;
    this->dateVaccin = dateVaccin;
}

// Getters & Setters
int Patient::getId() const { return id; }
QString Patient::getNom() const { return nom; }
QString Patient::getPrenom() const { return prenom; }
QString Patient::getDateNaissance() const { return dateNaissance; }
QString Patient::getSexe() const { return sexe; }
QString Patient::getAdresse() const { return adresse; }
QString Patient::getEmail() const { return email; }
QString Patient::getTelephone() const { return telephone; }
QString Patient::getCin() const { return cin; }
QString Patient::getGroupeSanguin() const { return groupeSanguin; }
QString Patient::getStatutVaccinal() const { return statutVaccinal; }
QString Patient::getDateVaccin() const { return dateVaccin; }

void Patient::setId(int id) { this->id = id; }
void Patient::setNom(QString nom) { this->nom = nom; }
void Patient::setPrenom(QString prenom) { this->prenom = prenom; }
void Patient::setDateNaissance(QString dateNaissance) { this->dateNaissance = dateNaissance; }
void Patient::setSexe(QString sexe) { this->sexe = sexe; }
void Patient::setAdresse(QString adresse) { this->adresse = adresse; }
void Patient::setEmail(QString email) { this->email = email; }
void Patient::setTelephone(QString telephone) { this->telephone = telephone; }
void Patient::setCin(QString cin) { this->cin = cin; }
void Patient::setGroupeSanguin(QString groupeSanguin) { this->groupeSanguin = groupeSanguin; }
void Patient::setStatutVaccinal(QString statutVaccinal) { this->statutVaccinal = statutVaccinal; }
void Patient::setDateVaccin(QString dateVaccin) { this->dateVaccin = dateVaccin; }

// CRUD
bool Patient::ajouter() {
    QSqlQuery query;
    query.prepare("INSERT INTO PATIENT (ID_PATIENT, NOM, PRENOM, DATE_NAISSANCE, SEXE, ADRESSE, EMAIL, TELEPHONE, GROUPE_SANGUIN, STATUT_VACCINAL, DATE_VACCIN) "
                  "VALUES (:id, :nom, :prenom,TO_DATE(:dateNaissance, 'YYYY-MM-DD'), :sexe, :adresse, :email, :telephone, :groupeSanguin, :statutVaccinal, :dateVaccin);");

    query.bindValue(":id", id);
    query.bindValue(":nom", nom);
    query.bindValue(":prenom", prenom);
    query.bindValue(":dateNaissance", dateNaissance);
    query.bindValue(":sexe", sexe);
    query.bindValue(":adresse", adresse);
    query.bindValue(":email", email);
    query.bindValue(":telephone", telephone);
    query.bindValue(":groupeSanguin", groupeSanguin);
    //query.bindValue(":statutVaccinal", statutVaccinal);
    query.bindValue(":statutVaccinal", 0);
    //query.bindValue(":dateVaccin", dateVaccin);
    query.bindValue(":dateVaccin", 0);
    return query.exec();
}

bool Patient::supprimer(int id) {
    QSqlQuery query;
    query.prepare("DELETE FROM PATIENT WHERE ID_Patient = :id");
    query.bindValue(":id", id);
    return query.exec();
}

bool Patient::modifier(int id) {
    QSqlQuery query;
    query.prepare("UPDATE PATIENT SET Nom=:nom, Prenom=:prenom, Date_Naissance=TO_DATE(:dateNaissance, 'YYYY-MM-DD'), Sexe=:sexe, Adresse=:adresse, "
                  "Email=:email, Telephone=:telephone, Groupe_Sanguin=:groupeSanguin, Statut_Vaccinal=:statutVaccinal, "
                  "Date_Vaccin=:dateVaccin WHERE ID_Patient=:id");
    query.bindValue(":id", id);
    query.bindValue(":nom", nom);
    query.bindValue(":prenom", prenom);
    query.bindValue(":dateNaissance", dateNaissance);
    query.bindValue(":sexe", sexe);
    query.bindValue(":adresse", adresse);
    query.bindValue(":email", email);
    query.bindValue(":telephone", telephone);
    //query.bindValue(":cin", cin);
    query.bindValue(":groupeSanguin", groupeSanguin);
    query.bindValue(":statutVaccinal", 0);
    query.bindValue(":dateVaccin", 0);

    return query.exec();
}

QSqlQueryModel* Patient::afficher() {
    QSqlQueryModel* model = new QSqlQueryModel();
    model->setQuery("SELECT * FROM PATIENT");
    return model;
}
