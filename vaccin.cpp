#include "vaccin.h"

Vaccin::Vaccin(){

}

Vaccin::Vaccin(int id, QString nom, int idTypeVaccin, QString agentCible, QString statutDev, int deteDev, QString paysOrigine,
               float tempConservation,int stockDisponible, int datePeremption, QString autorisation){
    this->id = id;
    this->nom = nom;
    this->idTypeVaccin = idTypeVaccin;
    this->agentCible = agentCible;
    this->statutDev = statutDev;
    this->dateDev = deteDev;
    this->paysOrigine = paysOrigine;
    this->tempConservation = tempConservation;
    this->stockDisponible = stockDisponible;
    this->datePeremption = datePeremption;
    this->autorisation = autorisation;
}

bool Vaccin::verifierSaisie(QString &messageErreur) {
    if (id <= 0) {
        messageErreur = "L'ID du vaccin doit être un nombre positif.";
        return false;
    }
    if (nom.isEmpty() || nom.length() > 30) {
        messageErreur = "Le nom du vaccin doit contenir entre 1 et 30 caractères.";
        return false;
    }
    if (idTypeVaccin < 0) {
        messageErreur = "L'ID du type de vaccin doit être positif.";
        return false;
    }
    if (agentCible.isEmpty() || agentCible.length() > 30) {
        messageErreur = "L'agent pathogène ciblé doit contenir entre 1 et 30 caractères.";
        return false;
    }
    if (statutDev.isEmpty() || statutDev.length() > 20) {
        messageErreur = "Le statut de développement doit contenir entre 1 et 20 caractères.";
        return false;
    }
    if (dateDev <= 0) {
        messageErreur = "La date de développement doit être un nombre positif.";
        return false;
    }
    if (paysOrigine.isEmpty() || paysOrigine.length() > 20) {
        messageErreur = "Le pays d'origine doit contenir entre 1 et 20 caractères.";
        return false;
    }
    if (tempConservation < -100 || tempConservation > 100) {
        messageErreur = "La température de conservation doit être comprise entre -100°C et 100°C.";
        return false;
    }
    if (stockDisponible < 0) {
        messageErreur = "Le stock disponible ne peut pas être négatif.";
        return false;
    }
    if (datePeremption <= 0) {
        messageErreur = "La date de péremption doit être un nombre positif.";
        return false;
    }
    if (autorisation.length() > 30) {
        messageErreur = "L'autorisation réglementaire doit contenir au maximum 30 caractères.";
        return false;
    }
    return true;
}

bool Vaccin::ajouter() {
    QString erreur;
    if (!verifierSaisie(erreur)) {
        QMessageBox::warning(NULL, "Echoué",erreur);
        return false;
    }

    QSqlQuery query;
    query.prepare("INSERT INTO VACCIN (ID_VACCIN, NOM, ID_TYPE_VACCIN, AGENT_CIBLE, STATUT_DEVELOPPEMENT, DATE_DEVELOPPEMENT, PAYS_ORIGINE, TEMP_CONSERVATION, STOCK_DISPONIBLE, DATE_PEREMPTION, AUTORISATION) "
                  "VALUES (:id, :nom, :idTypeVaccin, :agentCible, :statutDev, :dateDev, :paysOrigine, :tempConservation, :stockDisponible, :datePeremption, :autorisation)");
    query.bindValue(":id", id);
    query.bindValue(":nom", nom);
    query.bindValue(":idTypeVaccin", idTypeVaccin);
    query.bindValue(":agentCible", agentCible);
    query.bindValue(":statutDev", statutDev);
    query.bindValue(":dateDev", dateDev);
    query.bindValue(":paysOrigine", paysOrigine);
    query.bindValue(":tempConservation", tempConservation);
    query.bindValue(":stockDisponible", stockDisponible);
    query.bindValue(":datePeremption", datePeremption);
    query.bindValue(":autorisation", autorisation);

    return query.exec();
}

bool Vaccin::modifier() {
    QString erreur;
    if (!verifierSaisie(erreur)) {
        QMessageBox::warning(NULL, "Echoué",erreur);
        return false;
    }

    QSqlQuery query;
    query.prepare("UPDATE VACCIN SET NOM=:nom, ID_TYPE_VACCIN=:idTypeVaccin, AGENT_CIBLE=:agentCible, STATUT_DEVELOPPEMENT=:statutDev, DATE_DEVELOPPEMENT=:dateDev, PAYS_ORIGINE=:paysOrigine, TEMP_CONSERVATION=:tempConservation, STOCK_DISPONIBLE=:stockDisponible, DATE_PEREMPTION=:datePeremption, AUTORISATION=:autorisation WHERE ID_VACCIN=:id");
    query.bindValue(":id", id);
    query.bindValue(":nom", nom);
    query.bindValue(":idTypeVaccin", idTypeVaccin);
    query.bindValue(":agentCible", agentCible);
    query.bindValue(":statutDev", statutDev);
    query.bindValue(":dateDev", dateDev);
    query.bindValue(":paysOrigine", paysOrigine);
    query.bindValue(":tempConservation", tempConservation);
    query.bindValue(":stockDisponible", stockDisponible);
    query.bindValue(":datePeremption", datePeremption);
    query.bindValue(":autorisation", autorisation);

    return query.exec();
}

bool Vaccin::supprimer(int id) {
    if (id <= 0) {
        QMessageBox::warning(NULL, "Echoué","Erreur : ID invalide.");
        return false;
    }

    QSqlQuery query;
    query.prepare("DELETE FROM VACCIN WHERE ID_VACCIN = :id");
    query.bindValue(":id", id);
    return query.exec();
}

QSqlQueryModel* Vaccin::afficher(){
    QSqlQueryModel* model = new QSqlQueryModel();
    model->setQuery("SELECT * FROM VACCIN");
    return model;
}

bool Vaccin::existe(int id) {
    QSqlQuery query;
    query.prepare("SELECT COUNT(*) FROM VACCIN WHERE ID_VACCIN = :id");
    query.bindValue(":id", id);

    if (query.exec() && query.next()) {
        return query.value(0).toInt() > 0;  // Retourne true si l'ID existe
    }
    return false;
}
