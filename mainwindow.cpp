#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QSqlError>
#include <QSqlQuery>
#include <QDialog>
#include <QMessageBox>
#include <QPixmap>
#include <QPalette>
#include "dialoglistepatient.h"
#include "dialoglistemedecin.h"
#include "dialoglisteressources.h"
#include "chatbot.h"
#include "Design.h"


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);


    // Connexion des boutons
    connect(ui->ButtonParametre, &QPushButton::clicked, this, &MainWindow::OuvrirParametre);
    connect(ui->ButtonExit, &QPushButton::clicked, this, &MainWindow::ExitApp);
    connect(ui->ButtonListeProjet, &QPushButton::clicked, this, &MainWindow::OuvrirListeProjet);
    connect(ui->ButtonListeMedecin, &QPushButton::clicked, this, &MainWindow::OuvrirListeMedecin);
    connect(ui->ButtonCalendrier, &QPushButton::clicked, this, &MainWindow::OuvrirCalendrier);
    connect(ui->ButtonConsultationStock, &QPushButton::clicked, this, &MainWindow::OuvrirChoixStock);
    connect(ui->ButtonPatient, &QPushButton::clicked, this, &MainWindow::OuvrirPatient);
    connect(ui->ButtonImport, &QPushButton::clicked, this, &MainWindow::OuvrirImportPDF);
    connect(ui->ButtonChat,&QPushButton::clicked,this,&MainWindow::OuvrirChatbot);
    connect(ui->ButtonGenererRessource,&QPushButton::clicked,this,&MainWindow::OuvrirGenererRessource);
    connect(ui->ButtonStat, &QPushButton::clicked, this, &MainWindow::OuvrirChoixStat);


    setupDesign();

    QPixmap bkgnd(":/images/backgroundV.png");
    bkgnd = bkgnd.scaled(this->size(), Qt::IgnoreAspectRatio);
    QPalette palette;
    palette.setBrush(QPalette::Window, bkgnd);
    this->setAutoFillBackground(true);
    this->setPalette(palette);

    // Cacher les boutons
    ui->ButtonPatient->setHidden(true);
    ui->ButtonParametre->setHidden(true);
    ui->ButtonExit->setHidden(true);
    ui->ButtonListeProjet->setHidden(true);
    ui->ButtonCalendrier->setHidden(true);
    ui->ButtonConsultationStock->setHidden(true);
    ui->ButtonListeMedecin->setHidden(true);
    ui->ButtonImport->setHidden(true);
    ui->ButtonChat->setHidden(true);
    ui->ButtonGenererRessource->setHidden(true);
    ui->ButtonStat->setHidden(true);

    // Connexion à la base de données
    bool Connected;
    Connected = Cnx.CreateConnexion();
    if (!Connected) {
        QMessageBox::warning(this, "Erreur", "Couldn't Connect to DB");
    }

    // Afficher la boîte de dialogue de connexion après l'affichage de la fenêtre
    QTimer::singleShot(0, this, &MainWindow::showLoginDialog);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::OuvrirParametre() {}

void MainWindow::OuvrirResource(){
    Dialoglisteressources *NewDialog = new Dialoglisteressources();
    NewDialog->exec();
}

void MainWindow::OuvrirStockVaccin()
{
    ListeVaccinDialog *NewDialog = new ListeVaccinDialog();
    NewDialog->exec();
}

void MainWindow::ExitApp()
{
    close();
}

void MainWindow::OuvrirChatbot(){
    Chatbot *NewDialog = new Chatbot();
    NewDialog->exec();
}

void MainWindow::OuvrirChoixStock()
{
    DialogChoixStock *NewDialog = new DialogChoixStock();
    NewDialog->exec();
    int Choix = NewDialog->GetResult();

    if (Choix == 1) {
        OuvrirResource();
    } else if (Choix == 2) {
        OuvrirStockVaccin();
    }
}

