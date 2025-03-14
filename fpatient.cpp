#include "fpatient.h"

FPatient::FPatient() {}

#include "mainwindow.h"
#include "ui_mainwindow-.h"
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include <QMessageBox>
#include <QDebug>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    connectToDatabase();  // Connexion à la base de données

    // Connecter les boutons aux slots
    connect(ui->pushButtonAjouter, &QPushButton::clicked, this, &MainWindow::on_pushButtonAjouter_clicked);
    connect(ui->pushButtonModifier, &QPushButton::clicked, this, &MainWindow::on_pushButtonModifier_clicked);
    connect(ui->pushButtonSupprimer, &QPushButton::clicked, this, &MainWindow::on_pushButtonSupprimer_clicked);
    connect(ui->pushButtonAfficher, &QPushButton::clicked, this, &MainWindow::on_pushButtonAfficher_clicked);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::connectToDatabase() {
    db = QSqlDatabase::addDatabase("QSQLITE");  // Exemple pour une base SQLite
    db.setDatabaseName("database.db");  // Nom de la base de données

    if (!db.open()) {
        qDebug() << "Erreur de connexion à la base de données: " << db.lastError();
        QMessageBox::critical(this, "Erreur", "Impossible de se connecter à la base de données");
    }
}

void MainWindow::updateTable() {
    /*QSqlQueryModel *model = patient.afficher();
    ui->tableView->setModel(model);  // Assurez-vous d'avoir un QTableView dans votre interface*/
}

// Ajouter un patient
void MainWindow::on_pushButtonAjouter_clicked() {
    /* QMessageBox::information(nullptr, QObject::tr("database is open"),
                             QObject::tr("Ajouter.\n"
                                         "Click Cancel to exit."), QMessageBox::Cancel);*/
    int id = ui->lineEditId->text().toInt();
    QString nom = ui->lineEditNom->text();
    if(nom == ""){

        return
    }
    QString prenom = ui->lineEditPrenom->text();
    QString dateNaissance = ui->lineEditDateNaissance->text();
    QString sexe = ui->lineEditSexe->text();
    QString adresse = ui->lineEditAdresse->text();
    QString email = ui->lineEditEmail->text();
    QString telephone = ui->lineEditTelephone->text();
    QString cin = ui->lineEditCin->text();
    QString groupeSanguin = ui->lineEditGroupeSanguin->text();
    QString statutVaccinal = ui->lineEditStatutVaccinal->text();
    QString dateVaccin = ui->lineEditDateVaccin->text();

    patient.setId(id);
    patient.setNom(nom);
    patient.setPrenom(prenom);
    patient.setDateNaissance(dateNaissance);
    patient.setSexe(sexe);
    patient.setAdresse(adresse);
    patient.setEmail(email);
    patient.setTelephone(telephone);
    patient.setCin(cin);
    patient.setGroupeSanguin(groupeSanguin);
    patient.setStatutVaccinal(statutVaccinal);
    patient.setDateVaccin(dateVaccin);

    if (patient.ajouter()) {
        QMessageBox::information(this, "Succès", "Patient ajouté avec succès");
        updateTable();
    } else {
        QMessageBox::warning(this, "Erreur", "Échec de l'ajout du patient");
    }
}

// Modifier un patient
void MainWindow::on_pushButtonModifier_clicked() {

    /* QMessageBox::information(nullptr, QObject::tr("database is open"),
                             QObject::tr("Modifier.\n"
                                         "Click Cancel to exit."), QMessageBox::Cancel);*/
    int id = ui->lineEditId->text().toInt();
    QString nom = ui->lineEditNom->text();
    QString prenom = ui->lineEditPrenom->text();
    QString dateNaissance = ui->lineEditDateNaissance->text();
    QString sexe = ui->lineEditSexe->text();
    QString adresse = ui->lineEditAdresse->text();
    QString email = ui->lineEditEmail->text();
    QString telephone = ui->lineEditTelephone->text();
    QString cin = ui->lineEditCin->text();
    QString groupeSanguin = ui->lineEditGroupeSanguin->text();
    QString statutVaccinal = ui->lineEditStatutVaccinal->text();
    QString dateVaccin = ui->lineEditDateVaccin->text();

    patient.setId(id);
    patient.setNom(nom);
    patient.setPrenom(prenom);
    patient.setDateNaissance(dateNaissance);
    patient.setSexe(sexe);
    patient.setAdresse(adresse);
    patient.setEmail(email);
    patient.setTelephone(telephone);
    patient.setCin(cin);
    patient.setGroupeSanguin(groupeSanguin);
    patient.setStatutVaccinal(statutVaccinal);
    patient.setDateVaccin(dateVaccin);

    if (patient.modifier(id)) {
        QMessageBox::information(this, "Succès", "Patient modifié avec succès");
        updateTable();
    } else {
        QMessageBox::warning(this, "Erreur", "Échec de la modification du patient");
    }
}

// Supprimer un patient
void MainWindow::on_pushButtonSupprimer_clicked() {
    /*QMessageBox::information(nullptr, QObject::tr("database is open"),
                             QObject::tr("Supprimer.\n"
                                         "Click Cancel to exit."), QMessageBox::Cancel);*/
    int id = ui->lineEditId->text().toInt();

    if (patient.supprimer(id)) {
        QMessageBox::information(this, "Succès", "Patient supprimé avec succès");
        updateTable();
    } else {
        QMessageBox::warning(this, "Erreur", "Échec de la suppression du patient");
    }
}

// Afficher les patients
void MainWindow::on_pushButtonAfficher_clicked() {
    /*QMessageBox::information(nullptr, QObject::tr("database is open"),
                             QObject::tr("Afficher.\n"
                                         "Click Cancel to exit."), QMessageBox::Cancel);*/
    updateTable();
}
