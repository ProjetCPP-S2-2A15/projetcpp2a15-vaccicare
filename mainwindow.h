#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QMessageBox>
#include <QTimer>
#include "connection.h"
#include "logindialog.h"
#include "listeprojetdialog.h"
#include "projectworkflowdialog.h"
#include "calendrierdialog.h"
#include "dialoglistemedecin.h"
#include"fichemedecin.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void showLoginDialog();
private:
    LogInDialog::User CurrUser;
    void SetUpUIForUser(LogInDialog::Result CurrUser);
    Ui::MainWindow *ui;
    Connection Cnx;
    void OuvrirParametre();
    void ExitApp();
    void OuvrirCalendrier();
    void OuvrirResource();
    void OuvrirStockVaccin();
    void OuvrirListeProjet();
    void OuvrirListeMedecin();
    void OuvrirStatistique();
};
#endif // MAINWINDOW_H
