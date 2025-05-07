#ifndef DIALOGLISTERESSOURCES_H
#define DIALOGLISTERESSOURCES_H

#include <QDialog>
#include "resources.h"
#include <QSqlQueryModel>

namespace Ui {
class Dialoglisteressources;
}

class Dialoglisteressources : public QDialog
{
    Q_OBJECT

public:
    explicit Dialoglisteressources(QWidget *parent = nullptr);
    ~Dialoglisteressources();

private slots:
    void on_buttonAjouter_clicked();
    void on_buttonModifier_clicked();
    void on_buttonSupprimer_clicked();

private:
    Ui::Dialoglisteressources *ui;
    void refreshtableView();
    void ExitApp();
    void setupDesign();
    //Resources afficher;
};

#endif // DIALOGLISTERESSOURCES_H

