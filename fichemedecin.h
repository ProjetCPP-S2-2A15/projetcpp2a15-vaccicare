#ifndef FICHEMEDECIN_H
#define FICHEMEDECIN_H

#include <QDialog>
#include "medecin.h"
#include <QDebug>

namespace Ui {
class ficheMedecin;
}

class ficheMedecin : public QDialog
{
    Q_OBJECT

public:
    explicit ficheMedecin(QWidget *parent = nullptr);
    ~ficheMedecin();
    Medecin GetResult() const { return result;};
    //void Valider();
    void Annuler();


private slots:
    void Operation(const QString &op);
    void on_Pat_Button_Confirmer_clicked();
    void on_pushButton_Confirmer_clicked();

private:
    Ui::ficheMedecin *ui;
    Medecin result;
    QString operation_actuelle;
    void ExitApp();
};

#endif // FICHEMEDECIN_H
