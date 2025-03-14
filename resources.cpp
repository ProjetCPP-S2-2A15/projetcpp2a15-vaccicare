#include "resources.h"
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include <QMessageBox>
#include <QDebug>

// Constructor
Resources::Resources(int id, QString n, int type, int q, int dispo, int date, int fournisseur, int cout)
    : id_ressource(id), nom(n), id_type_ressource(type), quantite(q), disponibilite(dispo),
    date_acquisition(date), id_fournisseur(fournisseur), cout_acquisition(cout) {}

// Add a resource to the database
bool Resources::ajouter() {
    // Input validation checkpoints
    if (nom.isEmpty()) { // Name can't be empty
        qDebug() << "Error: The name of the resource cannot be empty.";
        return false;
    }
    if (quantite <= 0) {    // Quantity must be positive
        qDebug() << "Error: Quantity must be a positive number.";
        return false;
    }
    if (disponibilite < 0) {    // Availability can't be negative
        qDebug() << "Error: Availability cannot be negative.";
        return false;
    }
    if (cout_acquisition <= 0) { // Cost must be positive
        qDebug() << "Error: The acquisition cost must be a positive number.";
        return false;
    }
    if (date_acquisition < 10000000) { // Date must follow the YYYYMMDD format
        qDebug() << "Error: The acquisition date must be in YYYYMMDD format.";
        return false;
    }

    QSqlQuery query; // Create the query
    query.prepare("INSERT INTO RESSOURCE (ID_RESSOURCE, NOM, ID_TYPE_RESSOURCE, QUANTITE, DISPONIBILITE, DATE_ACQUISITION, ID_FOURNISSEUR, COUT_ACQUISITION) "
                  "VALUES (:id, :nom, :id_type, :quantite, :disponibilite, :date_acquisition, :id_fournisseur, :cout)");
    query.bindValue(":id", id_ressource); // Bind the resource data
    query.bindValue(":nom", nom);
    query.bindValue(":id_type", id_type_ressource);
    query.bindValue(":quantite", quantite);
    query.bindValue(":disponibilite", disponibilite);
    query.bindValue(":date_acquisition", date_acquisition);
    query.bindValue(":id_fournisseur", id_fournisseur);
    query.bindValue(":cout", cout_acquisition);

    if (query.exec()) { // Execute the query
        qDebug() << "Resource successfully added!";
        return true;
    } else {
        qDebug() << "Error adding resource:" << query.lastError().text();
        return false;
    }
}

// Delete a resource by its ID
bool Resources::supprimer(int id) {
    if (id <= 0) {
        qDebug() << "Error: The resource ID must be a positive number.";
        return false;
    }

    QSqlQuery query;
    query.prepare("DELETE FROM RESSOURCE WHERE ID_RESSOURCE = :id");
    query.bindValue(":id", id);

    if (query.exec()) {
        qDebug() << "Resource successfully deleted!";
        return true;
    } else {
        qDebug() << "Error deleting resource:" << query.lastError().text();
        return false;
    }
}

// Modify an existing resource
bool Resources::modifier() {
    // Input validation checkpoints
    if (nom.isEmpty()) { // Name can't be empty
        qDebug() << "Error: The name of the resource cannot be empty.";
        return false;
    }
    if (quantite <= 0) {    // Quantity must be positive
        qDebug() << "Error: Quantity must be a positive number.";
        return false;
    }
    if (disponibilite < 0) {    // Availability can't be negative
        qDebug() << "Error: Availability cannot be negative.";
        return false;
    }
    if (cout_acquisition <= 0) { // Cost must be positive
        qDebug() << "Error: The acquisition cost must be a positive number.";
        return false;
    }
    if (date_acquisition < 10000000) { // Date must follow the YYYYMMDD format
        qDebug() << "Error: The acquisition date must be in YYYYMMDD format.";
        return false;
    }

    QSqlQuery query;
    query.prepare("UPDATE RESSOURCE SET NOM = :nom, ID_TYPE_RESSOURCE = :id_type, QUANTITE = :quantite, DISPONIBILITE = :disponibilite, "
                  "DATE_ACQUISITION = :date_acquisition, ID_FOURNISSEUR = :id_fournisseur, COUT_ACQUISITION = :cout "
                  "WHERE ID_RESSOURCE = :id");
    query.bindValue(":id", id_ressource);
    query.bindValue(":nom", nom);
    query.bindValue(":id_type", id_type_ressource);
    query.bindValue(":quantite", quantite);
    query.bindValue(":disponibilite", disponibilite);
    query.bindValue(":date_acquisition", date_acquisition);
    query.bindValue(":id_fournisseur", id_fournisseur);
    query.bindValue(":cout", cout_acquisition);

    if (query.exec()) {
        qDebug() << "Resource successfully modified!";
        return true;
    } else {
        qDebug() << "Error modifying resource:" << query.lastError().text();
        return false;
    }
}

// Display all resources
void Resources::afficher() {
    QSqlQuery query("SELECT * FROM RESSOURCE");
    while (query.next()) {
        qDebug() << "ID:" << query.value("ID_RESSOURCE").toInt()
        << "Name:" << query.value("NOM").toString()
        << "Type:" << query.value("ID_TYPE_RESSOURCE").toInt()
        << "Quantity:" << query.value("QUANTITE").toInt()
        << "Availability:" << query.value("DISPONIBILITE").toInt()
        << "Acquisition Date:" << query.value("DATE_ACQUISITION").toInt()
        << "Supplier:" << query.value("ID_FOURNISSEUR").toInt()
        << "Acquisition Cost:" << query.value("COUT_ACQUISITION").toInt();
    }
}
