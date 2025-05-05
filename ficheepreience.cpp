#include "ficheepreience.h"
#include "ui_ficheepreience.h"

ficheEpreience::ficheEpreience(QWidget *parent,bool IsModeAjout,int ID_Experience) :
    QDialog(parent),
    ui(new Ui::ficheEpreience)
{
    ui->setupUi(this);
    this->IsModeAjout = IsModeAjout;
    this->ID_Experience = ID_Experience;

    connect(ui->ButtonValider,&QPushButton::clicked,this,&ficheEpreience::Valider);
    connect(ui->ButtonAnnuler,&QPushButton::clicked,this,&ficheEpreience::Annuler);

    SetupUI();
}

ficheEpreience::~ficheEpreience()
{
    delete ui;
}

void ficheEpreience::SetupUI(){
    if(IsModeAjout){
        ui->LineEditNom->setReadOnly(false);
        ui->LineEditId->setReadOnly(false);
        ui->LineEditId->setText(QString::number(ID_Experience));
        ui->LineEditNom->setText("");
        ui->TextEditDescription->setText("");
    }else{
        result = Experience::LoadExperienceDescriptionFromDb(ID_Experience);
        ui->LineEditId->setReadOnly(true);
        ui->LineEditId->setText(QString::number(result.ID_Experience));
        ui->LineEditNom->setReadOnly(true);
        ui->LineEditNom->setText(result.Nom);
        ui->TextEditDescription->setText(result.Description);
    }
}

void ficheEpreience::Valider(){
    result.Description = ui->TextEditDescription->toPlainText();
    result.ID_Experience = ui->LineEditId->text().toInt();
    result.Nom = ui->LineEditNom->text();
    close();
}

void ficheEpreience::Annuler(){
    result.ID_Experience = -1;
    close();
}
