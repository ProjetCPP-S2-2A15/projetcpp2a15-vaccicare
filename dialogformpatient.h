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
    explicit Dialogformpatient(QWidget *parent = nullptr,bool IsModeAjout = false,int ID_Patient = 0);
    void setOperation(const QString &op);  // Setter for the operation
    ~Dialogformpatient();
    Patient GetResult() const { return result;};

private slots:
    void Valider();
    void Annuler();

private:
    Ui::Dialogformpatient *ui;
    void ExitApp();
    Patient result;
    int ID_Patient;
    bool IsModeAjout;
    void setupDesign();
};

#endif // DIALOGFORMPATIENT_H
