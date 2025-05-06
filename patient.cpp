#include "patient.h"
#include <QSqlQuery>
#include <QSqlError>
#include <QVariant>

// Constructeurs
Patient::Patient()
{
    ID_PATIENT = 0;
    NOM = "";
    PRENOM = "";
    DATE_NAISSANCE = QDate();
    SEXE = "";
    ADRESSE = "";
    TELEPHONE = 0;
    EMAIL = "";
    GROUPE_SANGUIN = "";
    STATUT_VACCINAL = "";
    DATE_VACCIN = QDate();
    ID_PROJET_TESTER = 0;
}

Patient::Patient(int ID_PATIENT, QString NOM, QString PRENOM, QDate DATE_NAISSANCE, QString SEXE, QString ADRESSE, int TELEPHONE, QString EMAIL, QString GROUPE_SANGUIN, QString STATUT_VACCINAL, QDate DATE_VACCIN, int ID_PROJET_TESTER)
{
    this->ID_PATIENT = ID_PATIENT;
    this->NOM = NOM;
    this->PRENOM = PRENOM;
    this->DATE_NAISSANCE = DATE_NAISSANCE;
    this->SEXE = SEXE;
    this->ADRESSE = ADRESSE;
    this->TELEPHONE = TELEPHONE;
    this->EMAIL = EMAIL;
    this->GROUPE_SANGUIN = GROUPE_SANGUIN;
    this->STATUT_VACCINAL = STATUT_VACCINAL;
    this->DATE_VACCIN = DATE_VACCIN;
    this->ID_PROJET_TESTER = ID_PROJET_TESTER;
}

bool Patient::Ajouter()
{
    QSqlQuery query;
    query.prepare("INSERT INTO PATIENT (ID_PATIENT, NOM, PRENOM, DATE_NAISSANCE, SEXE, ADRESSE, TELEPHONE, EMAIL, GROUPE_SANGUIN, STATUT_VACCINAL, DATE_VACCIN, ID_PROJET_TESTER) "
                  "VALUES (:ID_PATIENT, :NOM, :PRENOM, :DATE_NAISSANCE, :SEXE, :ADRESSE, :TELEPHONE, :EMAIL, :GROUPE_SANGUIN, :STATUT_VACCINAL, :DATE_VACCIN, :ID_PROJET_TESTER)");
    query.bindValue(":ID_PATIENT", ID_PATIENT);
    query.bindValue(":NOM", NOM);
    query.bindValue(":PRENOM", PRENOM);
    query.bindValue(":DATE_NAISSANCE", DATE_NAISSANCE);
    query.bindValue(":SEXE", SEXE);
    query.bindValue(":ADRESSE", ADRESSE);
    query.bindValue(":TELEPHONE", TELEPHONE);
    query.bindValue(":EMAIL", EMAIL);
    query.bindValue(":GROUPE_SANGUIN", GROUPE_SANGUIN);
    query.bindValue(":STATUT_VACCINAL", STATUT_VACCINAL);
    query.bindValue(":DATE_VACCIN", DATE_VACCIN);
    query.bindValue(":ID_PROJET_TESTER", ID_PROJET_TESTER);
    return query.exec();
}

bool Patient::Modifier()
{
    QSqlQuery query;
    query.prepare("UPDATE PATIENT SET NOM=:NOM, PRENOM=:PRENOM, DATE_NAISSANCE=:DATE_NAISSANCE, SEXE=:SEXE, ADRESSE=:ADRESSE, TELEPHONE=:TELEPHONE, EMAIL=:EMAIL, GROUPE_SANGUIN=:GROUPE_SANGUIN, STATUT_VACCINAL=:STATUT_VACCINAL, DATE_VACCIN=:DATE_VACCIN, ID_PROJET_TESTER=:ID_PROJET_TESTER WHERE ID_PATIENT=:ID_PATIENT");
    query.bindValue(":ID_PATIENT", ID_PATIENT);
    query.bindValue(":NOM", NOM);
    query.bindValue(":PRENOM", PRENOM);
    query.bindValue(":DATE_NAISSANCE", DATE_NAISSANCE);
    query.bindValue(":SEXE", SEXE);
    query.bindValue(":ADRESSE", ADRESSE);
    query.bindValue(":TELEPHONE", TELEPHONE);
    query.bindValue(":EMAIL", EMAIL);
    query.bindValue(":GROUPE_SANGUIN", GROUPE_SANGUIN);
    query.bindValue(":STATUT_VACCINAL", STATUT_VACCINAL);
    query.bindValue(":DATE_VACCIN", DATE_VACCIN);
    query.bindValue(":ID_PROJET_TESTER", ID_PROJET_TESTER);
    return query.exec();
}

