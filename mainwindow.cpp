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

    if(CurrUser == LogInDialog::Result::Canceled){
        close(); // Close the main window if login was canceled
    }

    SetUpUIForUser(CurrUser);
}

void MainWindow::SetUpUIForUser(LogInDialog::Result CurrUser){
    switch(CurrUser){
        case LogInDialog::Result::Admin :

            break;
        case LogInDialog::Result::Doctor:

            break;
        case LogInDialog::Result::Secratary:

            break;

    }
}

void MainWindow::OuvrirParametre(){}

void MainWindow::OuvrirCalendrier(){}
void MainWindow::OuvrirResource(){}
void MainWindow::OuvrirStockVaccin(){}
void MainWindow::OuvrirListeProjet(){}
void MainWindow::OuvrirStatistique(){}

void MainWindow::ExitApp(){
    close();
}
