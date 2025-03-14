#include "medecin.h"
#include "ui_medecin.h"
#include "connection.h"
#include <QMessageBox>
#include <QDebug>
#include <QSqlQueryModel>
#include <QRegularExpression>

// Constructeur pour la fenêtre UI
medecin::medecin(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::medecin)
{
    ui->setupUi(this);
    connect(ui->ajouter, &QPushButton::clicked, this, &medecin::on_ajouter_clicked);
    connect(ui->afficher, &QPushButton::clicked, this, &medecin::on_afficher_clicked);
    connect(ui->supprimer, &QPushButton::clicked, this, &medecin::on_supprimer_clicked);
    connect(ui->modifier, &QPushButton::clicked, this, &medecin::on_modifier_clicked);
}

// Constructeur pour stocker les données du médecin
medecin::medecin(int id_cherch, const QString& nom, const QString& specialite, const QString& grade,
                 const QString& email, int telephone, const QString& disponibilite,
                 int experience, const QString& statut, const QString& login,
                 const QString& motDePasse, QWidget *parent)
    : QMainWindow(parent), ui(new Ui::medecin)
{
    this->id_cherch = id_cherch;
    this->nom = nom;
    this->specialite = specialite;
    this->grade = grade;
    this->email = email;
    this->telephone = telephone;
    this->disponibilite = disponibilite;
    this->experience = experience;
    this->statut = statut;
    this->login = login;
    this->motDePasse = motDePasse;

    ui->setupUi(this);
}

medecin::~medecin()
{
    delete ui;
}

void medecin::on_ajouter_clicked()
{
    // Vérifier que tous les champs sont remplis
    if (ui->lineEdit_16->text().isEmpty() || ui->lineEdit_2->text().isEmpty() ||
        ui->lineEdit_3->text().isEmpty() || ui->lineEdit_4->text().isEmpty() ||
        ui->lineEdit_5->text().isEmpty() || ui->lineEdit_6->text().isEmpty() ||
        ui->lineEdit_7->text().isEmpty() || ui->lineEdit_8->text().isEmpty() ||
        ui->lineEdit_9->text().isEmpty() || ui->lineEdit_10->text().isEmpty() ||
        ui->lineEdit_11->text().isEmpty()) {
        QMessageBox::warning(this, "Erreur", "Tous les champs sont obligatoires !");
        return;
    }

    // Contrôle de l'ID
    bool idOk;
    int id_cherch = ui->lineEdit_16->text().toInt(&idOk);
    if (!idOk || id_cherch <= 0) {
        QMessageBox::warning(this, "Erreur", "L'ID doit être un nombre entier positif !");
        return;
    }

    // Contrôle de l'email
    QString email = ui->lineEdit_5->text();
    QRegularExpression emailRegex(R"((^[a-zA-Z0-9_.+-]+@[a-zA-Z0-9-]+\.[a-zA-Z0-9-.]+$))");
    if (!emailRegex.match(email).hasMatch()) {
        QMessageBox::warning(this, "Erreur", "L'email doit être au format 'example@gmail.com' !");
        return;
    }

    // Contrôle du téléphone
    QString telephoneStr = ui->lineEdit_6->text();
    if (telephoneStr.length() != 8) {
        QMessageBox::warning(this, "Erreur", "Le numéro de téléphone doit contenir exactement 8 chiffres !");
        return;
    }

    bool telephoneOk, experienceOk;
    int telephone = telephoneStr.toInt(&telephoneOk);
    int experience = ui->lineEdit_8->text().toInt(&experienceOk);

    if (!telephoneOk || !experienceOk) {
        QMessageBox::warning(this, "Erreur", "Le téléphone et l'expérience doivent être des nombres entiers !");
        return;
    }

    // Si tous les contrôles sont passés, on procède à l'ajout
    QString nom = ui->lineEdit_2->text();
    QString specialite = ui->lineEdit_3->text();
    QString grade = ui->lineEdit_4->text();
    QString disponibilite = ui->lineEdit_7->text();
    QString statut = ui->lineEdit_9->text();
    QString login = ui->lineEdit_10->text();
    QString motDePasse = ui->lineEdit_11->text();

    medecin M(id_cherch, nom, specialite, grade, email, telephone, disponibilite, experience, statut, login, motDePasse);

    bool test = M.ajouter();
    if (test) {
        QMessageBox::information(this, "Succès", "Médecin ajouté avec succès !");
        qDebug() << "Médecin ajouté avec succès.";
    } else {
        QMessageBox::critical(this, "Erreur", "Ajout non effectué !");
        qDebug() << "Erreur lors de l'ajout du médecin.";
    }
}

