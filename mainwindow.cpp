#include "mainwindow.h"
#include "qsqlerror.h"
#include "qsqlquery.h"
#include "ui_mainwindow.h"
#include <QMessageBox>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    bool Connected;
    Connected = Cnx.CreateConnexion();
    if(!Connected){
       QMessageBox::warning(this, "Erreur", "Cound't Connect to DB");
    }

    LogInDialog *NewLogInDialog = new LogInDialog();
    NewLogInDialog->exec();

    LogInDialog::Result DialogResult;
    DialogResult = NewLogInDialog->getResult();

    switch(DialogResult){
        case LogInDialog::Result::Admin :
            break;
        case LogInDialog::Result::Doctor:
            break;
        case LogInDialog::Result::Secratary:
            break;
        case LogInDialog::Result::Canceled :
            break;

    }

}

MainWindow::~MainWindow()
{
    delete ui;
}

