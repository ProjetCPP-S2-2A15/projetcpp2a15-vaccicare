#ifndef DIALOGLISTEMEDECIN_H
#define DIALOGLISTEMEDECIN_H

#include <QDialog>
#include "medecin.h"
#include"fichemedecin.h"

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
    void on_pushButton_2_supprimer_clicked();
    void on_lineEdit_rechercher_textChanged(const QString &text);
    void on_comboBox_tri_currentTextChanged(const QString &text);
private:
    Ui::DialogListeMedecin *ui;
    void refreshtableView();
    void OuvrirAjouter();
};


#endif // DIALOGLISTEMEDECIN_H
