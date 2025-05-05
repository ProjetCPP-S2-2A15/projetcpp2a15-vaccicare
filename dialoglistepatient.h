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
           //void on_Pat_Button_ExportPDF_clicked();
private:
    Ui::Dialoglistepatient *ui;
    void ExitApp();
};

#endif // DIALOGLISTEPATIENT_H
