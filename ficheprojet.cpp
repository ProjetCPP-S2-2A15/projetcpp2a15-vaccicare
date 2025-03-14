#include "ficheprojet.h"
#include "ui_ficheprojet.h"

FicheProjet::FicheProjet(int projectID,bool IsModeAjouter, QWidget *parent)
    : QDialog(parent), ui(new Ui::FicheProjet), m_projectID(projectID) {
    m_IsModeAjout = IsModeAjouter;
    ui->setupUi(this);
    connect(ui->checkBox,&QCheckBox::toggled,this,&FicheProjet::CheckBoxChecked);

    SetupUI(projectID,IsModeAjouter);
}

FicheProjet::~FicheProjet()
{
    delete ui;
}

void FicheProjet::SetupUI(int ID,bool IsModeAjout){
    //Set ID of Project
    ui->lineEditID->setText(QString::number(ID));

    //Fill Responsable ComboBox
    std::vector<QString> DocotorList;
    QSqlQuery Querry;
    Querry.prepare("Select NOM FROM MEDECIN WHERE ID_CHERCHEUR > 0");
    Querry.exec();

    while(Querry.next()){
        DocotorList.push_back(Querry.value(0).toString());
    }

    // Clear any existing items in the ComboBox
    ui->comboBoxResponsable->clear();

    // Add items from the vector to the ComboBox
    for (const QString& doctorName : DocotorList) {
        ui->comboBoxResponsable->addItem(doctorName);
    }

    //Disable Date_Fin
    ui->dateEditDebut_2->setEnabled(false);

    if(IsModeAjout){
        ui->LineEditNom->setText("");
        ui->LineEditBudget->setText("");
        ui->checkBox->setChecked(false);
        ui->dateEditDebut->setDate(QDate::currentDate());
        ui->dateEditDebut_2->setDate(QDate::currentDate());
    }else{
        Projet P;
        P.Data = P.LoadProjetData(ID);
        ui->LineEditNom->setText(P.Data.Nom);
        ui->LineEditBudget->setText(QString::number(P.Data.Budget));
        ui->dateEditDebut->setDate(Date::ConvertIntToDate(P.Data.DateDebut));
        if(P.Data.DateFin == -1){
            ui->checkBox->setChecked(false);
            ui->dateEditDebut_2->setDate(QDate::currentDate());
        }else{
            ui->checkBox->setChecked(true);
            ui->dateEditDebut_2->setDate(Date::ConvertIntToDate(P.Data.DateFin));
        }
        int index = ui->ComboBoxStatut->findText(Projet::ConvertEnumToString(P.Data.Status));
        if (index != -1) {
            ui->ComboBoxStatut->setCurrentIndex(index);
        }
    }
}

void FicheProjet::CheckBoxChecked(){
    if(!ui->checkBox->isChecked()){
        ui->dateEditDebut_2->setEnabled(false);
    }else{
        ui->dateEditDebut_2->setEnabled(true);
    }
}

void FicheProjet::accept(){
    Projet P;
    //Get Data From the Form
    P.Data.ID_Projet = ui->lineEditID->text().toInt();
    P.Data.Nom = ui->LineEditNom->text();
    P.Data.DateDebut = Date::ConvertDateToInt(ui->dateEditDebut->text());
    if(ui->checkBox->isChecked()){
        P.Data.DateFin = Date::ConvertDateToInt(ui->dateEditDebut_2->text());
    }else{
        P.Data.DateFin = -1;
    }
    P.Data.Status = Projet::ConvertStringToEnum(ui->ComboBoxStatut->currentText());
    P.Data.Budget = ui->LineEditBudget->text().toInt();
    P.Data.Id_Responsable = Projet::GetIdResponsable( ui->comboBoxResponsable->currentText());

    //Validate Data
    bool MissingValues = false;
    QString MandatoryValues = "";

    if(m_projectID != P.Data.ID_Projet){
        QMessageBox::warning(this,"Invalid Action","you can't change the ID of the Project",QMessageBox::Ok);
        ui->lineEditID->setText(QString::number(m_projectID));
        return;
    }

    if(P.Data.Nom.isEmpty()  ){
        MissingValues = true;
        MandatoryValues += "Nom,";
    }

    if(P.Data.Status == Projet::NONE){
        MissingValues = true;
        MandatoryValues += "Statut,";
    }

    if(ui->LineEditBudget->text().isEmpty()){
        MissingValues = true;
        MandatoryValues += "Budget,";
    }

    if(MissingValues){
        QMessageBox::warning(this,"Missing Data","The Following fields are mandatory!\n" + MandatoryValues,QMessageBox::Ok);
        return;
    }

    if(P.Data.DateFin != -1 && P.Data.DateDebut > P.Data.DateFin){
        QMessageBox::warning(this,"Invalid Dates","the End date of the Project can't be before the Start Date",QMessageBox::Ok);
        return;
    }

    //Perform Add or Modify Action if all the checks pass
    if(m_IsModeAjout){
        //Add Project to DB
        if(P.AddProjectToDB()){
            result_ = Accepted;
            QDialog::accept();
        }else{
            QMessageBox::warning(this, "Error", "Unexpected Error while adding a project\n Try retrying or restart the app", QMessageBox::Ok);
        }
    }else{
        //Modify Project in DB
        if(P.ModifyProjectDataInDB()){
            result_ = Accepted;
            QDialog::accept();
        }else{
            QMessageBox::warning(this, "Error", "Unexpected Error while adding a project\n Try retrying or restart the app", QMessageBox::Ok);
        }
    }
}

void FicheProjet::cancel() {
    result_ = Canceled;
    QDialog::reject();
}
