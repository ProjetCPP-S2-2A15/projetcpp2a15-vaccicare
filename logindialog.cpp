#include "logindialog.h"
#include "ui_logindialog.h"

LogInDialog::LogInDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::LogInDialog)
{
    ui->setupUi(this);
}

LogInDialog::~LogInDialog()
{
    delete ui;
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
        close();
        return;
    }

    QSqlQuery query;
    query.prepare("SELECT COUNT(*) FROM MEDECIN WHERE LOGIN = :login AND MOT_DE_PASSE = :password");
    query.bindValue(":login", login);
    query.bindValue(":password", password);

    if (query.exec() && query.next()) {
        int count = query.value(0).toInt();
        if (count > 0) {
            QMessageBox::information(this, "Succès", "Connecté avec succès !");
            // Tu peux ajouter ici l'ouverture de la fenêtre principale
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
