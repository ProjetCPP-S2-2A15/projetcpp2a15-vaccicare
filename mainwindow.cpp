#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    Vaccin v;

    ui->setupUi(this);
    Cnx.createconnect();
    UpdateTable(v);

    ui->comboBoxAgent->addItems({"Virus","Bactérie","Parasite","Champignon","Autre"});
    ui->comboBoxStatut->addItems({"En recherche","PhaseI","PhaseII","PhaseIII","Approuvé","Suspendu"});
    ui->comboBoxAutorisation->addItems({"FDA","EMA","OMS","ANSM","Autre"});

    connect(ui->pushButtonAjouter, &QPushButton::clicked, this, &MainWindow::on_ajouterButton_clicked);
    connect(ui->PushButtonModifier, &QPushButton::clicked, this, &MainWindow::on_ModifierButton_clicked);
    connect(ui->PushButtonSupprimer, &QPushButton::clicked, this, &MainWindow::on_SupprimerButton_clicked);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_ajouterButton_clicked() {
    int id = ui->lineEdit->text().toInt();
    QString nom = ui->lineEditNom->text();
    int idType = 0;
    QString agent = ui->comboBoxAgent->currentText();
    QString statut = ui->comboBoxStatut->currentText();
    QDate dateDev = ui->dateEditDateDev->date();
    QString pays = ui->lineEditPays->text();
    float temp = ui->lineEditTemp->text().toFloat();
    int stock = ui->lineEditStock->text().toInt();
    QDate datePer = ui->dateEditPeremption->date();
    QString autorisation = ui->comboBoxAutorisation->currentText();

    Vaccin v(id, nom, idType, agent, statut, dateDev, pays, temp, stock, datePer, autorisation);
    if (v.ajouter()) {
        QMessageBox::information(this, "Success", "Ajout Success !");
        UpdateTable(v);
    } else {
        QMessageBox::warning(this, "Erreur", "Ajout échoué !");
    }
}

void MainWindow::UpdateTable(Vaccin v) {
    ui->tableViewVaccins->setModel(v.afficher());
}

void MainWindow::on_ModifierButton_clicked() {
    int id = ui->lineEdit->text().toInt();
    QString nom = ui->lineEditNom->text();
    int idType = 0;
    QString agent = ui->comboBoxAgent->currentText();
    QString statut = ui->comboBoxStatut->currentText();
    QDate dateDev = ui->dateEditDateDev->date();
    QString pays = ui->lineEditPays->text();
    float temp = ui->lineEditTemp->text().toFloat();
    int stock = ui->lineEditStock->text().toInt();
    QDate datePer = ui->dateEditPeremption->date();
    QString autorisation = ui->comboBoxAutorisation->currentText();

    Vaccin v;
    if (!v.existe(id)) {
        QMessageBox::warning(this, "Erreur", "ID inexistant ! Impossible de modifier.");
        return;
    }

    v = Vaccin(id, nom, idType, agent, statut, dateDev, pays, temp, stock, datePer, autorisation);
    if (v.modifier()) {
        QMessageBox::information(this, "Success", "Modification Success !");
        UpdateTable(v);
    } else {
        QMessageBox::warning(this, "Erreur", "Modification échouée !");
    }
}

void MainWindow::on_SupprimerButton_clicked() {
    int id = ui->lineEdit->text().toInt();
    Vaccin v(id, "", 0, "", "", QDate(), "", 0, 0, QDate(), "");
    if (v.supprimer(id)) {
        QMessageBox::information(this, "Success", "Suppression Success !");
        UpdateTable(v);
    } else {
        QMessageBox::warning(this, "Erreur", "Suppression échouée !");
    }
}
