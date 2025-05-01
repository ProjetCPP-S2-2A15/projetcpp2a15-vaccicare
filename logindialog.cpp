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
        result_ = Result::Admin;
        break;
    case 2:
        result_ = Result::Doctor;
        break;
    case 3:
        result_ = Result::Secratary;
        break;
    default:
        result_ = Result::Canceled;
        break;
    }
    close();
}
