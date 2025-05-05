#ifndef DIALOGFORMPATIENT_H
#define DIALOGFORMPATIENT_H

#include "patient.h"
#include <QDialog>

namespace Ui {
class Dialogformpatient;
}

class Dialogformpatient : public QDialog
{
    Q_OBJECT

public:
    explicit Dialogformpatient(QWidget *parent = nullptr);
    void setOperation(const QString &op);  // Setter for the operation
    ~Dialogformpatient();
    Patient GetResult() const { return result;};

private slots:
    void on_Pat_Button_Confirmer_clicked();
    void Valider();
    void Annuler();

private:
    Ui::Dialogformpatient *ui;
    QString operation_actuelle;
    void ExitApp();
    Patient result;
};

#endif // DIALOGFORMPATIENT_H
