#ifndef LISTEVACCINDIALOG_H
#define LISTEVACCINDIALOG_H

#include <QDialog>
#include "vaccin.h"

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

};

#endif // LISTEVACCINDIALOG_H
