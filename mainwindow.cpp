#include "mainwindow.h"
#include "projet.h"
#include "ui_mainwindow.h"
#include "speechrecognizer.h"
#include <QTextEdit>
#include <QTimer>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include <QMessageBox>
#include <QDebug>
#include <QInputDialog>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    //Connect Buttons To Correct Event and functions

    connect(ui->ButtonAjouter,&QPushButton::clicked,this,&MainWindow::AddProject);
    connect(ui->ButtonModifier,&QPushButton::clicked,this,&MainWindow::ModifyProject);
    connect(ui->ButtonSupprimer,&QPushButton::clicked,this,&MainWindow::DeleteProject);
    connect(ui->ButtonCloturer,&QPushButton::clicked,this,&MainWindow::CloseProject);
    connect(ui->ButtonExport,&QPushButton::clicked,this,&MainWindow::ExportProjectList);
    connect(ui->ButtonRechercher,&QPushButton::clicked,this,&MainWindow::SearchProjects);
    connect(ui->ButtonStatistics,&QPushButton::clicked,this,&MainWindow::ShowStatistics);


    //Connect To Database
    bool Connected_To_DB;
    Connected_To_DB = Cnx.createconnect();
    if(!Connected_To_DB){
        // Show a warning message box
        QMessageBox::warning(this,"Database Connection Failed",
                    "Failed to connect to the database. The application will now exit.",
                    QMessageBox::Ok);
    }

    SetupTable();

    // Initialize speech recognition
        m_speechRecognizer = new SpeechRecognizer(this);
        connect(m_speechRecognizer, &SpeechRecognizer::recognized, this, &MainWindow::onSpeechRecognized);
        connect(m_speechRecognizer, &SpeechRecognizer::errorOccurred, this, &MainWindow::onSpeechError);

        // Set up timer for processing events
        m_speechTimer = new QTimer(this);
        connect(m_speechTimer, &QTimer::timeout, m_speechRecognizer, &SpeechRecognizer::processEvents);
        m_speechTimer->start(50); // Check every 50ms

        // Connect buttons
        connect(ui->PushButtonListen, &QPushButton::clicked, this, &MainWindow::onStartListening);
        connect(ui->PushButtonStopListening, &QPushButton::clicked, this, &MainWindow::onStopListening);

}


void MainWindow::onStartListening()
{
    m_speechRecognizer->startListening();
    ui->textEdit->append("Listening started...");
}

void MainWindow::onStopListening()
{
    m_speechRecognizer->stopListening();
    ui->textEdit->append("Listening stopped");
}

void MainWindow::onSpeechRecognized(const QString &text)
{
    ui->textEdit->append("Recognized: " + text);
}

void MainWindow::onSpeechError(const QString &message)
{
    QMessageBox::warning(this, "Speech Recognition Error", message);
    ui->textEdit->append("Error: " + message);
}

MainWindow::~MainWindow()
{
        delete ui;
}

//Function to Initialize the table Header and fill with Initial Data on load
void MainWindow::SetupTable(){
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
void MainWindow::FillTable(bool AFFICHER_CLOTURER,QString NomChercheur,QString NomProjet){
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
void MainWindow::AddProject(){
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
void MainWindow::ModifyProject(){
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
void MainWindow::DeleteProject(){
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
void MainWindow::CloseProject(){
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
void MainWindow::SearchProjects(){
    FillTable(ui->CheckBoxAfficherProjetCloturer->isChecked(),
              ui->lineEditNom->text(),
              ui->LineEditNomProjet->text());
}

//Function that triggers when the "Export" button is pressed
void MainWindow::ExportProjectList(){
    // Open FicheProjet window
    ExportDialog *NewExportDialog = new ExportDialog(this);
    NewExportDialog->exec();
}

void MainWindow::ShowStatistics(){
    // Open FicheProjet window
    StatsScreen *StatScreen = new StatsScreen(this);
    StatScreen->exec();
}