void medecin::on_afficher_clicked()
{
    medecin med;
    std::unique_ptr<QSqlQueryModel> model = med.afficher();

    if (model) {
        // Définir les en-têtes de colonnes
        model->setHeaderData(0, Qt::Horizontal, tr("ID"));
        model->setHeaderData(1, Qt::Horizontal, tr("Nom"));
        model->setHeaderData(2, Qt::Horizontal, tr("Spécialité"));
        model->setHeaderData(3, Qt::Horizontal, tr("Grade"));
        model->setHeaderData(4, Qt::Horizontal, tr("Email"));
        model->setHeaderData(5, Qt::Horizontal, tr("Téléphone"));
        model->setHeaderData(6, Qt::Horizontal, tr("Disponibilité"));
        model->setHeaderData(7, Qt::Horizontal, tr("Expérience"));
        model->setHeaderData(8, Qt::Horizontal, tr("Statut"));
        model->setHeaderData(9, Qt::Horizontal, tr("Login"));
        model->setHeaderData(10, Qt::Horizontal, tr("Mot de passe"));

        // Appliquer le modèle à la table view
        ui->tableView->setModel(model.release());
        ui->tableView->resizeColumnsToContents();
    } else {
        QMessageBox::critical(this, "Erreur", "Impossible d'afficher les médecins.");
    }
}

void medecin::on_supprimer_clicked()
{
    bool idOk;
    int id_cherch = ui->lineEdit->text().toInt(&idOk);

    if (!idOk || id_cherch <= 0) {
        QMessageBox::warning(this, "Erreur", "Veuillez entrer un ID valide (nombre entier positif) !");
        return;
    }

    medecin med;
    bool success = med.supprimer(id_cherch);

    if (success) {
        QMessageBox::information(this, "Succès", "Médecin supprimé avec succès !");
        qDebug() << "Médecin supprimé avec succès.";
    } else {
        QMessageBox::critical(this, "Erreur", "Échec de la suppression du médecin. L'ID n'existe pas dans la base de données.");
        qDebug() << "Erreur lors de la suppression du médecin. L'ID n'existe pas.";
    }
    on_afficher_clicked();
}

void medecin::on_modifier_clicked()
{
    // Vérifier que tous les champs sont remplis
    if (ui->lineEdit_16->text().isEmpty() || ui->lineEdit_2->text().isEmpty() ||
        ui->lineEdit_3->text().isEmpty() || ui->lineEdit_4->text().isEmpty() ||
        ui->lineEdit_5->text().isEmpty() || ui->lineEdit_6->text().isEmpty() ||
        ui->lineEdit_7->text().isEmpty() || ui->lineEdit_8->text().isEmpty() ||
        ui->lineEdit_9->text().isEmpty() || ui->lineEdit_10->text().isEmpty() ||
        ui->lineEdit_11->text().isEmpty()) {
        QMessageBox::warning(this, "Erreur", "Tous les champs sont obligatoires !");
        return;
    }

    // Contrôle de l'ID
    bool idOk;
    int id_cherch = ui->lineEdit_16->text().toInt(&idOk);
    if (!idOk || id_cherch <= 0) {
        QMessageBox::warning(this, "Erreur", "L'ID doit être un nombre entier positif !");
        return;
    }

    // Contrôle de l'email
    QString email = ui->lineEdit_5->text();
    QRegularExpression emailRegex(R"((^[a-zA-Z0-9_.+-]+@[a-zA-Z0-9-]+\.[a-zA-Z0-9-.]+$))");
    if (!emailRegex.match(email).hasMatch()) {
        QMessageBox::warning(this, "Erreur", "L'email doit être au format 'example@gmail.com' !");
        return;
    }

    // Contrôle du téléphone
    QString telephoneStr = ui->lineEdit_6->text();
    if (telephoneStr.length() != 8) {
        QMessageBox::warning(this, "Erreur", "Le numéro de téléphone doit contenir exactement 8 chiffres !");
        return;
    }

    bool telephoneOk, experienceOk;
    int telephone = telephoneStr.toInt(&telephoneOk);
    int experience = ui->lineEdit_8->text().toInt(&experienceOk);

    if (!telephoneOk || !experienceOk) {
        QMessageBox::warning(this, "Erreur", "Le téléphone et l'expérience doivent être des nombres entiers !");
        return;
    }

    // Si tous les contrôles sont passés, on procède à la modification
    QString nom = ui->lineEdit_2->text();
    QString specialite = ui->lineEdit_3->text();
    QString grade = ui->lineEdit_4->text();
    QString disponibilite = ui->lineEdit_7->text();
    QString statut = ui->lineEdit_9->text();
    QString login = ui->lineEdit_10->text();
    QString motDePasse = ui->lineEdit_11->text();

    medecin M(id_cherch, nom, specialite, grade, email, telephone, disponibilite, experience, statut, login, motDePasse);
    bool success = M.modifier(id_cherch);

    if (success) {
        QMessageBox::information(this, "Succès", "Médecin modifié avec succès !");
        qDebug() << "Médecin modifié avec succès.";
    } else {
        QMessageBox::critical(this, "Erreur", "Échec de la modification du médecin.");
        qDebug() << "Erreur lors de la modification du médecin.";
    }
    on_afficher_clicked();
}

