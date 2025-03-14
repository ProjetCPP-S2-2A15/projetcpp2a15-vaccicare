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

    connect(ui->pushButtonAjouter,&QPushButton::clicked,this,&MainWindow::on_ajouterButton_clicked);
    connect(ui->PushButtonModifier,&QPushButton::clicked,this,&MainWindow::on_ModifierButton_clicked);
    connect(ui->PushButtonSupprimer,&QPushButton::clicked,this,&MainWindow::on_SupprimerButton_clicked);

}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_ajouterButton_clicked() {
    int id = ui->lineEditID->text().toInt();
    QString nom = ui->lineEditNom->text();
    //int idType = ui->lineEditType->text().toInt();
    int idType = 0;
    QString agent = ui->lineEditAgent->text();
    QString statut = ui->lineEditStatut->text();
    //int dateDev = ui->lineEditDateDev->text().toInt();
    int dateDev = 100;
    QString pays = ui->lineEditPays->text();
    float temp = ui->lineEditTemp->text().toFloat();
    int stock = ui->lineEditStock->text().toInt();
    //int datePer = ui->lineEditPeremption->text().toInt();
    int datePer = 200;
    QString autorisation = ui->lineEditAutorisation->text();

    Vaccin v(id, nom, idType, agent, statut, dateDev, pays, temp, stock, datePer, autorisation);
    if (v.ajouter()) {
        QMessageBox::warning(this, "Success", "Ajout Success !");
        UpdateTable(v);
    } else {
        QMessageBox::warning(this, "Erreur", "Ajout échoué !");
    }
}

void MainWindow::UpdateTable(Vaccin v){
    ui->tableViewVaccins->setModel(v.afficher());
}

void MainWindow::on_ModifierButton_clicked(){
    int id = ui->lineEditID->text().toInt();
    QString nom = ui->lineEditNom->text();
    //int idType = ui->lineEditType->text().toInt();
    int idType = 0;
    QString agent = ui->lineEditAgent->text();
    QString statut = ui->lineEditStatut->text();
    //int dateDev = ui->lineEditDateDev->text().toInt();
    int dateDev = 100;
    QString pays = ui->lineEditPays->text();
    float temp = ui->lineEditTemp->text().toFloat();
    int stock = ui->lineEditStock->text().toInt();
    //int datePer = ui->lineEditPeremption->text().toInt();
    int datePer = 200;
    QString autorisation = ui->lineEditAutorisation->text();

    Vaccin v;
    if (!v.existe(id)) {
        QMessageBox::warning(this, "Erreur", "ID inexistant ! Impossible de modifier.");
        return;
    }

    v = Vaccin(id, nom, idType, agent, statut, dateDev, pays, temp, stock, datePer, autorisation);
    if (v.modifier()) {
        QMessageBox::warning(this, "Success", "Modifier Success !");
        UpdateTable(v);
    } else {
        QMessageBox::warning(this, "Erreur", "Modifier échoué !");
    }
}

void MainWindow::on_SupprimerButton_clicked(){
    int id = ui->lineEditID->text().toInt();
    Vaccin v(id, "", 0, "", "", 0, "", 0, 0, 0, "");
    if (v.supprimer(id)) {
        QMessageBox::warning(this, "Success", "Supprimer Success !");
        UpdateTable(v);
    } else {
        QMessageBox::warning(this, "Erreur", "Supprimer échoué !");
    }
}