bool Patient::Supprimer(int ID_PATIENT)
{
    QSqlQuery query;
    query.prepare("DELETE FROM PATIENT WHERE ID_PATIENT=:ID_PATIENT");
    query.bindValue(":ID_PATIENT", ID_PATIENT);
    return query.exec();
}

QSqlQueryModel* Patient::Afficher()
{
    QSqlQueryModel* model = new QSqlQueryModel();
    model->setQuery("SELECT TO_CHAR(ID_PATIENT) AS ID_PATIENT, NOM, PRENOM, TO_CHAR(DATE_NAISSANCE, 'DD/MM/YYYY') AS DATE_NAISSANCE, SEXE, ADRESSE, TO_CHAR(TELEPHONE) AS TELEPHONE, EMAIL, GROUPE_SANGUIN, STATUT_VACCINAL, TO_CHAR(DATE_VACCIN, 'DD/MM/YYYY') AS DATE_VACCIN, TO_CHAR(ID_PROJET_TESTER) AS ID_PROJET_TESTER FROM PATIENT");
    model->setHeaderData(0, Qt::Horizontal, QObject::tr("ID_PATIENT"));
    model->setHeaderData(1, Qt::Horizontal, QObject::tr("NOM"));
    model->setHeaderData(2, Qt::Horizontal, QObject::tr("PRENOM"));
    model->setHeaderData(3, Qt::Horizontal, QObject::tr("DATE_NAISSANCE"));
    model->setHeaderData(4, Qt::Horizontal, QObject::tr("SEXE"));
    model->setHeaderData(5, Qt::Horizontal, QObject::tr("ADRESSE"));
    model->setHeaderData(6, Qt::Horizontal, QObject::tr("TELEPHONE"));
    model->setHeaderData(7, Qt::Horizontal, QObject::tr("EMAIL"));
    model->setHeaderData(8, Qt::Horizontal, QObject::tr("GROUPE_SANGUIN"));
    model->setHeaderData(9, Qt::Horizontal, QObject::tr("STATUT_VACCINAL"));
    model->setHeaderData(10, Qt::Horizontal, QObject::tr("DATE_VACCIN"));
    model->setHeaderData(11, Qt::Horizontal, QObject::tr("ID_PROJET_TESTER"));
    return model;
}

QSqlQueryModel* Patient::Afficher_ID()
{
    QSqlQueryModel* model = new QSqlQueryModel();
    model->setQuery("SELECT TO_CHAR(ID_PATIENT) AS ID_PATIENT FROM PATIENT");
    model->setHeaderData(0, Qt::Horizontal, QObject::tr("ID_PATIENT"));
    return model;
}

