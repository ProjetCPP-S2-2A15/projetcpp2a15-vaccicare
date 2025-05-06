#include "vaccin.h"
#include <QSqlError>
#include <QSqlQuery>
#include <QDebug>

Vaccin::Vaccin() {}

Vaccin::Vaccin(int id, QString nom, int idTypeV, QString agentCible, QString statutDev, QDate dateDev, QString paysOrigine,
               float tempConservation, int stockDisponible, QDate datePeremption, QString autorisation) {
    this->id = id;
    this->nom = nom;
    this->idTypeV = idTypeV;
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
                  "VALUES (:id, :nom, :idTypeV, :agentCible, :statutDev, :dateDev, :paysOrigine, :tempConservation, :stockDisponible, :datePeremption, :autorisation)");

    query.bindValue(":id", id);
    query.bindValue(":nom", nom);
    query.bindValue(":idTypeV", idTypeV);
    query.bindValue(":agentCible", agentCible);
    query.bindValue(":statutDev", statutDev);
    query.bindValue(":dateDev", Date::ConvertDateToInt(dateDev));
    query.bindValue(":paysOrigine", paysOrigine);
    query.bindValue(":tempConservation", tempConservation);
    query.bindValue(":stockDisponible", stockDisponible);
    query.bindValue(":datePeremption", Date::ConvertDateToInt(datePeremption));
    query.bindValue(":autorisation", autorisation);

    return query.exec();
}

