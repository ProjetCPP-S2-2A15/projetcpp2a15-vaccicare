#ifndef FICHEMEDECIN_H
#define FICHEMEDECIN_H

#include <QDialog>
#include "medecin.h"
#include <QDebug>
#include "qsqlerror.h"
#include "qsqlquery.h"
#include "ui_fichemedecin.h"
#include <QMessageBox>

namespace Ui {
class ficheMedecin;
}

class ficheMedecin : public QDialog
{
    Q_OBJECT

public:
    explicit ficheMedecin(QWidget *parent = nullptr,bool IsModeAjout = false,int Id_Medecin = -1);
    ~ficheMedecin();
    Medecin GetResult() const { return result;};

private:
    Ui::ficheMedecin *ui;
    Medecin result;
    QString operation_actuelle;
    void ShowDialog();
    bool IsModeAjout;
    int ID_Medecin;
    void Comfirmer();
    void Annuler();

};

#endif // FICHEMEDECIN_H
