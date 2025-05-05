#include "dialogformpatient.h"
#include "ui_dialogformpatient.h"
#include <QMessageBox>

Dialogformpatient::Dialogformpatient(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Dialogformpatient)
{
    ui->setupUi(this);

    connect(ui->ButtonConfirmer,&QPushButton::clicked,this,&Dialogformpatient::on_Pat_Button_Confirmer_clicked);
    connect(ui->ButtonRetourner,&QPushButton::clicked,this,&Dialogformpatient::ExitApp);
}

Dialogformpatient::~Dialogformpatient()
{
    delete ui;
}

void Dialogformpatient::setOperation(const QString &op)
{
    operation_actuelle = op;
}

void Dialogformpatient::on_Pat_Button_Confirmer_clicked()
{
    if (operation_actuelle == "Ajouter") {
        // Ajouter le patient
        QMessageBox::information(this, "Info", "Ajout effectué");
        accept(); // Ferme le formulaire
    } else if (operation_actuelle == "Modifier") {
        // Modifier le patient
        QMessageBox::information(this, "Info", "Modification effectuée");
        accept(); // Ferme le formulaire
    } else if (operation_actuelle == "Supprimer") {
        // Supprimer le patient
        QMessageBox::information(this, "Info", "Suppression effectuée");
    } else {
        QMessageBox::warning(this, "Erreur", "Aucune opération définie !");
    }

}
void Dialogformpatient::ExitApp(){
    close();
}

void Dialogformpatient::Valider() {
    result.setID_PATIENT(ui->lineEdit->text().toInt());
    result.setNOM(ui->lineEdit_2->text());
    result.setPRENOM(ui->lineEdit_3->text());
    result.setDATE_NAISSANCE(QDate::fromString(ui->lineEdit_4->text(), "yyyy-MM-dd"));
    result.setSEXE(ui->lineEdit_5->text());
    result.setADRESSE(ui->lineEdit_6->text());
    result.setTELEPHONE(ui->lineEdit_7->text().toInt());
    result.setEMAIL(ui->lineEdit_8->text());
    result.setGROUPE_SANGUIN(ui->lineEdit_9->text());
    result.setSTATUT_VACCINAL(ui->lineEdit_10->text());
    result.setDATE_VACCIN(QDate::fromString(ui->lineEdit_11->text(), "yyyy-MM-dd"));
    result.setID_PROJET_TESTER(ui->lineEdit_12->text().toInt());

    close();
}

void Dialogformpatient::Annuler() {
    result.setID_PATIENT(-1);
    close();
}



