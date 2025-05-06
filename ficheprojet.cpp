#include "ficheprojet.h"
#include "ui_ficheprojet.h"

FicheProjet::FicheProjet(int projectID,bool IsModeAjouter, QWidget *parent)
    : QDialog(parent), ui(new Ui::FicheProjet), m_projectID(projectID) {
    m_IsModeAjout = IsModeAjouter;
    ui->setupUi(this);
    connect(ui->checkBox,&QCheckBox::toggled,this,&FicheProjet::CheckBoxChecked);

    SetupUI(projectID,IsModeAjouter);
    SetupDesign();
}

FicheProjet::~FicheProjet()
{
    delete ui;
}

void FicheProjet::SetupDesign(){
    // Design For Buttons
    StyleButtonGreen(ui->buttonBox->button(QDialogButtonBox::Ok));
    StyleButtonRed(ui->buttonBox->button(QDialogButtonBox::Cancel));

    //Design For Lables
    StyleLabel(ui->labelProjectID_4);
    StyleLabel(ui->labelProjectID);
    StyleLabel(ui->labelProjectID_2);
    StyleLabel(ui->labelProjectID_3);
    StyleLabel(ui->labelProjectID_5);
    StyleLabel(ui->labelProjectID_6);
    StyleLabel(ui->labelProjectID_7);

    //Design For TextEdits
    StyleLineEdit(ui->LineEditBudget);
    StyleLineEdit(ui->lineEditID);
    StyleLineEdit(ui->LineEditNom);

    //Design For ComboBox
    StyleComboBox(ui->ComboBoxStatut);
    StyleComboBox(ui->comboBoxResponsable);

    //Design For CheckBox
    StyleCheckBox(ui->checkBox);

    //Design For DateEdit
    StyleDateEdit(ui->dateEditDebut);
    StyleDateEdit(ui->dateEditDebut_2);
    //Design For TableView
    StyleTableView(ui->tableWidget);
}

void FicheProjet::SetupTableTeam(){
    ui->tableWidget->setColumnCount(3);

    // Set the column headers
    QStringList headers;
    headers << "Doctor Name" << "" << "ID Chercheur";
    ui->tableWidget->setHorizontalHeaderLabels(headers);

    // Set the column widths
    ui->tableWidget->setColumnWidth(0, 200); // Set "Project Name" column width to 200
    ui->tableWidget->setColumnWidth(1, 60); // Set "Lead" column width to 150

    //Hidden Column for Id to use in selecting a row
    ui->tableWidget->setColumnHidden(2, true);

    ui->tableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);  // Make it non-editable

    // Set the table size (for example, you can resize it to fill the available space)
    ui->tableWidget->resize(260, 370);

    //Fill Table data from database
    //FillTable(false,"");
}

void FicheProjet::SetupUI(int ID,bool IsModeAjout){
    //Set ID of Project
    ui->lineEditID->setText(QString::number(ID));

    //Fill Responsable ComboBox
    std::vector<DoctorData> DocotorList;
    QSqlQuery Querry;
    Querry.prepare("Select NOM,ID_CHERCHEUR FROM MEDECIN WHERE ID_CHERCHEUR > 0");
    Querry.exec();

    while(Querry.next()){
        DoctorData Temp;
        Temp.Name = Querry.value(0).toString();
        Temp.ID = Querry.value(1).toInt();
        DocotorList.push_back(Temp);
    }

    std::vector<int> DoctorInTeam;
    QSqlQuery QuerryTeam;
    QuerryTeam.prepare("Select ID_CHERCHEUR FROM EQUIPE_PROJET WHERE ID_PROJET = :id_projet");
    QuerryTeam.bindValue(":id_projet",ID);
    QuerryTeam.exec();

    while(QuerryTeam.next()){
        DoctorInTeam.push_back(QuerryTeam.value(0).toInt());
    }
    //SetUp Table for Team
    SetupTableTeam();

    // Clear any existing items in the ComboBox
    ui->comboBoxResponsable->clear();

    //Disable Date_Fin
    ui->dateEditDebut_2->setEnabled(false);

    // Clear the table before adding new data (optional)
    ui->tableWidget->clearContents();
    ui->tableWidget->setRowCount(DocotorList.size());  // Set the number of rows

    if(IsModeAjout){
        ui->LineEditNom->setText("");
        ui->LineEditBudget->setText("");
        ui->checkBox->setChecked(false);
        ui->dateEditDebut->setDate(QDate::currentDate());
        ui->dateEditDebut_2->setDate(QDate::currentDate());

        for (size_t i = 0; i < DocotorList.size(); ++i) {
            ui->comboBoxResponsable->addItem(DocotorList[i].Name);
            ui->tableWidget->setItem(i, 2, new QTableWidgetItem(QString::number(DocotorList[i].ID)));
            QTableWidgetItem *checkboxItem = new QTableWidgetItem();
            checkboxItem->setFlags(Qt::ItemIsUserCheckable | Qt::ItemIsEnabled);
            checkboxItem->setCheckState(Qt::Unchecked);
            ui->tableWidget->setItem(i, 1, checkboxItem);
            ui->tableWidget->setItem(i, 0, new QTableWidgetItem(DocotorList[i].Name));
        }
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

        for (size_t i = 0; i < DocotorList.size(); ++i) {
            ui->comboBoxResponsable->addItem(DocotorList[i].Name);
            ui->tableWidget->setItem(i, 2, new QTableWidgetItem(QString::number(DocotorList[i].ID)));
            ui->tableWidget->setItem(i, 0, new QTableWidgetItem(DocotorList[i].Name));
            QTableWidgetItem *checkboxItem = new QTableWidgetItem();
            checkboxItem->setFlags(Qt::ItemIsUserCheckable | Qt::ItemIsEnabled);
            if(CheckDocrotInTeam(DocotorList[i].ID,DoctorInTeam)){
                checkboxItem->setCheckState(Qt::Checked);
            }else{
                checkboxItem->setCheckState(Qt::Unchecked);
            }
            ui->tableWidget->setItem(i, 1, checkboxItem);
        }
    }
}

bool FicheProjet::CheckDocrotInTeam(int Id,std::vector<int> DoctorList){
    if(DoctorList.size() == 0){
        return false;
    }
    for(size_t index = 0;index < DoctorList.size(); index ++){
        if(Id == DoctorList[index]){
            return true;
        }
    }
    return false;
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

    // Collect selected team members from table
    std::vector<int> teamIDs;
    int rowCount = ui->tableWidget->rowCount();

    for (int row = 0; row < rowCount; ++row) {
        QTableWidgetItem *checkboxItem = ui->tableWidget->item(row, 1);
        if (checkboxItem && checkboxItem->checkState() == Qt::Checked) {
            QTableWidgetItem *idItem = ui->tableWidget->item(row, 2); // ID is in column 2
            if (idItem) {
                teamIDs.push_back(idItem->text().toInt());
            }
        }
    }
    P.Data.Team = teamIDs;

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

    if(std::find(P.Data.Team.begin(), P.Data.Team.end(), P.Data.Id_Responsable) == P.Data.Team.end()){
       QMessageBox::warning(this,"Missing Lead Doctor","The Lead Doctor Must Be Included In the Team",QMessageBox::Ok);
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

//
