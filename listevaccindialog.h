#ifndef LISTEVACCINDIALOG_H
#define LISTEVACCINDIALOG_H

#include <QDialog>
#include "fichevaccin.h"
#include "historiquevaccindialog.h"

namespace Ui {
class ListeVaccinDialog;
}

class ListeVaccinDialog : public QDialog
{
    Q_OBJECT

public:
    explicit ListeVaccinDialog(QWidget *parent = nullptr);
    ~ListeVaccinDialog();

private:
    Ui::ListeVaccinDialog *ui;
    void onClickRefreshTable();
    void FillTable(bool triParDateDev,bool triParDatePrem);
    void SetupTable();
    void on_PushButton_rechercherNom_clicked();
    void on_PushButton_rechercherPays_clicked();
    void on_ajouterButton_clicked();
    void on_ModifierButton_clicked();
    void on_SupprimerButton_clicked();
    void ExitDialog();
    void ShowStockHistory();

};

#endif // LISTEVACCINDIALOG_H
