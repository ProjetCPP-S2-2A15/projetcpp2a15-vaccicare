#ifndef LOGINDIALOG_H
#define LOGINDIALOG_H

#include <QDialog>
#include <QMainWindow>
#include "connection.h"
#include <QMessageBox>

namespace Ui {
class LogInDialog;
}

class LogInDialog : public QDialog
{
    Q_OBJECT

public:
    enum Result { Admin,Doctor,Secratary, Canceled };
    explicit LogInDialog(QWidget *parent = nullptr);
    ~LogInDialog();
     Result getResult() const { return result_; }
private:
    Result result_ = Canceled;
    Ui::LogInDialog *ui;
    void on_Log_clicked();
};

#endif // LOGINDIALOG_H
