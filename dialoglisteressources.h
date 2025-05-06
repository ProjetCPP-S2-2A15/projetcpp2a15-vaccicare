#ifndef DIALOGLISTERESSOURCES_H
#define DIALOGLISTERESSOURCES_H

#include "resources.h"
#include <QDialog>

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
    void ExitApp();
};

#endif // DIALOGLISTERESSOURCES_H
