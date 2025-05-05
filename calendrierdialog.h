#ifndef CALENDRIERDIALOG_H
#define CALENDRIERDIALOG_H

#include <QDialog>

namespace Ui {
class calendrierDialog;
}

class calendrierDialog : public QDialog
{
    Q_OBJECT

public:
    explicit calendrierDialog(QWidget *parent = nullptr);
    ~calendrierDialog();
private slots:
    void highlightDates();
    void on_calendarWidget_pat_selectionChanged();
private:
    Ui::calendrierDialog *ui;
    void ExitApp();
};

#endif // CALENDRIERDIALOG_H
