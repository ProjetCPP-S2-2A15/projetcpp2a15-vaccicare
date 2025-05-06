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
    explicit ficheressource(QWidget *parent = nullptr);
    ~ficheressource();
    void setOperation(const QString &op);
    Resources GetResult() const { return result;};
private slots:
    void on_Pat_Button_Confirmer_clicked();
    void Valider();
    void Annuler();
private:
    Ui::ficheressource *ui;
    QString operation_actuelle;
    Resources result = Resources(0,"",0,0,0,0,0,0);
    void ExitApp();
};

#endif // FICHERESSOURCE_H
