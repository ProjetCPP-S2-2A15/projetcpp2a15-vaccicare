#ifndef LOGINDIALOG_H
#define LOGINDIALOG_H

#include <QDialog>
#include <QMainWindow>
#include "connection.h"
#include <QPushButton>
#include <QMessageBox>

namespace Ui {
class LogInDialog;
}

class LogInDialog : public QDialog
{
    Q_OBJECT

public:
    enum Result { Admin,Doctor,Secratary, Canceled };
    typedef struct User{
        Result Droit;
        QString UserName;
        QString Password;
        int Id_User;
    }User;

    explicit LogInDialog(QWidget *parent = nullptr);
    ~LogInDialog();
     User getResult() const { return result_; }
private:
    User result_;
    Ui::LogInDialog *ui;
    QString UserName;
    QString Password;
    int UserId;
    void on_Log_clicked();
    void CloseDialog();
    void ReturnDroit(int DroitId);
    User SetUserData(Result Droit,QString Username, QString Password ,int UserId);
};

#endif // LOGINDIALOG_H
