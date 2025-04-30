#include "medecin.h"
#include <QDebug>
#include <QSqlQuery>
#include <QSqlError>
#include <QRegularExpression>
#include <QMessageBox>
#include <QSqlQueryModel>

Medecin::Medecin()
{
    id_medecin = 0;
    nom = "";
    specialite = "";
    grade = "";
    email = "";
    telephone = "";
    disponibilite = "";
    experience = 0;
    statut = "";
    login = "";
    mot_de_passe = "";
}

Medecin::Medecin(int id, QString n, QString s, QString g, QString e, QString t, int exp,
                 QString stat, QString disp, QString log, QString mdp)
    : id_medecin(id), nom(n), specialite(s), grade(g), email(e), telephone(t),
    experience(exp), statut(stat), disponibilite(disp), login(log), mot_de_passe(mdp)
{
}

bool Medecin::ajouter()
{
    // Input validation
    if (id_medecin <= 0) {
        QMessageBox::warning(nullptr, "Erreur", "L'ID du médecin doit être un nombre positif.");
        return false;
    }
    if (nom.trimmed().isEmpty()) {
        QMessageBox::warning(nullptr, "Erreur", "Le nom du médecin est obligatoire.");
        return false;
    }
    if (specialite.trimmed().isEmpty()) {
        QMessageBox::warning(nullptr, "Erreur", "La Specialite est obligatoire.");
        return false;
    }
    if (grade.trimmed().isEmpty()) {
        QMessageBox::warning(nullptr, "Erreur", "Le grade est obligatoire.");
        return false;
    }
    if (email.trimmed().isEmpty()) {
        QMessageBox::warning(nullptr, "Erreur", "L'email est obligatoire.");
        return false;
    }
    // Validate email format
    QRegularExpression regexEmail("^[a-zA-Z0-9._%+-]+@[a-zA-Z0-9.-]+\\.[a-zA-Z]{2,}$");
    if (!regexEmail.match(email).hasMatch()) {
        QMessageBox::warning(nullptr, "Erreur", "L'email n'est pas valide.");
        return false;
    }
    // Validate telephone (assuming 8 digits for simplicity, adjust as needed)
    QRegularExpression regexTel("^\\d{8}$");
    if (!telephone.isEmpty() && !regexTel.match(telephone).hasMatch()) {
        QMessageBox::warning(nullptr, "Erreur", "Le numéro de téléphone doit contenir exactement 8 chiffres.");
        return false;
    }
    if (experience < 0) {
        QMessageBox::warning(nullptr, "Erreur", "L'expérience doit être un nombre positif ou zéro.");
        return false;
    }
    if (statut.trimmed().isEmpty()) {
        QMessageBox::warning(nullptr, "Erreur", "Le statut est obligatoire.");
        return false;
    }
    if (disponibilite.trimmed().isEmpty()) {
        QMessageBox::warning(nullptr, "Erreur", "La disponibilité est obligatoire.");
        return false;
    }
    if (login.trimmed().isEmpty()) {
        QMessageBox::warning(nullptr, "Erreur", "Le login est obligatoire.");
        return false;
    }
    if (mot_de_passe.trimmed().isEmpty()) {
        QMessageBox::warning(nullptr, "Erreur", "Le mot de passe est obligatoire.");
        return false;
    }

    // Check if the ID already exists
    QSqlQuery checkQuery;
    checkQuery.prepare("SELECT COUNT(*) FROM MEDECIN WHERE ID_CHERCHEUR = :id");
    checkQuery.bindValue(":id", id_medecin);
    if (!checkQuery.exec()) {
        qDebug() << "Erreur lors de la vérification de l'ID :" << checkQuery.lastError().text();
        return false;
    }

    if (checkQuery.next() && checkQuery.value(0).toInt() > 0) {
        QMessageBox::warning(nullptr, "Erreur", "L'ID du médecin existe déjà.");
        return false;
    }

    // SQL query to insert a new doctor
    QSqlQuery query;
    query.prepare("INSERT INTO MEDECIN (ID_CHERCHEUR, NOM, SPECIALITE, GRADE, EMAIL, TELEPHONE, DISPONIBILITE, EXPERIENCE, STATUT, LOGIN, MOT_DE_PASSE) "
                  "VALUES (:id, :nom, :specialite, :grade, :email, :telephone, :disponibilite, :experience, :statut, :login, :mot_de_passe)");

    query.bindValue(":id", id_medecin);
    query.bindValue(":nom", nom);
    query.bindValue(":specialite", specialite);
    query.bindValue(":grade", grade);
    query.bindValue(":email", email);
    query.bindValue(":telephone", telephone.isEmpty() ? QVariant(QVariant::String) : telephone);
    query.bindValue(":disponibilite", disponibilite);
    query.bindValue(":experience", experience);
    query.bindValue(":statut", statut);
    query.bindValue(":login", login);
    query.bindValue(":mot_de_passe", mot_de_passe);

    bool success = query.exec();
    if (!success) {
        qDebug() << "Erreur SQL lors de l'ajout : " << query.lastError().text();
    } else {
        qDebug() << "Médecin ajouté avec succès.";
    }
    return success;
}