QSqlQueryModel* Patient::Afficher_Tri_NOM()
{
    QSqlQueryModel* model = new QSqlQueryModel();
    model->setQuery("SELECT TO_CHAR(ID_PATIENT) AS ID_PATIENT, NOM, PRENOM, TO_CHAR(DATE_NAISSANCE, 'DD/MM/YYYY') AS DATE_NAISSANCE, SEXE, ADRESSE, TO_CHAR(TELEPHONE) AS TELEPHONE, EMAIL, GROUPE_SANGUIN, STATUT_VACCINAL, TO_CHAR(DATE_VACCIN, 'DD/MM/YYYY') AS DATE_VACCIN, TO_CHAR(ID_PROJET_TESTER) AS ID_PROJET_TESTER FROM PATIENT ORDER BY NOM");
    model->setHeaderData(0, Qt::Horizontal, QObject::tr("ID_PATIENT"));
    model->setHeaderData(1, Qt::Horizontal, QObject::tr("NOM"));
    model->setHeaderData(2, Qt::Horizontal, QObject::tr("PRENOM"));
    model->setHeaderData(3, Qt::Horizontal, QObject::tr("DATE_NAISSANCE"));
    model->setHeaderData(4, Qt::Horizontal, QObject::tr("SEXE"));
    model->setHeaderData(5, Qt::Horizontal, QObject::tr("ADRESSE"));
    model->setHeaderData(6, Qt::Horizontal, QObject::tr("TELEPHONE"));
    model->setHeaderData(7, Qt::Horizontal, QObject::tr("EMAIL"));
    model->setHeaderData(8, Qt::Horizontal, QObject::tr("GROUPE_SANGUIN"));
    model->setHeaderData(9, Qt::Horizontal, QObject::tr("STATUT_VACCINAL"));
    model->setHeaderData(10, Qt::Horizontal, QObject::tr("DATE_VACCIN"));
    model->setHeaderData(11, Qt::Horizontal, QObject::tr("ID_PROJET_TESTER"));
    return model;
}

QSqlQueryModel* Patient::Afficher_Tri_STATUT_VACCINAL()
{
    QSqlQueryModel* model = new QSqlQueryModel();
    model->setQuery("SELECT TO_CHAR(ID_PATIENT) AS ID_PATIENT, NOM, PRENOM, TO_CHAR(DATE_NAISSANCE, 'DD/MM/YYYY') AS DATE_NAISSANCE, SEXE, ADRESSE, TO_CHAR(TELEPHONE) AS TELEPHONE, EMAIL, GROUPE_SANGUIN, STATUT_VACCINAL, TO_CHAR(DATE_VACCIN, 'DD/MM/YYYY') AS DATE_VACCIN, TO_CHAR(ID_PROJET_TESTER) AS ID_PROJET_TESTER FROM PATIENT ORDER BY STATUT_VACCINAL");
    model->setHeaderData(0, Qt::Horizontal, QObject::tr("ID_PATIENT"));
    model->setHeaderData(1, Qt::Horizontal, QObject::tr("NOM"));
    model->setHeaderData(2, Qt::Horizontal, QObject::tr("PRENOM"));
    model->setHeaderData(3, Qt::Horizontal, QObject::tr("DATE_NAISSANCE"));
    model->setHeaderData(4, Qt::Horizontal, QObject::tr("SEXE"));
    model->setHeaderData(5, Qt::Horizontal, QObject::tr("ADRESSE"));
    model->setHeaderData(6, Qt::Horizontal, QObject::tr("TELEPHONE"));
    model->setHeaderData(7, Qt::Horizontal, QObject::tr("EMAIL"));
    model->setHeaderData(8, Qt::Horizontal, QObject::tr("GROUPE_SANGUIN"));
    model->setHeaderData(9, Qt::Horizontal, QObject::tr("STATUT_VACCINAL"));
    model->setHeaderData(10, Qt::Horizontal, QObject::tr("DATE_VACCIN"));
    model->setHeaderData(11, Qt::Horizontal, QObject::tr("ID_PROJET_TESTER"));
    return model;
}

