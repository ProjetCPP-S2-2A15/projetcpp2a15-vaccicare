#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QSqlDatabase>
#include <QSqlQueryModel>
#include <QPushButton>
#include <QRegularExpressionValidator>
#include "patient.h"
#include "connection.h"

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_Ajouter_clicked();    // Ajouter un patient
    void on_Modifier_clicked();   // Modifier un patient
    void on_Supprimer_clicked();  // Supprimer un patient
    void on_Rechercher_clicked(); // Rechercher un patient

private:
    Ui::MainWindow *ui;
    QSqlDatabase db;
    Patient patient;

    void connectToDatabase();
    void updateTable();

    // Fonctions de validation
    bool validateID();
    bool validateName(const QString& name);
    bool validateDate(const QString& date);
    bool validateSexe();
    bool validateAdresse();
    bool validatePhoneOrCIN(const QString& input);
    bool validateGroupeSanguin();
    bool validateStatutVaccinal();
    bool validateDateVaccin();
};

#endif // MAINWINDOW_H