bool Medecin::modifier(int id)
{
    if (id <= 0) {
        QMessageBox::warning(nullptr, "Erreur", "L'ID du médecin doit être un nombre positif.");
        return false;
    }
    if (nom.trimmed().isEmpty()) {
        QMessageBox::warning(nullptr, "Erreur", "Le nom du médecin est obligatoire.");
        return false;
    }
    if (specialite.trimmed().isEmpty()) {
        QMessageBox::warning(nullptr, "Erreur", "La Specialite est obligatoire.");
        return false;
    }
    if (grade.trimmed().isEmpty()) {
        QMessageBox::warning(nullptr, "Erreur", "Le grade est obligatoire.");
        return false;
    }
    if (email.trimmed().isEmpty()) {
        QMessageBox::warning(nullptr, "Erreur", "L'email est obligatoire.");
        return false;
    }
    QRegularExpression regexEmail("^[a-zA-Z0-9._%+-]+@[a-zA-Z0-9.-]+\\.[a-zA-Z]{2,}$");
    if (!regexEmail.match(email).hasMatch()) {
        QMessageBox::warning(nullptr, "Erreur", "L'email n'est pas valide.");
        return false;
    }
    QRegularExpression regexTel("^\\d{8}$");
    if (!telephone.isEmpty() && !regexTel.match(telephone).hasMatch()) {
        QMessageBox::warning(nullptr, "Erreur", "Le numéro de téléphone doit contenir exactement 8 chiffres.");
        return false;
    }
    if (experience < 0) {
        QMessageBox::warning(nullptr, "Erreur", "L'expérience doit être un nombre positif ou zéro.");
        return false;
    }
    if (statut.trimmed().isEmpty()) {
        QMessageBox::warning(nullptr, "Erreur", "Le statut est obligatoire.");
        return false;
    }
    if (disponibilite.trimmed().isEmpty()) {
        QMessageBox::warning(nullptr, "Erreur", "La disponibilité est obligatoire.");
        return false;
    }
    if (login.trimmed().isEmpty()) {
        QMessageBox::warning(nullptr, "Erreur", "Le login est obligatoire.");
        return false;
    }
    if (mot_de_passe.trimmed().isEmpty()) {
        QMessageBox::warning(nullptr, "Erreur", "Le mot de passe est obligatoire.");
        return false;
    }

    // Check if the ID exists
    QSqlQuery checkQuery;
    checkQuery.prepare("SELECT COUNT(*) FROM MEDECIN WHERE ID_CHERCHEUR = :id");
    checkQuery.bindValue(":id", id);
    if (!checkQuery.exec()) {
        qDebug() << "Erreur lors de la vérification de l'ID :" << checkQuery.lastError().text();
        return false;
    }

    if (checkQuery.next() && checkQuery.value(0).toInt() == 0) {
        QMessageBox::warning(nullptr, "Erreur", "L'ID du médecin n'existe pas.");
        return false;
    }

    // Update the doctor
    QSqlQuery query;
    query.prepare("UPDATE MEDECIN SET NOM = :nom, SPECIALITE = :specialite, GRADE = :grade, "
                  "EMAIL = :email, TELEPHONE = :telephone, DISPONIBILITE = :disponibilite, "
                  "EXPERIENCE = :experience, STATUT = :statut, LOGIN = :login, MOT_DE_PASSE = :mot_de_passe "
                  "WHERE ID_CHERCHEUR = :id");

    query.bindValue(":id", id);
    query.bindValue(":nom", nom);
    query.bindValue(":specialite", specialite);
    query.bindValue(":grade", grade);
    query.bindValue(":email", email);
    query.bindValue(":telephone", telephone.isEmpty() ? QVariant(QVariant::String) : telephone);
    query.bindValue(":disponibilite", disponibilite);
    query.bindValue(":experience", experience);
    query.bindValue(":statut", statut);
    query.bindValue(":login", login);
    query.bindValue(":mot_de_passe", mot_de_passe);

    bool success = query.exec();
    if (!success) {
        qDebug() << "Erreur SQL lors de la modification : " << query.lastError().text();
    } else {
        qDebug() << "Médecin modifié avec succès.";
    }
    return success;
}

