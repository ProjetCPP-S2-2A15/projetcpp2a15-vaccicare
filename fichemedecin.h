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

private:
    Ui::ficheMedecin *ui;
    void on_pushButton3_ajouter_clicked();
    void on_pushButton_modifier_clicked();
};

#endif // FICHEMEDECIN_H