bool Vaccin::modifier() {
    QString erreur;
    if (!verifierSaisie(erreur)) {
        QMessageBox::warning(NULL, "Echoué", erreur);
        return false;    }

    QSqlQuery query;
    query.prepare("UPDATE VACCIN SET NOM=:nom, ID_TYPE_VACCIN=:idTypeV, AGENT_CIBLE=:agentCible, STATUT_DEVELOPPEMENT=:statutDev, DATE_DEVELOPPEMENT=:dateDev, PAYS_ORIGINE=:paysOrigine, TEMP_CONSERVATION=:tempConservation, STOCK_DISPONIBLE=:stockDisponible, DATE_PEREMPTION=:datePeremption, AUTORISATION=:autorisation WHERE ID_VACCIN=:id");
    query.bindValue(":id", id);
    query.bindValue(":nom", nom);
    query.bindValue(":idTypeV", idTypeV);
    query.bindValue(":agentCible", agentCible);
    query.bindValue(":statutDev", statutDev);
    query.bindValue(":dateDev", Date::ConvertDateToInt(dateDev));
    query.bindValue(":paysOrigine", paysOrigine);
    query.bindValue(":tempConservation", tempConservation);
    query.bindValue(":stockDisponible", stockDisponible);
    query.bindValue(":datePeremption", Date::ConvertDateToInt(datePeremption));
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
        Temp.idTypeV = Querry.value("ID_TYPE_VACCIN").toInt();
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
        Temp.idTypeV = Querry.value("ID_TYPE_VACCIN").toInt();
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
        Temp.idTypeV = Querry.value("ID_TYPE_VACCIN").toInt();
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

std::vector<Vaccin> Vaccin::rechercherParNom(const QString &nomRecherche) {
    std::vector<Vaccin> resultatsn;

    QSqlQuery query;
    QString requete = "SELECT * FROM VACCIN WHERE LOWER(NOM) LIKE '%" +nomRecherche.toLower() +"%'";
    query.exec(requete);

    while (query.next()) {
        Vaccin v;
        v.id = query.value("ID_VACCIN").toInt();
        v.nom = query.value("NOM").toString();
        v.idTypeV = query.value("ID_TYPE_VACCIN").toInt();
        v.agentCible = query.value("AGENT_CIBLE").toString();
        v.statutDev = query.value("STATUT_DEVELOPPEMENT").toString();
        v.dateDev = Date::ConvertIntToDate(query.value("DATE_DEVELOPPEMENT").toInt());
        v.paysOrigine = query.value("PAYS_ORIGINE").toString();
        v.tempConservation = query.value("TEMP_CONSERVATION").toFloat();
        v.stockDisponible = query.value("STOCK_DISPONIBLE").toInt();
        v.datePeremption = Date::ConvertIntToDate(query.value("DATE_PEREMPTION").toInt());
        v.autorisation = query.value("AUTORISATION").toString();

        resultatsn.push_back(v);
    }

    return resultatsn;
}

std::vector<Vaccin> Vaccin::rechercherParPays(const QString &paysRecherche) {
    std::vector<Vaccin> resultats;

    QSqlQuery query;

    query.prepare("SELECT * FROM VACCIN WHERE LOWER(PAYS_ORIGINE) LIKE '%" +paysRecherche.toLower() +"%'");
    query.exec();

    while (query.next()) {
        Vaccin v;
        v.id = query.value("ID_VACCIN").toInt();
        v.nom = query.value("NOM").toString();
        v.idTypeV = query.value("ID_TYPE_VACCIN").toInt();
        v.agentCible = query.value("AGENT_CIBLE").toString();
        v.statutDev = query.value("STATUT_DEVELOPPEMENT").toString();
        v.dateDev = Date::ConvertIntToDate(query.value("DATE_DEVELOPPEMENT").toInt());
        v.paysOrigine = query.value("PAYS_ORIGINE").toString();
        v.tempConservation = query.value("TEMP_CONSERVATION").toFloat();
        v.stockDisponible = query.value("STOCK_DISPONIBLE").toInt();
        v.datePeremption = Date::ConvertIntToDate(query.value("DATE_PEREMPTION").toInt());
        v.autorisation = query.value("AUTORISATION").toString();

        resultats.push_back(v);
    }

    return resultats;
}

int Vaccin::getStock(int id) {
    QSqlQuery query;
    query.prepare("SELECT STOCK_DISPONIBLE FROM VACCIN WHERE ID_VACCIN = :id");
    query.bindValue(":id", id);
    if (query.exec() && query.next()) {
        return query.value("STOCK_DISPONIBLE").toInt();
    }
    return 0;  // Si la récupération échoue
}

QString Vaccin::getTypeVaccinName(int idTypeV){
    QSqlQuery query;
    query.prepare("SELECT TYPE_VACCIN FROM TYPE_VACCIN WHERE ID_TYPE_VACCIN = :id");
    query.bindValue(":id", idTypeV);

    if (query.exec() && query.next()) {
        return query.value(0).toString();
    }

    return "Inconnu";
}

int Vaccin::getTypeVaccinId(const QString& typeName) {
    QSqlQuery query;
    query.prepare("SELECT ID_TYPE_VACCIN FROM TYPE_VACCIN WHERE TYPE_VACCIN = :typeName");
    query.bindValue(":typeName", typeName);

    if (!query.exec()) {
        //qDebug() << "Query failed: " << query.lastError().text();
        return 5;
    }

    if (query.next()) {
        int ID = query.value(0).toInt();
        return ID;
    }

    return -1;  // Si aucun résultat n'est trouvé
}

Vaccin Vaccin::GetVaccinFromDb(int Id_Vaccin){

    QSqlQuery query;
    QString requete = "SELECT * FROM VACCIN WHERE ID_VACCIN =:id ";
    query.prepare(requete);
    query.bindValue(":id",Id_Vaccin);
    query.exec();

    query.next();
    Vaccin v;
    v.id = query.value("ID_VACCIN").toInt();
    v.nom = query.value("NOM").toString();
    v.idTypeV = query.value("ID_TYPE_VACCIN").toInt();
    v.agentCible = query.value("AGENT_CIBLE").toString();
    v.statutDev = query.value("STATUT_DEVELOPPEMENT").toString();
    v.dateDev = Date::ConvertIntToDate(query.value("DATE_DEVELOPPEMENT").toInt());
    v.paysOrigine = query.value("PAYS_ORIGINE").toString();
    v.tempConservation = query.value("TEMP_CONSERVATION").toFloat();
    v.stockDisponible = query.value("STOCK_DISPONIBLE").toInt();
    v.datePeremption = Date::ConvertIntToDate(query.value("DATE_PEREMPTION").toInt());
    v.autorisation = query.value("AUTORISATION").toString();



    return v;
}

int Vaccin::GetLastID(){
    QSqlQuery Querry;
    Querry.prepare("Select MAX(ID_VACCIN) FROM VACCIN;");
    Querry.exec();

    if (Querry.next()) {  // Move to the first row
        QVariant result = Querry.value(0);
        if (result.isNull()) {
            return 0;  // Return 0 if the table is empty
        }
        return result.toInt();
    }

    return 0;  // If no rows exist, return 0
}
