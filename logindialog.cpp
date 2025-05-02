#include "logindialog.h"
#include "ui_logindialog.h"

LogInDialog::LogInDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::LogInDialog)
{
    ui->setupUi(this);
    connect(ui->buttonBox->button(QDialogButtonBox::Ok),&QPushButton::clicked,this,&LogInDialog::on_Log_clicked);
    connect(ui->buttonBox->button(QDialogButtonBox::Cancel),&QPushButton::clicked,this,&LogInDialog::CloseDialog);
}

LogInDialog::~LogInDialog()
{
    delete ui;
}

void LogInDialog::CloseDialog(){
    ReturnDroit(-1);
}

void LogInDialog::on_Log_clicked() {
    static int attempts = 3;

    QString login = ui->USRIN->toPlainText().trimmed();
    QString password = ui->MDPIN->toPlainText().trimmed();

    if (login.isEmpty() || password.isEmpty()) {
        QMessageBox::warning(this, "Erreur", "Veuillez remplir tous les champs !");
        return;
    }

    if (attempts <= 0) {
        QMessageBox::critical(this, "Erreur", "Trop de tentatives échouées. L'application va se fermer.");
        ReturnDroit(-1);
    }

    QSqlQuery query;
    query.prepare("SELECT COUNT(*),DROIT FROM MEDECIN WHERE LOGIN = :login AND MOT_DE_PASSE = :password GROUP BY DROIT");
    query.bindValue(":login", login);
    query.bindValue(":password", password);

    if (query.exec() && query.next()) {
        int count = query.value(0).toInt();
        if (count > 0) {
            //QMessageBox::information(this, "Succès", "Connecté avec succès !");
            UserName = login;
            Password = password;
            ReturnDroit(query.value(1).toInt());
        } else {
            attempts--;
            QMessageBox::warning(this, "Erreur", QString("Login ou mot de passe incorrect ! %1 tentative(s) restante(s).").arg(attempts));
            ui->MDPIN->clear();
            ui->USRIN->setFocus();
        }
    } else {
        QMessageBox::critical(this, "Erreur SQL", query.lastError().text());
    }
}

void LogInDialog::ReturnDroit(int DroitId){
    switch (DroitId) {
    case 0:
        result_ = SetUserData(Result::Admin,UserName,Password,1);
        break;
    case 1:
        result_ = SetUserData(Result::Doctor,UserName,Password,1);
        break;
    case 2:
        result_ = SetUserData(Result::Secratary,UserName,Password,1);
        break;
    default:
        result_ = SetUserData(Result::Canceled,"","",-1);
        break;
    }
    close();
}

LogInDialog::User LogInDialog::SetUserData(Result Droit,QString Username, QString Password ,int UserId){
    User Temp;
    Temp.Droit = Droit;
    Temp.UserName = Username;
    Temp.Password = Password;
    Temp.Id_User = UserId;
    return Temp;
}
