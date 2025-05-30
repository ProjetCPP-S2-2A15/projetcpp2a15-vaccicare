#include "fichevaccin.h"
#include "ui_fichevaccin.h"
#include "Design.h"


ficheVaccin::ficheVaccin(QWidget *parent ,bool IsModeAjout , int ID_Vaccin) :
    QDialog(parent),
    ui(new Ui::ficheVaccin)
{
    ui->setupUi(this);

    this->IsModeAjout = IsModeAjout;

    connect(ui->ButtonAnnuler,&QPushButton::clicked,this,&ficheVaccin::Annuler);
    connect(ui->ButtonConfirmer,&QPushButton::clicked,this,&ficheVaccin::Comfirmer);

    chargerTypesVaccin();
    setupDesign();

    //chargerHistoriqueDepuisFichier();

    ui->comboBoxAgent->addItems({"Virus","Bactérie","Parasite","Champignon","Autre"});
    ui->comboBoxStatut->addItems({"En recherche","PhaseI","PhaseII","PhaseIII","Approuvé","Suspendu"});
    ui->comboBoxAutorisation->addItems({"FDA","EMA","OMS","ANSM","Autre"});

    if(!IsModeAjout){
        Vaccin v;
        v = Vaccin::GetVaccinFromDb(ID_Vaccin);

        ui->lineEditidv->setText(QString::number(v.id));
        ui->lineEditNom->setText(v.nom);
        ui->comboBoxTypev->setCurrentIndex(v.idTypeV);
        ui->comboBoxAgent->setCurrentText(v.agentCible);
        ui->comboBoxStatut->setCurrentText(v.statutDev);
        ui->dateEditDateDev->setDate(v.dateDev);
        ui->lineEditPays->setText(v.paysOrigine);
        ui->lineEditTemp->setText(QString::number(v.tempConservation));
        ui->lineEditStock->setText(QString::number(v.stockDisponible));
        ui->dateEditPeremption->setDate(v.datePeremption);
        ui->comboBoxAutorisation->setCurrentText(v.autorisation);
    }else{
        ui->lineEditidv->setText(QString::number(ID_Vaccin));
    }
}

ficheVaccin::~ficheVaccin()
{
    delete ui;
}

void ficheVaccin::setupDesign()
{
    StyleLabel(ui->label);
    StyleLabel(ui->label_5);
    StyleLabel(ui->label_9);

    StyleLineEdit(ui->lineEditidv);
    StyleLineEdit(ui->lineEditNom);
    StyleLineEdit(ui->lineEditPays);
    StyleLineEdit(ui->lineEditTemp);
    StyleLineEdit(ui->lineEditStock);

    StyleComboBox(ui->comboBoxAgent);
    StyleComboBox(ui->comboBoxTypev);
    StyleComboBox(ui->comboBoxStatut);
    StyleComboBox(ui->comboBoxAutorisation);

    StyleButtonGreen(ui->ButtonConfirmer);
    StyleButtonRed(ui->ButtonAnnuler);

    StyleDateEdit(ui->dateEditDateDev);
    StyleDateEdit(ui->dateEditPeremption);

}

void ficheVaccin::chargerTypesVaccin() {
    QSqlQuery query("SELECT ID_TYPE_VACCIN, TYPE_VACCIN FROM TYPE_VACCIN ORDER BY ID_TYPE_VACCIN");
    while (query.next()) {
        int id = query.value(0).toInt();
        QString name = query.value(1).toString();
        ui->comboBoxTypev->addItem(name, id); // name displayed, ID stored as data
    }
}

void ficheVaccin::Comfirmer(){
    int id = ui->lineEditidv->text().toInt();
    QString nom = ui->lineEditNom->text();
    QString Temp = ui->comboBoxTypev->currentText();
    int idType = Vaccin::getTypeVaccinId(Temp);
    QString agent = ui->comboBoxAgent->currentText();
    QString statut = ui->comboBoxStatut->currentText();
    QDate dateDev = ui->dateEditDateDev->date();
    QString pays = ui->lineEditPays->text();
    float temp = ui->lineEditTemp->text().toFloat();
    int stock = ui->lineEditStock->text().toInt();
    QDate datePer = ui->dateEditPeremption->date();
    QString autorisation = ui->comboBoxAutorisation->currentText();


    Vaccin v(id, nom, idType, agent, statut, dateDev, pays, temp, stock, datePer, autorisation);
    if(IsModeAjout){
        if (v.idExists(id)) {
            QMessageBox::information(this, "Erreur", "L'ID du vaccin existe déjà."); ;
            return;
        }
    }

    QString erreur;
    if (!v.verifierSaisie(erreur)) {
        QMessageBox::warning(NULL, "Echoué", erreur);
        return;
    }

    result = v;
    close();
}

void ficheVaccin::Annuler(){
    Vaccin v;
    result = v;
    result.id = -1;
    close();
}
