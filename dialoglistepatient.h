#ifndef DIALOGLISTEPATIENT_H
#define DIALOGLISTEPATIENT_H

#include <QDialog>

namespace Ui {
class Dialoglistepatient;
}

class Dialoglistepatient : public QDialog
{
    Q_OBJECT

public:
    explicit Dialoglistepatient(QWidget *parent = nullptr);
    ~Dialoglistepatient();
private slots:
    void on_buttonAjouter_clicked();
    void on_buttonModifier_clicked();
    void on_buttonSupprimer_clicked();
    //void FormPatient();
    //void on_Pat_Button_ExportPDF_clicked();
private:
    Ui::Dialoglistepatient *ui;
    void ExitApp();
    void setupDesign();
};

#endif // DIALOGLISTEPATIENT_H
