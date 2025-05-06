#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QMessageBox>
#include <QTimer>
#include "connection.h"
#include "logindialog.h"
#include "ListeProjetDialog.h"
#include "projectworkflowdialog.h"
#include "calendrierdialog.h"
#include "dialogchoixstock.h"
#include "listevaccindialog.h"
#include "dialoglistemedecin.h"
#include "fichemedecin.h"
#include "dialoglistepatient.h"
#include "dialogchoixtypeimportpdf.h"

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
    void OuvrirPatient();
    void OuvrirChoixStock();
    void setupDesign();
    void OuvrirImportPDF();
    void OuvrirChatbot();
protected:
    void resizeEvent(QResizeEvent *event) override;
};

#endif // MAINWINDOW_H
