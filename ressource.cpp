#include "ressource.h"
#include "qsqlerror.h"

Ressource::Ressource() {}

Ressource::Ressource(int id, QString nom, int type, int qt, int dispo, int date, int frs, int cout)
{
    this->id_ressource = id;
    this->nom = nom;
    this->id_type_ressource = type;
    this->quantite = qt;
    this->disponibilite = dispo;
    this->date_acquisition = date;
    this->id_fournisseur = frs;
    this->cout_acquisition = cout;
}

int Ressource::getId() { return id_ressource; }
QString Ressource::getNom() { return nom; }
int Ressource::getIdTypeRessource() { return id_type_ressource; }
int Ressource::getQuantite() { return quantite; }
int Ressource::getDisponibilite() { return disponibilite; }
int Ressource::getDateAcquisition() { return date_acquisition; }
int Ressource::getIdFournisseur() { return id_fournisseur; }
int Ressource::getCoutAcquisition() { return cout_acquisition; }

void Ressource::setId(int id) { id_ressource = id; }
void Ressource::setNom(QString n) { nom = n; }
void Ressource::setIdTypeRessource(int type) { id_type_ressource = type; }
void Ressource::setQuantite(int qt) { quantite = qt; }
void Ressource::setDisponibilite(int dispo) { disponibilite = dispo; }
void Ressource::setDateAcquisition(int date) { date_acquisition = date; }
void Ressource::setIdFournisseur(int frs) { id_fournisseur = frs; }
void Ressource::setCoutAcquisition(int cout) { cout_acquisition = cout; }

bool Ressource::ajouter()
{
    QSqlQuery query;
    query.prepare("INSERT INTO RESSOURCE (ID_RESSOURCE, NOM, ID_TYPE_RESSOURCE, QUANTITE, DISPONIBILITE, DATE_ACQUISITION, ID_FOURNISSEUR, COUT_ACQUISITION) "
                  "VALUES (:id, :nom, :type, :quantite, :dispo, :date, :frs, :cout)");
    query.bindValue(":id", id_ressource);
    query.bindValue(":nom", nom);
    query.bindValue(":type", id_type_ressource);
    query.bindValue(":quantite", quantite);
    query.bindValue(":dispo", disponibilite);
    query.bindValue(":date", date_acquisition);
    query.bindValue(":frs", id_fournisseur);
    query.bindValue(":cout", cout_acquisition);
    return query.exec();
}

QSqlQueryModel* Ressource::afficher()
{
    QSqlQueryModel *model = new QSqlQueryModel();
    model->setQuery("SELECT * FROM RESSOURCE");
    return model;
}

bool Ressource::supprimer(int id)
{
    QSqlQuery query;
    query.prepare("DELETE FROM RESSOURCE WHERE ID_RESSOURCE = :id");
    query.bindValue(":id", id);
    return query.exec();
}

bool Ressource::modifier(int id, QString nom, int type, int fournisseur, int quantite, int cout_acquisition, int disponibilite, int date_acquisition)
{
    QSqlQuery query;
    query.prepare("UPDATE RESSOURCE SET NOM = :nom, ID_TYPE_RESSOURCE = :type, "
                  "QUANTITE = :quantite, DISPONIBILITE = :dispo, DATE_ACQUISITION = :date, "
                  "ID_FOURNISSEUR = :frs, COUT_ACQUISITION = :cout "
                  "WHERE ID_RESSOURCE = :id");

    query.bindValue(":id", id_ressource);
    query.bindValue(":nom", nom);
    query.bindValue(":type", id_type_ressource);
    query.bindValue(":quantite", quantite);
    query.bindValue(":dispo", disponibilite);
    query.bindValue(":date", date_acquisition);
    query.bindValue(":frs", id_fournisseur);
    query.bindValue(":cout", cout_acquisition);

    return query.exec();

   if (!query.exec()) {
        qDebug() << "Erreur modification:" << query.lastError().text();
    return false;
    }

    return true;
}
QSqlQueryModel* Ressource::rechercherParNom(const QString& nom)
{
    QSqlQueryModel *model = new QSqlQueryModel();
    QSqlQuery query;

    query.prepare("SELECT * FROM RESSOURCE WHERE LOWER(NOM) LIKE LOWER(:nom)");
    query.bindValue(":nom", "%" + nom + "%");

    if (!query.exec()) {
        qDebug() << "Erreur recherche nom:" << query.lastError().text();
        return model;
    }

    model->setQuery(std::move(query));
    return model;
}
QSqlQueryModel* Ressource::trierPar(const QString& critere)
{
    QSqlQueryModel *model = new QSqlQueryModel();
    QSqlQuery query;

    QString queryString;

    if (critere == "Nom") {
        queryString = "SELECT * FROM RESSOURCE ORDER BY NOM ASC";
    } else if (critere == "ID") {
        queryString = "SELECT * FROM RESSOURCE ORDER BY ID_RESSOURCE ASC";
    } else {
        queryString = "SELECT * FROM RESSOURCE";  // Default fallback
    }

    query.prepare(queryString);

    if (!query.exec()) {
        qDebug() << "Erreur tri:" << query.lastError().text();
        return model;
    }

    model->setQuery(std::move(query));
    return model;
}

