#include "vaccin.h"

Vaccin::Vaccin() {}

Vaccin::Vaccin(int id, QString nom, int idTypeVaccin, QString agentCible, QString statutDev, QDate dateDev, QString paysOrigine,
               float tempConservation, int stockDisponible, QDate datePeremption, QString autorisation) {
    this->id = id;
    this->nom = nom;
    this->idTypeVaccin = 0;
    this->agentCible = agentCible;
    this->statutDev = statutDev;
    this->dateDev = dateDev;
    this->paysOrigine = paysOrigine;
    this->tempConservation = tempConservation;
    this->stockDisponible = stockDisponible;
    this->datePeremption = datePeremption;
    this->autorisation = autorisation;
}
bool Vaccin::idExists(int id) {
    QSqlQuery query;
    query.prepare("SELECT COUNT(*) FROM VACCIN WHERE ID_VACCIN = :id");
    query.bindValue(":id", id);
    query.exec();

    if (query.next()) {
        return query.value(0).toInt() > 0;
    }
    return false;
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

    if (statutDev.isEmpty()) {
        messageErreur = "Veuillez sélectionner un statut de développement.";
        return false;
    }

    if (agentCible.isEmpty()) {
        messageErreur = "Veuillez sélectionner un agent pathogène ciblé.";
        return false;
    }

    if (autorisation.isEmpty()) {
        messageErreur = "Veuillez sélectionner un type d'autorisation.";
        return false;
    }


    if (dateDev.isNull()) {
        messageErreur = "La date de développement doit être valide.";
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
    if (datePeremption.isNull()) {
        messageErreur = "La date de péremption doit être valide.";
        return false;
    }

    return true;
}

bool Vaccin::ajouter() {
    QString erreur;
    if (!verifierSaisie(erreur)) {
        QMessageBox::warning(NULL, "Echoué", erreur);
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
    query.bindValue(":dateDev", Date::ConvertDateToInt(dateDev.toString("dd/MM/yyyy")));
    query.bindValue(":paysOrigine", paysOrigine);
    query.bindValue(":tempConservation", tempConservation);
    query.bindValue(":stockDisponible", stockDisponible);
    query.bindValue(":datePeremption", Date::ConvertDateToInt(datePeremption.toString("dd/MM/yyyy")));
    query.bindValue(":autorisation", autorisation);

    return query.exec();
}

bool Vaccin::modifier() {
    QString erreur;
    if (!verifierSaisie(erreur)) {
        QMessageBox::warning(NULL, "Echoué", erreur);
        return false;    }

    QSqlQuery query;
    query.prepare("UPDATE VACCIN SET NOM=:nom, ID_TYPE_VACCIN=:idTypeVaccin, AGENT_CIBLE=:agentCible, STATUT_DEVELOPPEMENT=:statutDev, DATE_DEVELOPPEMENT=:dateDev, PAYS_ORIGINE=:paysOrigine, TEMP_CONSERVATION=:tempConservation, STOCK_DISPONIBLE=:stockDisponible, DATE_PEREMPTION=:datePeremption, AUTORISATION=:autorisation WHERE ID_VACCIN=:id");
    query.bindValue(":id", id);
    query.bindValue(":nom", nom);
    query.bindValue(":idTypeVaccin", idTypeVaccin);
    query.bindValue(":agentCible", agentCible);
    query.bindValue(":statutDev", statutDev);
    query.bindValue(":dateDev", Date::ConvertDateToInt(dateDev.toString("dd/MM/yyyy")));
    query.bindValue(":paysOrigine", paysOrigine);
    query.bindValue(":tempConservation", tempConservation);
    query.bindValue(":stockDisponible", stockDisponible);
    query.bindValue(":datePeremption", Date::ConvertDateToInt(datePeremption.toString("dd/MM/yyyy")));
    query.bindValue(":autorisation", autorisation);

    return query.exec();
}

bool Vaccin::supprimer(int id) {
    if (id <= 0) {
        QMessageBox::warning(NULL, "Echoué", "Erreur : ID invalide.");
        return false;
    }

    QSqlQuery query;
    query.prepare("DELETE FROM VACCIN WHERE ID_VACCIN = :id");
    query.bindValue(":id", id);
    return query.exec();
}

std::vector<Vaccin> Vaccin::afficher() {
    QSqlQuery Querry;
    QString QuerryDetail = "SELECT * FROM VACCIN WHERE ID_VACCIN > 0";
    Querry.prepare(QuerryDetail);
    Querry.exec();

    std::vector<Vaccin> ListVaccin;

    Vaccin Temp;
    while(Querry.next()){
        Temp.id = Querry.value("ID_VACCIN").toInt();
        Temp.nom = Querry.value("NOM").toString();
        Temp.agentCible = Querry.value("AGENT_CIBLE").toString();
        Temp.statutDev = Querry.value("STATUT_DEVELOPPEMENT").toString();
        Temp.dateDev = Date::ConvertIntToDate(Querry.value("DATE_DEVELOPPEMENT").toInt());
        Temp.paysOrigine = Querry.value("PAYS_ORIGINE").toString();
        Temp.tempConservation = Querry.value("TEMP_CONSERVATION").toFloat();
        Temp.stockDisponible = Querry.value("STOCK_DISPONIBLE").toInt();
        Temp.datePeremption = Date::ConvertIntToDate(Querry.value("DATE_PEREMPTION").toInt());
        Temp.autorisation = Querry.value("AUTORISATION").toString();

        ListVaccin.push_back(Temp);
    }

    return ListVaccin;
}

std::vector<Vaccin> Vaccin::afficherTriParDateDev() {
    QSqlQuery Querry;
    QString QuerryDetail = "SELECT * FROM vaccin ORDER BY DATE_DEVELOPPEMENT ASC";
    Querry.prepare(QuerryDetail);
    Querry.exec();

    std::vector<Vaccin> ListVaccin;

    Vaccin Temp;
    while(Querry.next()){
        Temp.id = Querry.value("ID_VACCIN").toInt();
        Temp.nom = Querry.value("NOM").toString();
        Temp.agentCible = Querry.value("AGENT_CIBLE").toString();
        Temp.statutDev = Querry.value("STATUT_DEVELOPPEMENT").toString();
        Temp.dateDev = Date::ConvertIntToDate(Querry.value("DATE_DEVELOPPEMENT").toInt());
        Temp.paysOrigine = Querry.value("PAYS_ORIGINE").toString();
        Temp.tempConservation = Querry.value("TEMP_CONSERVATION").toFloat();
        Temp.stockDisponible = Querry.value("STOCK_DISPONIBLE").toInt();
        Temp.datePeremption = Date::ConvertIntToDate(Querry.value("DATE_PEREMPTION").toInt());
        Temp.autorisation = Querry.value("AUTORISATION").toString();

        ListVaccin.push_back(Temp);
    }

    return ListVaccin;
}



std::vector<Vaccin> Vaccin::afficherTriParDatePrem() {
    QSqlQuery Querry;
    QString QuerryDetail = "SELECT * FROM vaccin ORDER BY DATE_PEREMPTION ASC";
    Querry.prepare(QuerryDetail);
    Querry.exec();

    std::vector<Vaccin> ListVaccin;

    Vaccin Temp;
    while(Querry.next()){
        Temp.id = Querry.value("ID_VACCIN").toInt();
        Temp.nom = Querry.value("NOM").toString();
        Temp.agentCible = Querry.value("AGENT_CIBLE").toString();
        Temp.statutDev = Querry.value("STATUT_DEVELOPPEMENT").toString();
        Temp.dateDev = Date::ConvertIntToDate(Querry.value("DATE_DEVELOPPEMENT").toInt());
        Temp.paysOrigine = Querry.value("PAYS_ORIGINE").toString();
        Temp.tempConservation = Querry.value("TEMP_CONSERVATION").toFloat();
        Temp.stockDisponible = Querry.value("STOCK_DISPONIBLE").toInt();
        Temp.datePeremption = Date::ConvertIntToDate(Querry.value("DATE_PEREMPTION").toInt());
        Temp.autorisation = Querry.value("AUTORISATION").toString();

        ListVaccin.push_back(Temp);
    }

    return ListVaccin;
}


bool Vaccin::existe(int id) {
    QSqlQuery query;
    query.prepare("SELECT COUNT(*) FROM VACCIN WHERE ID_VACCIN = :id");
    query.bindValue(":id", id);

    if (query.exec() && query.next()) {
        return query.value(0).toInt() > 0;
    }
    return false;
}
