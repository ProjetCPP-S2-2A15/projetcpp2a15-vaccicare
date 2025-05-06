#ifndef DIALOGLISTEMEDECIN_H
#define DIALOGLISTEMEDECIN_H

#include <QDialog>
#include "medecin.h"
#include <QDebug>
#include "fichemedecin.h"

namespace Ui {
class DialogListeMedecin;
}

class DialogListeMedecin : public QDialog
{
    Q_OBJECT

public:
    explicit DialogListeMedecin(QWidget *parent = nullptr);
    ~DialogListeMedecin();

private slots:
    void on_buttonSupprimer_clicked();
    void on_lineEdit_rechercher_textChanged(const QString &text);
    void on_comboBox_tri_currentTextChanged(const QString &text);
    void on_buttonModifier_clicked();
    void ExitDialog();
    void refreshtableView();
    void OuvrirAjouter();
private:
    Ui::DialogListeMedecin *ui;
    void setupDesign();
};


#endif // DIALOGLISTEMEDECIN_H