QSqlQueryModel* Patient::Afficher_Tri_GROUPE_SANGUIN()
{
    QSqlQueryModel* model = new QSqlQueryModel();
    model->setQuery("SELECT TO_CHAR(ID_PATIENT) AS ID_PATIENT, NOM, PRENOM, TO_CHAR(DATE_NAISSANCE, 'DD/MM/YYYY') AS DATE_NAISSANCE, SEXE, ADRESSE, TO_CHAR(TELEPHONE) AS TELEPHONE, EMAIL, GROUPE_SANGUIN, STATUT_VACCINAL, TO_CHAR(DATE_VACCIN, 'DD/MM/YYYY') AS DATE_VACCIN, TO_CHAR(ID_PROJET_TESTER) AS ID_PROJET_TESTER FROM PATIENT ORDER BY GROUPE_SANGUIN");
    model->setHeaderData(0, Qt::Horizontal, QObject::tr("ID_PATIENT"));
    model->setHeaderData(1, Qt::Horizontal, QObject::tr("NOM"));
    model->setHeaderData(2, Qt::Horizontal, QObject::tr("PRENOM"));
    model->setHeaderData(3, Qt::Horizontal, QObject::tr("DATE_NAISSANCE"));
    model->setHeaderData(4, Qt::Horizontal, QObject::tr("SEXE"));
    model->setHeaderData(5, Qt::Horizontal, QObject::tr("ADRESSE"));
    model->setHeaderData(6, Qt::Horizontal, QObject::tr("TELEPHONE"));
    model->setHeaderData(7, Qt::Horizontal, QObject::tr("EMAIL"));
    model->setHeaderData(8, Qt::Horizontal, QObject::tr("GROUPE_SANGUIN"));
    model->setHeaderData(9, Qt::Horizontal, QObject::tr("STATUT_VACCINAL"));
    model->setHeaderData(10, Qt::Horizontal, QObject::tr("DATE_VACCIN"));
    model->setHeaderData(11, Qt::Horizontal, QObject::tr("ID_PROJET_TESTER"));
    return model;
}

void Patient::clearTable(QTableView *table)
{
    QSqlQueryModel* modelFeragh = new QSqlQueryModel();
    modelFeragh->clear();
    table->setModel(modelFeragh);
}

void Patient::Recherche(QTableView *table, QString x)
{
    QSqlQueryModel *model = new QSqlQueryModel();
    QSqlQuery *query = new QSqlQuery;
    query->prepare("SELECT TO_CHAR(ID_PATIENT) AS ID_PATIENT, NOM, PRENOM, TO_CHAR(DATE_NAISSANCE, 'DD/MM/YYYY') AS DATE_NAISSANCE, SEXE, ADRESSE, TO_CHAR(TELEPHONE) AS TELEPHONE, EMAIL, GROUPE_SANGUIN, STATUT_VACCINAL, TO_CHAR(DATE_VACCIN, 'DD/MM/YYYY') AS DATE_VACCIN, TO_CHAR(ID_PROJET_TESTER) AS ID_PROJET_TESTER FROM PATIENT WHERE regexp_like(NOM, :X) OR regexp_like(EMAIL, :X) OR regexp_like(DATE_NAISSANCE, :X)");
    query->bindValue(":X", x);
    if (x.isEmpty())
    {
        query->prepare("SELECT TO_CHAR(ID_PATIENT) AS ID_PATIENT, NOM, PRENOM, TO_CHAR(DATE_NAISSANCE, 'DD/MM/YYYY') AS DATE_NAISSANCE, SEXE, ADRESSE, TO_CHAR(TELEPHONE) AS TELEPHONE, EMAIL, GROUPE_SANGUIN, STATUT_VACCINAL, TO_CHAR(DATE_VACCIN, 'DD/MM/YYYY') AS DATE_VACCIN, TO_CHAR(ID_PROJET_TESTER) AS ID_PROJET_TESTER FROM PATIENT");
    }
    query->exec();
    model->setQuery(*query);
    table->setModel(model);
    table->show();
}

QAbstractItemModel* Patient::GetDataForPDF(){
    QSqlQueryModel* model = new QSqlQueryModel();
    model->setQuery("SELECT * FROM PATIENT WHERE ID_PATIENT > 0");

    if (model->lastError().isValid()) {
        qDebug() << "Query Error:" << model->lastError().text();
    }

    return model;
}
