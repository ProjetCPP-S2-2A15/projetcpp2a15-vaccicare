#include "mainwindow.h"
#include "qsqlerror.h"
#include "qsqlquery.h"
#include "ui_mainwindow.h"
#include <QMessageBox>
#include <QTimer>

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

    switch(CurrUser){
        case LogInDialog::Result::Admin :
            QMessageBox::information(this, "Erreur", "Admin");
            break;
        case LogInDialog::Result::Doctor:
            QMessageBox::information(this, "Erreur", "Doctor");
            break;
        case LogInDialog::Result::Secratary:
        QMessageBox::information(this, "Erreur", "Secratary");
            break;
        case LogInDialog::Result::Canceled :
            QMessageBox::information(this, "Erreur", "Canceled");
            close(); // Close the main window if login was canceled
            break;
    }
}
