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

    void on_Pat_Button_Ajouter_clicked();

    void on_Pat_Button_Modifier_clicked();

    void on_Pat_Button_Supprimer_clicked();

    void on_Pat_Combo_IDs_currentIndexChanged(int index);

    void on_Pat_Button_Tri_Nom_clicked();

    void on_Pat_Button_Tri_Statut_clicked();

    void on_Pat_Button_Tri_Type_clicked();

    void on_Pat_Line_Recherche_textChanged(const QString &arg1);

    void on_Pat_Button_ExportPDF_clicked();

    void on_Pat_Button_Statistique_clicked();

    void on_Pat_Button_Statistique_2_clicked();

    void clearSpLabelStats();


    void highlightDates();

    void on_calendarWidget_pat_selectionChanged();

    void sendVaccinationReminders();

private:
    Ui::MainWindow *ui;
    QSqlDatabase db;
    Patient P;
};

#endif // MAINWINDOW_H