bool Medecin::supprimer(int id)
{
    // Validate the ID
    if (id <= 0) {
        QMessageBox::warning(nullptr, "Erreur", "L'ID du médecin doit être un nombre positif.");
        return false;
    }

    // Check if the ID exists
    QSqlQuery checkQuery;
    checkQuery.prepare("SELECT COUNT(*) FROM MEDECIN WHERE ID_CHERCHEUR = :id");
    checkQuery.bindValue(":id", id);
    if (!checkQuery.exec()) {
        qDebug() << "Erreur lors de la vérification de l'ID :" << checkQuery.lastError().text();
        return false;
    }

    if (checkQuery.next() && checkQuery.value(0).toInt() == 0) {
        QMessageBox::warning(nullptr, "Erreur", "L'ID du médecin n'existe pas.");
        return false;
    }

    // Delete the record
    QSqlQuery query;
    query.prepare("DELETE FROM MEDECIN WHERE ID_CHERCHEUR = :id");
    query.bindValue(":id", id);

    bool success = query.exec();
    if (!success) {
        qDebug() << "Erreur SQL lors de la suppression : " << query.lastError().text();
    } else {
        qDebug() << "Médecin avec ID " << id << " supprimé avec succès.";
    }
    return success;
}

bool Medecin::fetchMedecinById(int id, QString &nom, QString &specialite, QString &grade, QString &email,
                               QString &telephone, QString &disponibilite, int &experience,
                               QString &statut, QString &login, QString &mot_de_passe)
{
    QSqlQuery query;
    query.prepare("SELECT * FROM MEDECIN WHERE ID_CHERCHEUR = :id");
    query.bindValue(":id", id);

    if (query.exec()) {
        if (query.next()) {
            nom = query.value("NOM").toString();
            specialite = query.value("SPECIALITE").toString();
            grade = query.value("GRADE").toString();
            email = query.value("EMAIL").toString();
            telephone = query.value("TELEPHONE").toString();
            disponibilite = query.value("DISPONIBILITE").toString();
            experience = query.value("EXPERIENCE").toInt();
            statut = query.value("STATUT").toString();
            login = query.value("LOGIN").toString();
            mot_de_passe = query.value("MOT_DE_PASSE").toString();
            return true;
        }
    } else {
        qDebug() << "Error fetching doctor by ID:" << query.lastError().text();
    }
    return false;
}

