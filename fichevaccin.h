#ifndef FICHEVACCIN_H
#define FICHEVACCIN_H

#include <QDialog>
#include "vaccin.h"

namespace Ui {
class ficheVaccin;
}

class ficheVaccin : public QDialog
{
    Q_OBJECT

public:
    explicit ficheVaccin(QWidget *parent = nullptr, bool IsModeAjout = false,int ID_Vaccin = 0);
    ~ficheVaccin();
    Vaccin GetResult() const {return result;};
private:
    Ui::ficheVaccin *ui;
    Vaccin result;
    void Annuler();
    void Comfirmer();
    void chargerTypesVaccin();
    bool IsModeAjout;
};

#endif // FICHEVACCIN_H
