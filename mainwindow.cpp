#include "mainwindow.h"
#include "qsqlerror.h"
#include "qsqlquery.h"
#include "ui_mainwindow.h"
#include <QDialog>
#include "dialoglistepatient.h"
#include "dialoglistemedecin.h"


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    connect(ui->ButtonParametre,&QPushButton::clicked,this,&MainWindow::OuvrirParametre);
    connect(ui->ButtonExit,&QPushButton::clicked,this,&MainWindow::ExitApp);
    connect(ui->ButtonListeProjet,&QPushButton::clicked,this,&MainWindow::OuvrirListeProjet);
    connect(ui->ButtonListeMedecin,&QPushButton::clicked,this,&MainWindow::OuvrirListeMedecin);
    connect(ui->ButtonCalendrier,&QPushButton::clicked,this,&MainWindow::OuvrirCalendrier);
    connect(ui->ButtonConsultationStock,&QPushButton::clicked,this,&MainWindow::OuvrirChoixStock);
    connect(ui->ButtonStatistique,&QPushButton::clicked,this,&MainWindow::OuvrirStatistique);
    connect(ui->ButtonPatient,&QPushButton::clicked,this,&MainWindow::OuvrirPatient);

    this->setStyleSheet("QMainWindow { background-image: url(:/picture/background vaccicare.png); background-repeat: no-repeat; background-position: center; }");

    ui->ButtonPatient->setHidden(true);
    ui->ButtonParametre->setHidden(true);
    ui->ButtonExit->setHidden(true);
    ui->ButtonListeProjet->setHidden(true);
    ui->ButtonCalendrier->setHidden(true);
    ui->ButtonConsultationStock->setHidden(true);
    ui->ButtonStatistique->setHidden(true);
    ui->ButtonListeMedecin->setHidden(true);

    bool Connected;
    Connected = Cnx.CreateConnexion();
    if(!Connected){
        QMessageBox::warning(this, "Erreur", "Cound't Connect to DB");
    }

    // Defer showing the login dialog until the main window is shown
    QTimer::singleShot(0, this, &MainWindow::showLoginDialog);

}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::OuvrirParametre(){}

void MainWindow::OuvrirStatistique(){}

void MainWindow::OuvrirResource(){
    QMessageBox::warning(this, "Erreur", "Ress");
}

void MainWindow::OuvrirStockVaccin(){
    ListeVaccinDialog *NewDialog = new ListeVaccinDialog();
    NewDialog->exec();
}

void MainWindow::ExitApp(){
    close();
}

void MainWindow::OuvrirChoixStock(){
    DialogChoixStock *NewDialog = new DialogChoixStock();
    NewDialog->exec();
    int Choix = NewDialog->GetResult();

    if(Choix == 1){
        OuvrirResource();
    }else if(Choix == 2){
        OuvrirStockVaccin();
    }
}

void MainWindow::OuvrirListeProjet(){
    if(CurrUser.Droit == LogInDialog::Result::Admin){
        ListeProjetDialog *NewDialog = new ListeProjetDialog();
        NewDialog->exec();
    }else{
        ProjectWorkFlowDialog *NewDialog = new ProjectWorkFlowDialog(this,CurrUser.Id_User);
        NewDialog->exec();
    }

}

void MainWindow::OuvrirListeMedecin(){
    DialogListeMedecin *NewDialog = new DialogListeMedecin();
    NewDialog->exec();
}

void MainWindow::showLoginDialog(){
    LogInDialog *NewLogInDialog = new LogInDialog(this);
    NewLogInDialog->exec();

    CurrUser = NewLogInDialog->getResult();

    if(CurrUser.Droit == LogInDialog::Result::Canceled){
        close(); // Close the main window if login was canceled
    }

    SetUpUIForUser(CurrUser.Droit);
}

void MainWindow::SetUpUIForUser(LogInDialog::Result CurrUser){
    switch(CurrUser){
    case LogInDialog::Result::Admin :
        ui->ButtonPatient->setHidden(false) ;
        ui->ButtonParametre->setHidden(false) ;
        ui->ButtonExit->setHidden(false);
        ui->ButtonListeProjet->setHidden(false);
        ui->ButtonCalendrier->setHidden(false);
        ui->ButtonConsultationStock->setHidden(false);
        ui->ButtonStatistique->setHidden(false);
        ui->ButtonListeMedecin->setHidden(false);
        break;
    case LogInDialog::Result::Doctor:
        ui->ButtonParametre->setHidden(false);
        ui->ButtonExit->setHidden(false);
        ui->ButtonListeProjet->setHidden(false);
        break;
    case LogInDialog::Result::Secratary:

        break;

    }
}

void MainWindow::OuvrirCalendrier(){
    calendrierDialog *NewDialog = new calendrierDialog();
    NewDialog->exec();
}

void MainWindow::OuvrirPatient(){
    if(CurrUser.Droit == LogInDialog::Result::Admin){
    Dialoglistepatient *NewDialog = new Dialoglistepatient();
    NewDialog->exec();
}
}
