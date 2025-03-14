#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "patient.h"
#include <QMessageBox>
#include <QSqlError>
#include <QRegularExpressionValidator>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    Connection Cnx;
    Cnx.createconnect();
    updateTable();

    // Ajout des validateurs
    QRegularExpressionValidator* idValidator = new QRegularExpressionValidator(QRegularExpression("^\\d{4}$"), this);
    ui->ID->setValidator(idValidator);

    QRegularExpressionValidator* nameValidator = new QRegularExpressionValidator(QRegularExpression("^[a-z]+$"), this);
    ui->bNom->setValidator(nameValidator);
    ui->bPrenom->setValidator(nameValidator);

    QRegularExpressionValidator* adresseValidator = new QRegularExpressionValidator(QRegularExpression("^[a-zA-Z\\s]{1,20}$"), this);
    ui->bAdresse->setValidator(adresseValidator);

    QRegularExpressionValidator* phoneCINValidator = new QRegularExpressionValidator(QRegularExpression("^\\d{8}$"), this);
    ui->bTelephone->setValidator(phoneCINValidator);
    //ui->bCIN->setValidator(phoneCINValidator);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::updateTable() {
    ui->tableView->setModel(patient.afficher());
}

// Fonctions de validation
bool MainWindow::validateID() {
    QString id = ui->ID->text();
    QRegularExpression regex("^\\d{4}$"); // 4 chiffres exactement
    return regex.match(id).hasMatch();
}

bool MainWindow::validateName(const QString& name) {
    QRegularExpression regex("^[a-z]+$"); // Seulement des lettres minuscules
    return regex.match(name).hasMatch();
}

bool MainWindow::validateDate(const QString& date) {
    QDate qDate = QDate::fromString(date, "yyyy-MM-dd");
    return qDate.isValid();
}

// bool MainWindow::validateSexe() {
//     return ui->radioButton->isChecked() || ui->HOMME->isChecked(); // Une seule case cochée
// }

bool MainWindow::validateAdresse() {
    QString adresse = ui->bAdresse->text();
    QRegularExpression regex("^[a-zA-Z\\s]{1,20}$"); // Lettres et espaces, max 20 caractères
    return regex.match(adresse).hasMatch();
}

bool MainWindow::validatePhoneOrCIN(const QString& input) {
    QRegularExpression regex("^\\d{8}$"); // 8 chiffres exactement
    return regex.match(input).hasMatch();
}

// bool MainWindow::validateGroupeSanguin() {
//     // Utilisez les noms des QRadioButton tels qu'ils sont définis dans le fichier .ui
//     return ui->radioButton->isChecked() || ui->radioButton_2->isChecked() ||
//            ui->radioButton_3->isChecked() || ui->radioButton_4->isChecked();
// }

// bool MainWindow::validateStatutVaccinal() {
//     // Utilisez les noms des QRadioButton tels qu'ils sont définis dans le fichier .ui
//     return ui->radioButton_5->isChecked() || ui->radioButton_6->isChecked();
// }

bool MainWindow::validateDateVaccin() {
    QString dateVaccin = ui->bDateduvaccin->date().toString("yyyy-MM-dd");
    return validateDate(dateVaccin); // Utilise la même fonction que pour la date de naissance
}

