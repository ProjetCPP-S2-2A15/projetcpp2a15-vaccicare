#ifndef FICHERESSOURCE_H
#define FICHERESSOURCE_H

#include "resources.h"
#include "qdatetime.h"
#include <QDialog>

namespace Ui {
class ficheressource;
}

class ficheressource : public QDialog
{
    Q_OBJECT

public:
    explicit ficheressource(QWidget *parent = nullptr,bool IsModeAjout = false, int ID_RESSOURCE = 0);
    ~ficheressource();
    Resources GetResult() const { return result;};
    void ShowDialog();
    bool IsModeAjout;
    int ID_Ressource;
    void Valider();
    void Annuler();
private:
    Ui::ficheressource *ui;
    void SetupDesign();
    Resources result;
};

#endif // FICHERESSOURCE_H