// Implémentation des fonctions de base de données

bool medecin::ajouter()
{
    QSqlQuery query;

    query.prepare("INSERT INTO medecin (ID_CHERCHEUR, NOM, SPECIALITE, GRADE, EMAIL, TELEPHONE, DISPONIBILITE, EXPERIENCE, STATUT, LOGIN, MOT_DE_PASSE) "
                  "VALUES (:id, :nom, :specialite, :grade, :email, :telephone, :disponibilite, :experience, :statut, :login, :motDePasse)");
    query.bindValue(":id", id_cherch);
    query.bindValue(":nom", nom);
    query.bindValue(":specialite", specialite);
    query.bindValue(":grade", grade);
    query.bindValue(":email", email);
    query.bindValue(":telephone", telephone);
    query.bindValue(":disponibilite", disponibilite);
    query.bindValue(":experience", experience);
    query.bindValue(":statut", statut);
    query.bindValue(":login", login);
    query.bindValue(":motDePasse", motDePasse);
    qDebug() << "Requête SQL : " << query.executedQuery();
    if (!query.exec()) {
        qDebug() << "Erreur SQL : " << query.lastError().text();
        return false;
    }

    return true;
}

bool medecin::supprimer(int id)
{
    QSqlQuery query;

    // Vérifier si l'ID existe dans la base de données
    query.prepare("SELECT ID_CHERCHEUR FROM medecin WHERE ID_CHERCHEUR = :id");
    query.bindValue(":id", id);

    if (!query.exec()) {
        qDebug() << "Erreur SQL lors de la vérification de l'ID : " << query.lastError().text();
        return false;
    }

    if (!query.next()) {
        // L'ID n'existe pas dans la base de données
        qDebug() << "L'ID" << id << "n'existe pas dans la base de données.";
        return false;
    }

    // Si l'ID existe, procéder à la suppression
    query.prepare("DELETE FROM medecin WHERE ID_CHERCHEUR = :id");
    query.bindValue(":id", id);

    if (!query.exec()) {
        qDebug() << "Erreur SQL lors de la suppression : " << query.lastError().text();
        return false;
    }

    return true;
}

bool medecin::modifier(int id)
{
    QSqlQuery query;
    query.prepare("UPDATE medecin SET NOM = :nom, SPECIALITE = :specialite, GRADE = :grade, "
                  "EMAIL = :email, TELEPHONE = :telephone, DISPONIBILITE = :disponibilite, "
                  "EXPERIENCE = :experience, STATUT = :statut, LOGIN = :login, MOT_DE_PASSE = :motDePasse "
                  "WHERE ID_CHERCHEUR = :id");

    query.bindValue(":id", id);
    query.bindValue(":nom", nom);
    query.bindValue(":specialite", specialite);
    query.bindValue(":grade", grade);
    query.bindValue(":email", email);
    query.bindValue(":telephone", telephone);
    query.bindValue(":disponibilite", disponibilite);
    query.bindValue(":experience", experience);
    query.bindValue(":statut", statut);
    query.bindValue(":login", login);
    query.bindValue(":motDePasse", motDePasse);

    return query.exec();
}

std::unique_ptr<QSqlQueryModel> medecin::afficher()
{
    auto model = std::make_unique<QSqlQueryModel>();
    model->setQuery("SELECT * FROM medecin where ID_CHERCHEUR > 0");

    if (model->lastError().isValid()) {
        qDebug() << "SQL Error: " << model->lastError().text();
        return nullptr;
    }

    return model;
}
