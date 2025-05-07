#include "dialogformpatient.h"
#include "ui_dialogformpatient.h"
#include "Design.h"
#include <QMessageBox>

Dialogformpatient::Dialogformpatient(QWidget *parent,bool IsModeAjout,int ID_Patient) :
    QDialog(parent),
    ui(new Ui::Dialogformpatient)
{
    ui->setupUi(this);

    connect(ui->ButtonConfirmer,&QPushButton::clicked,this,&Dialogformpatient::Valider);
    connect(ui->ButtonRetourner,&QPushButton::clicked,this,&Dialogformpatient::ExitApp);

    this->IsModeAjout = IsModeAjout;
    this->ID_Patient = ID_Patient;

    if(IsModeAjout){
        ui->Pat_Line_ID->setText(QString::number(ID_Patient));
    }else{
        QSqlQuery query1;
        query1.prepare("SELECT * FROM PATIENT WHERE ID_PATIENT=:id");
        query1.bindValue(":id",ID_Patient);
                if (query1.exec()) {
            while (query1.next()) {
                ui->Pat_Line_ID->setText(query1.value(0).toString());
                ui->Pat_Line_Nom->setText(query1.value(1).toString());
                ui->Pat_Line_Prenom->setText(query1.value(2).toString());
                ui->Pat_Date_naissance->setDate(query1.value(3).toDate());
                ui->Pat_Combo_Sexe->setCurrentText(query1.value(4).toString());
                ui->Pat_Adresse->setText(query1.value(5).toString());
                ui->Pat_Telephone->setText(query1.value(6).toString());
                ui->Pat_Email->setText(query1.value(7).toString());
                ui->Pat_Combo_groupe->setCurrentText(query1.value(8).toString());
                ui->Pat_Combo_Statut->setCurrentText(query1.value(9).toString());
                ui->Pat_Date_Vaccin->setDate(query1.value(10).toDate());
                ui->Pat_Line_Id_Testeur->setText(query1.value(11).toString());
            }
        }
    }

    setupDesign();

}

Dialogformpatient::~Dialogformpatient()
{
    delete ui;
}

void Dialogformpatient::setupDesign() {



}



void Dialogformpatient::ExitApp(){
    close();
}

void Dialogformpatient::Valider() {

    int ID_PATIENT = ui->Pat_Line_ID->text().toInt();
    QString ID_String = ui->Pat_Line_ID->text();
    QString NOM = ui->Pat_Line_Nom->text();
    QString PRENOM = ui->Pat_Line_Prenom->text();
    QDate DATE_NAISSANCE = ui->Pat_Date_naissance->date();
    QString SEXE = ui->Pat_Combo_Sexe->currentText();
    QString ADRESSE = ui->Pat_Adresse->text();
    QString TELEPHONE_String = ui->Pat_Telephone->text();
    int TELEPHONE = TELEPHONE_String.toInt();
    QString EMAIL = ui->Pat_Email->text();
    QString GROUPE_SANGUIN = ui->Pat_Combo_groupe->currentText();
    QString STATUT_VACCINAL = ui->Pat_Combo_Statut->currentText();
    QDate DATE_VACCIN = ui->Pat_Date_Vaccin->date();
    int ID_PROJET_TESTER = ui->Pat_Line_Id_Testeur->text().toInt();
    QString ID_PROJET_String = ui->Pat_Line_Id_Testeur->text();


    Patient P(ID_PATIENT, NOM, PRENOM, DATE_NAISSANCE, SEXE, ADRESSE, TELEPHONE, EMAIL, GROUPE_SANGUIN, STATUT_VACCINAL, DATE_VACCIN, ID_PROJET_TESTER);
    result = P;
    close();
}

void Dialogformpatient::Annuler() {
    result.setID_PATIENT(-1);
    close();
}



