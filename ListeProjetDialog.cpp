#include "listeprojetdialog.h"
#include "ui_listeprojetdialog.h"

ListeProjetDialog::ListeProjetDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ListeProjetDialog)
{
    ui->setupUi(this);

    //Connect Buttons To Correct Event and functions

    connect(ui->ButtonAjouter,&QPushButton::clicked,this,&ListeProjetDialog::AddProject);
    connect(ui->ButtonModifier,&QPushButton::clicked,this,&ListeProjetDialog::ModifyProject);
    connect(ui->ButtonSupprimer,&QPushButton::clicked,this,&ListeProjetDialog::DeleteProject);
    connect(ui->ButtonCloturer,&QPushButton::clicked,this,&ListeProjetDialog::CloseProject);
    connect(ui->ButtonExit,&QPushButton::clicked,this,&ListeProjetDialog::CloseDialog);
//    connect(ui->ButtonExport,&QPushButton::clicked,this,&ListeProjetDialog::ExportProjectList);
    connect(ui->ButtonRechercher,&QPushButton::clicked,this,&ListeProjetDialog::SearchProjects);
//    connect(ui->ButtonStatistics,&QPushButton::clicked,this,&ListeProjetDialog::ShowStatistics);

    //Set Up UI elements with design
    SetupTable();
    SetupDesign();



}

ListeProjetDialog::~ListeProjetDialog()
{
    delete ui;
}

void ListeProjetDialog::SetupDesign(){

    //Design For Buttons
    StyleButton(ui->ButtonAjouter);
    StyleButton(ui->ButtonModifier);
    StyleButton(ui->ButtonSupprimer);
    StyleButton(ui->ButtonCloturer);
    StyleButton(ui->ButtonExport);
    StyleButton(ui->ButtonRechercher);
    StyleButton(ui->ButtonStatistics);
    StyleButton(ui->ButtonExit);

    //Design For Labels
    StyleLabel(ui->label);
    StyleLabel(ui->label_2);

    //Design For Line Edit
    StyleLineEdit(ui->LineEditNomProjet);
    StyleLineEdit(ui->lineEditNom);

    //Design for tableView
    StyleTableView(ui->tableWidget);

    //Design for checkBox
    StyleCheckBox(ui->CheckBoxAfficherProjetCloturer);

}

//Function to Initialize the table Header and fill with Initial Data on load
void ListeProjetDialog::SetupTable(){
    // Set the number of columns (3 in this case)
        ui->tableWidget->setColumnCount(4);

        // Set the column headers
        QStringList headers;
        headers << "Project Name" << "Lead" << "Start Date" << "ID Projet";
        ui->tableWidget->setHorizontalHeaderLabels(headers);

        // Set the column widths
        ui->tableWidget->setColumnWidth(0, 200); // Set "Project Name" column width to 200
        ui->tableWidget->setColumnWidth(1, 150); // Set "Lead" column width to 150
        ui->tableWidget->setColumnWidth(2, 150); // Set "Start Date" column width to 150

        //Hidden Column for Id to use in selecting a row
        ui->tableWidget->setColumnHidden(3, true);

        ui->tableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);  // Make it non-editable

        // Set the table size (for example, you can resize it to fill the available space)
        ui->tableWidget->resize(500, 400);

        //Fill Table data from database
        FillTable(false,"","");
}

//Function to Import table data from database and fill the table rows
void ListeProjetDialog::FillTable(bool AFFICHER_CLOTURER,QString NomChercheur,QString NomProjet){
    std::vector<Projet> ListProjet;
    ListProjet = Projet::ReadProjectListFromDB(AFFICHER_CLOTURER,NomChercheur,NomProjet);

    // Clear the table before adding new data (optional)
    ui->tableWidget->clearContents();
    ui->tableWidget->setRowCount(ListProjet.size());  // Set the number of rows

    // Fill the table
    for (size_t i = 0; i < ListProjet.size(); ++i) {
        ui->tableWidget->setItem(i, 0, new QTableWidgetItem(ListProjet[i].Data.Nom));
        ui->tableWidget->setItem(i, 1, new QTableWidgetItem(Projet::GetChercheurFromID(ListProjet[i].Data.Id_Responsable)));
        ui->tableWidget->setItem(i, 2, new QTableWidgetItem(Date::ConvertIntToDate(ListProjet[i].Data.DateDebut).toString()));
        ui->tableWidget->setItem(i, 3, new QTableWidgetItem(QString::number(ListProjet[i].Data.ID_Projet)));
    }
}