void MainWindow::OuvrirListeProjet()
{
    if (CurrUser.Droit == LogInDialog::Result::Admin) {
        ListeProjetDialog *NewDialog = new ListeProjetDialog();
        NewDialog->exec();
    } else {
        ProjectWorkFlowDialog *NewDialog = new ProjectWorkFlowDialog(this, CurrUser.Id_User);
        NewDialog->exec();
    }
}

void MainWindow::OuvrirListeMedecin()
{
    DialogListeMedecin *NewDialog = new DialogListeMedecin();
    NewDialog->exec();
}

void MainWindow::showLoginDialog()
{
    LogInDialog *NewLogInDialog = new LogInDialog(this);
    int result = NewLogInDialog->exec();

    CurrUser = NewLogInDialog->getResult();

    if (result == QDialog::Rejected) {
            // Dialog was closed with 'X' or cancel
            close();
            return;
        }

    if (CurrUser.Droit == LogInDialog::Result::Canceled) {
        close();
    }

    SetUpUIForUser(CurrUser.Droit);
}

void MainWindow::SetUpUIForUser(LogInDialog::Result CurrUser)
{
    switch (CurrUser) {
    case LogInDialog::Result::Admin:
        ui->ButtonPatient->setHidden(false);
        ui->ButtonParametre->setHidden(false);
        ui->ButtonExit->setHidden(false);
        ui->ButtonListeProjet->setHidden(false);
        ui->ButtonCalendrier->setHidden(false);
        ui->ButtonConsultationStock->setHidden(false);
        ui->ButtonListeMedecin->setHidden(false);
        ui->ButtonImport->setHidden(false);
        ui->ButtonChat->setHidden(false);
        ui->ButtonStat->setHidden(false);
        break;
    case LogInDialog::Result::Doctor:
        ui->ButtonParametre->setHidden(false);
        ui->ButtonExit->setHidden(false);
        ui->ButtonListeProjet->setHidden(false);
        ui->ButtonChat->setHidden(false);
        ui->ButtonGenererRessource->setHidden(false);
        break;
    case LogInDialog::Result::Secratary:
        break;
    }
}

void MainWindow::OuvrirCalendrier()
{
    calendrierDialog *NewDialog = new calendrierDialog();
    NewDialog->exec();
}

void MainWindow::OuvrirPatient()
{
    if (CurrUser.Droit == LogInDialog::Result::Admin) {
        Dialoglistepatient *NewDialog = new Dialoglistepatient();
        NewDialog->exec();
    }
}

void MainWindow::resizeEvent(QResizeEvent *event)
{
    QMainWindow::resizeEvent(event);
    QPixmap background(":/image/background_vaccicare.png");
    if (!background.isNull()) {
        QPalette palette;
        palette.setBrush(QPalette::Window, background.scaled(this->size(), Qt::KeepAspectRatio, Qt::SmoothTransformation));
        this->setPalette(palette);
    }
}

void MainWindow::OuvrirImportPDF(){
    DialogChoixTypeImportPDF *NewDialog = new DialogChoixTypeImportPDF(this);
    NewDialog->exec();
}

void MainWindow::setupDesign(){

    StyleButtonGreen(ui->ButtonParametre);

    StyleButtonDarkblue(ui->ButtonListeMedecin);
    StyleButtonDarkblue(ui->ButtonListeProjet);
    StyleButtonDarkblue(ui->ButtonPatient);
    StyleButtonDarkblue(ui->ButtonConsultationStock);
    StyleButtonDarkblue(ui->ButtonCalendrier);
    StyleButtonDarkblue(ui->ButtonImport);
    StyleButtonDarkblue(ui->ButtonStat);

    StyleButtonGreen(ui->ButtonChat);
    StyleButtonDarkred(ui->ButtonExit);

}

void MainWindow::OuvrirGenererRessource(){
    AddRessource *NewDialog = new AddRessource(this);
    NewDialog->exec();
}

void MainWindow::OuvrirChoixStat(){
    DialogChoixStat *NewDialog = new DialogChoixStat(this);
    NewDialog->exec();
}
