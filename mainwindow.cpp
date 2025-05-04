#include "mainwindow.h"
#include "qsqlerror.h"
#include "qsqlquery.h"
#include "ui_mainwindow.h"


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    connect(ui->ButtonParametre,&QPushButton::clicked,this,&MainWindow::OuvrirParametre);
    connect(ui->ButtonExit,&QPushButton::clicked,this,&MainWindow::ExitApp);
    connect(ui->ButtonListeProjet,&QPushButton::clicked,this,&MainWindow::OuvrirListeProjet);
    connect(ui->ButtonCalendrier,&QPushButton::clicked,this,&MainWindow::OuvrirCalendrier);
    connect(ui->ButtonRessource,&QPushButton::clicked,this,&MainWindow::OuvrirResource);
    connect(ui->ButtonStockVaccin,&QPushButton::clicked,this,&MainWindow::OuvrirStockVaccin);
    connect(ui->ButtonStatistique,&QPushButton::clicked,this,&MainWindow::OuvrirStatistique);


    ui->ButtonParametre->setHidden(true);
    ui->ButtonExit->setHidden(true);
    ui->ButtonListeProjet->setHidden(true);
    ui->ButtonCalendrier->setHidden(true);
    ui->ButtonRessource->setHidden(true);
    ui->ButtonStockVaccin->setHidden(true);
    ui->ButtonStatistique->setHidden(true);

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

void MainWindow::showLoginDialog()
{
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
        ui->ButtonParametre->setHidden(false) ;
        ui->ButtonExit->setHidden(false);
        ui->ButtonListeProjet->setHidden(false);
        ui->ButtonCalendrier->setHidden(false);
        ui->ButtonRessource->setHidden(false);
        ui->ButtonStockVaccin->setHidden(false);
        ui->ButtonStatistique->setHidden(false);
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

void MainWindow::OuvrirParametre(){}

void MainWindow::OuvrirCalendrier(){
    calendrierDialog *NewDialog = new calendrierDialog();
    NewDialog->exec();
}

void MainWindow::OuvrirResource(){}
void MainWindow::OuvrirStockVaccin(){}

void MainWindow::OuvrirListeProjet(){
    if(CurrUser.Droit == LogInDialog::Result::Admin){
        ListeProjetDialog *NewDialog = new ListeProjetDialog();
        NewDialog->exec();
    }else{
        ProjectWorkFlowDialog *NewDialog = new ProjectWorkFlowDialog(this,CurrUser.Id_User);
        NewDialog->exec();
    }

}
void MainWindow::OuvrirStatistique(){}

void MainWindow::ExitApp(){
    close();
}