QSqlQueryModel* Medecin::afficher(const QString &searchText, const QString &criterion, const QString &sortCriterion)
{
    QSqlQueryModel* model = new QSqlQueryModel();
    QSqlQuery query;

    // Determine the search column based on the criterion parameter
    QString columnName;
    if (criterion == "Nom") {
        columnName = "NOM";
    } else if (criterion == "Specialite") {
        columnName = "SPECIALITE";
    } else {
        qDebug() << "Invalid criterion:" << criterion << "- Displaying all doctors.";
        columnName = ""; // Will display all doctors if criterion is invalid
    }

    // Determine the sort column based on the sortCriterion parameter
    QString sortColumn;
    if (sortCriterion == "Id") {
        sortColumn = "ID_CHERCHEUR";
    } else if (sortCriterion == "Nom") {
        sortColumn = "NOM";
    } else if (sortCriterion == "Specialite") {
        sortColumn = "SPECIALITE";
    } else if (sortCriterion == "Grade") {
        sortColumn = "GRADE";
    } else {
        qDebug() << "Invalid sort criterion:" << sortCriterion << "- Sorting by ID_CHERCHEUR.";
        sortColumn = "ID_CHERCHEUR"; // Default to sorting by ID
    }

    // Prepare the query
    QString queryString = "SELECT ID_CHERCHEUR, NOM, SPECIALITE, GRADE, EMAIL, TELEPHONE, DISPONIBILITE, "
                          "EXPERIENCE, STATUT, LOGIN, MOT_DE_PASSE FROM MEDECIN";
    if (!searchText.isEmpty() && !columnName.isEmpty()) {
        queryString += QString(" WHERE %1 LIKE :searchText").arg(columnName);
    }
    queryString += QString(" ORDER BY %1").arg(sortColumn);

    qDebug() << "Executing query:" << queryString;
    query.prepare(queryString);

    // Bind the search text with wildcards for LIKE
    if (!searchText.isEmpty() && !columnName.isEmpty()) {
        query.bindValue(":searchText", "%" + searchText + "%");
        qDebug() << "Bound searchText:" << "%" + searchText + "%";
    }

    if (query.exec()) {
        model->setQuery(query);
        model->setHeaderData(0, Qt::Horizontal, QObject::tr("ID Médecin"));
        model->setHeaderData(1, Qt::Horizontal, QObject::tr("Nom"));
        model->setHeaderData(2, Qt::Horizontal, QObject::tr("Specialite"));
        model->setHeaderData(3, Qt::Horizontal, QObject::tr("Grade"));
        model->setHeaderData(4, Qt::Horizontal, QObject::tr("Email"));
        model->setHeaderData(5, Qt::Horizontal, QObject::tr("Téléphone"));
        model->setHeaderData(6, Qt::Horizontal, QObject::tr("Disponibilité"));
        model->setHeaderData(7, Qt::Horizontal, QObject::tr("Expérience"));
        model->setHeaderData(8, Qt::Horizontal, QObject::tr("Statut"));
        model->setHeaderData(9, Qt::Horizontal, QObject::tr("Login"));
        model->setHeaderData(10, Qt::Horizontal, QObject::tr("Mot de Passe"));
    } else {
        qDebug() << "Error executing query:" << query.lastError().text();
        delete model;
        return nullptr;
    }

    qDebug() << "Query executed successfully. Row count:" << model->rowCount();
    return model;
}
bool Medecin::verifierLogin(const QString &login, const QString &mot_de_passe)
{
    QSqlQuery query;
    query.prepare("SELECT LOGIN, MOT_DE_PASSE FROM MEDECIN WHERE LOGIN = :login AND MOT_DE_PASSE = :mot_de_passe");
    query.bindValue(":login", login);
    query.bindValue(":mot_de_passe", mot_de_passe);

    if (query.exec()) {
        if (query.next()) {
            return true;
        }
    } else {
        qDebug() << "Erreur lors de la vérification du login :" << query.lastError().text();
    }
    return false;
}