//Function that triggers when the "Ajouter" button is pressed
void ListeProjetDialog::AddProject(){
    int projectID = Projet::GetLastProjectID() + 1;
    // Open FicheProjet window
    FicheProjet *fiche = new FicheProjet(projectID,true, this);
    fiche->exec();  // Show as a modal dialog
    if(fiche->getResult() == FicheProjet::Accepted){
        FillTable(ui->CheckBoxAfficherProjetCloturer->isChecked(),
                  ui->lineEditNom->text(),
                  ui->LineEditNomProjet->text());
    }
}

//Function that triggers when the "Modifier" button is pressed
void ListeProjetDialog::ModifyProject(){
    // Get selected row from table
    int selectedRow = ui->tableWidget->currentRow();

    // Check if a row is selected
    if (selectedRow == -1) {
        QMessageBox::warning(this, "Error", "Please select a row first!", QMessageBox::Ok);
        return;
    }

    QTableWidgetItem* item = ui->tableWidget->item(selectedRow, 3);
    if (!item) {
        QMessageBox::warning(this, "Error", "No data in hidden column!", QMessageBox::Ok);
        return;
    }

    int projectID = item->text().toInt();

    // Open FicheProjet window
    FicheProjet *fiche = new FicheProjet(projectID,false, this);
    fiche->exec();  // Show as a modal dialog

    if(fiche->getResult() == FicheProjet::Accepted){
        FillTable(ui->CheckBoxAfficherProjetCloturer->isChecked(),
                  ui->lineEditNom->text(),
                  ui->LineEditNomProjet->text());
    }
}

//Function that triggers when the "Supprimer" button is pressed
void ListeProjetDialog::DeleteProject(){
    // Get selected row from table
    int selectedRow = ui->tableWidget->currentRow();

    // Check if a row is selected
    if (selectedRow == -1) {
        QMessageBox::warning(this, "Error", "Please select a row first!", QMessageBox::Ok);
        return;
    }

    QTableWidgetItem* item = ui->tableWidget->item(selectedRow, 3);
    if (!item) {
        QMessageBox::warning(this, "Error", "No data in hidden column!", QMessageBox::Ok);
        return;
    }

    int projectID = item->text().toInt();

    QMessageBox::StandardButton reply = QMessageBox::question(this,"Delete",
                "Are you sure you want to delete this project\n you can't undo this action",QMessageBox::Ok|QMessageBox::No);

    if(reply ==QMessageBox::Ok){
        if(Projet::DeleteProjectFromDb(projectID)){
            QMessageBox::warning(this, "Success", "Project Deleted Successfuly", QMessageBox::Ok);
            FillTable(ui->CheckBoxAfficherProjetCloturer->isChecked(),
                      ui->lineEditNom->text(),
                      ui->LineEditNomProjet->text());
        }
    }
}

//Function that triggers when the "Cloturer" button is pressed
void ListeProjetDialog::CloseProject(){
    // Get selected row from table
    int selectedRow = ui->tableWidget->currentRow();

    // Check if a row is selected
    if (selectedRow == -1) {
        QMessageBox::warning(this, "Error", "Please select a row first!", QMessageBox::Ok);
        return;
    }

    QTableWidgetItem* item = ui->tableWidget->item(selectedRow, 3);
    if (!item) {
        QMessageBox::warning(this, "Error", "No data in hidden column!", QMessageBox::Ok);
        return;
    }

    int projectID = item->text().toInt();

    QMessageBox::StandardButton reply = QMessageBox::question(this,"Cloture","Are you sure you want to close this project",QMessageBox::Ok|QMessageBox::No);

    if(reply ==QMessageBox::Ok){
        if(Projet::CloseProject(projectID)){
            QMessageBox::warning(this, "Success", "Project Status changed to Closed", QMessageBox::Ok);
            FillTable(ui->CheckBoxAfficherProjetCloturer->isChecked(),
                      ui->lineEditNom->text(),
                      ui->LineEditNomProjet->text());
        }
    }
}

//Function that triggers when the "Rechercher" button is pressed
void ListeProjetDialog::SearchProjects(){

    FillTable(ui->CheckBoxAfficherProjetCloturer->isChecked(),
              ui->lineEditNom->text(),
              ui->LineEditNomProjet->text());
}

void ListeProjetDialog::CloseDialog(){
    close();
}

//Function that triggers when the "Export" button is pressed
//void ListeProjetDialog::ExportProjectList(){
//    // Open FicheProjet window
//    ExportDialog *NewExportDialog = new ExportDialog(this);
//    NewExportDialog->exec();
//}

//void ListeProjetDialog::ShowStatistics(){

//    // Open FicheProjet window
//    StatsScreen *StatScreen = new StatsScreen(this);
//    StatScreen->exec();
//}