void MainWindow::on_Ajouter_clicked() {
    // Validation des champs
    if (!validateID()) {
        QMessageBox::warning(this, "Erreur", "L'ID doit contenir exactement 4 chiffres.");
        return;
    }
    if (!validateName(ui->bNom->text())) {
        QMessageBox::warning(this, "Erreur", "Le nom doit contenir uniquement des lettres minuscules.");
        return;
    }
    if (!validateName(ui->bPrenom->text())) {
        QMessageBox::warning(this, "Erreur", "Le prénom doit contenir uniquement des lettres minuscules.");
        return;
    }
    if (!validateDate(ui->bDate->date().toString("yyyy-MM-dd"))) {
        QMessageBox::warning(this, "Erreur", "La date de naissance est invalide.");
        return;
    }
    // if (!validateSexe()) {
    //     QMessageBox::warning(this, "Erreur", "Veuillez sélectionner un sexe.");
    //     return;
    // }
    if (!validateAdresse()) {
        QMessageBox::warning(this, "Erreur", "L'adresse doit contenir uniquement des lettres et ne pas dépasser 20 caractères.");
        return;
    }
    if (!validatePhoneOrCIN(ui->bTelephone->text())) {
        QMessageBox::warning(this, "Erreur", "Le téléphone doit contenir exactement 8 chiffres.");
        return;
    }
    // if (!validatePhoneOrCIN(ui->bCIN->text())) {
    //     QMessageBox::warning(this, "Erreur", "Le CIN doit contenir exactement 8 chiffres.");
    //     return;
    // }
    // if (!validateGroupeSanguin()) {
    //     QMessageBox::warning(this, "Erreur", "Veuillez sélectionner un groupe sanguin.");
    //     return;
    // }
    // if (!validateStatutVaccinal()) {
    //     QMessageBox::warning(this, "Erreur", "Veuillez sélectionner un statut vaccinal.");
    //     return;
    // }
    // if (!validateDateVaccin()) {
    //     QMessageBox::warning(this, "Erreur", "La date du vaccin est invalide.");
    //     return;
    // }

    // Si toutes les validations passent, ajouter le patient
    int id = ui->ID->text().toInt();
    QString nom = ui->bNom->text();
    QString prenom = ui->bPrenom->text();
    QString dateNaissance = ui->bDate->date().toString("yyyy-MM-dd");
    QString sexe = ui->sexebox->currentText();
    QString adresse = ui->bAdresse->text();
    QString email = ui->bEmail->text();
    QString telephone = ui->bTelephone->text();
    QString cin = "";
    QString groupeSanguin = ui->Groupesanguin_2->text(); // Correction ici
    QString statutVaccinal = ui->Statutvaccinal->text(); // Correction ici
    QString dateVaccin = ui->bDateduvaccin->date().toString("yyyy-MM-dd");

    Patient p(id, nom, prenom, dateNaissance, sexe, adresse, email, telephone, cin, groupeSanguin, statutVaccinal, dateVaccin);
    if (p.ajouter()) {
        QMessageBox::information(this, "Succès", "Patient ajouté avec succès.");
        updateTable();
    } else {
        QMessageBox::critical(this, "Erreur", "Échec de l'ajout du patient.");
    }
}

void MainWindow::on_Supprimer_clicked() {
    int id = ui->ID->text().toInt();

    if (id <= 0) {
        QMessageBox::warning(this, "Erreur", "ID invalide.");
        return;
    }

    if (patient.supprimer(id)) {
        QMessageBox::information(this, "Succès", "Patient supprimé avec succès.");
        updateTable();
    } else {
        QMessageBox::critical(this, "Erreur", "Échec de la suppression du patient.");
    }
}

void MainWindow::on_Modifier_clicked() {
    int id = ui->ID->text().toInt();
    QString nom = ui->bNom->text();
    QString prenom = ui->bPrenom->text();
    QString dateNaissance = ui->bDate->date().toString("yyyy-MM-dd");
    QString sexe = ui->sexebox->currentText();
    QString adresse = ui->bAdresse->text();
    QString email = ui->bEmail->text();
    QString telephone = ui->bTelephone->text();
    QString cin = "";
    QString groupeSanguin = ui->Groupesanguin_2->text(); // Correction ici
    QString statutVaccinal = ui->Statutvaccinal->text(); // Correction ici
    QString dateVaccin = ui->bDateduvaccin->date().toString("yyyy-MM-dd");

    Patient p(id, nom, prenom, dateNaissance, sexe, adresse, email, telephone, cin, groupeSanguin, statutVaccinal, dateVaccin);
    if (p.modifier(id)) {
        QMessageBox::information(this, "Succès", "Patient modifié avec succès.");
        updateTable();
    } else {
        QMessageBox::critical(this, "Erreur", "Échec de la modification du patient.");
    }
}

void MainWindow::on_Rechercher_clicked() {
    QString searchText = ui->Barrerecherche->text();
    QSqlQueryModel* model = new QSqlQueryModel();
    model->setQuery("SELECT * FROM PATIENT WHERE Nom LIKE '%" + searchText + "%' OR Prenom LIKE '%" + searchText + "%'");
    ui->tableView->setModel(model);
}
