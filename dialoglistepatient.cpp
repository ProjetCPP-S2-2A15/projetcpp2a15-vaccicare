#include "dialoglistepatient.h"
#include "dialogformpatient.h"
#include "patient.h"
#include "ui_dialoglistepatient.h"
#include <QMessageBox>
#include <QSqlError>
#include <QRegularExpressionValidator>
#include <QSqlQuery>
#include <QTextStream>
#include <QFileDialog>
#include <QTextDocument>
#include <QSystemTrayIcon>
#include <QMessageBox>
#include <QRegularExpression>
#include <QtPrintSupport/QPrinter>
#include <QtPrintSupport/QPrintDialog>
#include <QTextDocument>
#include <QFileDialog>
#include <QFileInfo>
#include <QTextStream>
#include <QtCharts>
#include <QtCharts/QChartView>
#include <QtCharts/QChart>
#include <QtCharts/QPieSeries>
#include <QPainter>
#include <QSqlQuery>
#include <QSqlError>
#include <QtCharts/QPieSlice>
#include <QSsl>
#include <QSslSocket> // Add this for SSL support
#include <QMessageBox>
#include "Design.h"
#include <QScopedPointer>
#include "smtp.h"

Dialoglistepatient::Dialoglistepatient(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::Dialoglistepatient)
{
    ui->setupUi(this);
    connect(ui->ButtonAjouter,&QPushButton::clicked,this,&Dialoglistepatient::on_buttonAjouter_clicked);
    connect(ui->ButtonModifier,&QPushButton::clicked,this,&Dialoglistepatient::on_buttonModifier_clicked);
    connect(ui->ButtonSupprimer,&QPushButton::clicked,this,&Dialoglistepatient::on_buttonSupprimer_clicked);
    connect(ui->ButtonRetourner,&QPushButton::clicked,this,&Dialoglistepatient::ExitApp);

    ui->tableView->setModel(Patient::Afficher());
    setupDesign();
    sendVaccinationReminders();
}

Dialoglistepatient::~Dialoglistepatient()
{
    delete ui;
}

void Dialoglistepatient::setupDesign() {

    StyleButtonGreen(ui->ButtonAjouter);
    StyleButtonRed(ui->ButtonSupprimer);
    StyleButtonBlue(ui->ButtonModifier);
    StyleButtonDarkred(ui->ButtonRetourner);

    StyleTextEdit(ui->recherche);
    StyleComboBox(ui->recherche_par);
    StyleComboBox(ui->tri);

    StyleTableView(ui->tableView);

}


void Dialoglistepatient::on_buttonAjouter_clicked()
{
    Dialogformpatient *dialog = new Dialogformpatient(this,true,Patient::GetLastId() + 1);
    dialog->exec();
    Patient result = dialog->GetResult();
    if(result.getID_PATIENT() != -1){
        result.Ajouter(); // Ajoute directement à la base
        ui->tableView->setModel(Patient::Afficher());
    }
}

void Dialoglistepatient::on_buttonModifier_clicked()
{

    QModelIndexList selectedRows = ui->tableView->selectionModel()->selectedRows();
    if (selectedRows.isEmpty()) {
        QMessageBox::warning(this, "Aucune sélection", "Veuillez sélectionner un vaccin à modifier.");
        return;
    }

    int row = selectedRows.first().row();
    QAbstractItemModel *model = ui->tableView->model();
    int idPatient = model->data(model->index(row, 0)).toInt();  // Assuming ID is in column 0

    Dialogformpatient *dialog = new Dialogformpatient(this,false,idPatient);
    dialog->exec();
    Patient result = dialog->GetResult();
    if(result.getID_PATIENT() != -1){
        result.Modifier(); // Met à jour le patient dans la base
        ui->tableView->setModel(Patient::Afficher());
    }
}

void Dialoglistepatient::on_buttonSupprimer_clicked()
{
    QModelIndexList selectedRows = ui->tableView->selectionModel()->selectedRows();
    if (selectedRows.isEmpty()) {
        QMessageBox::warning(this, "Aucune sélection", "Veuillez sélectionner un vaccin à modifier.");
        return;
    }

    int row = selectedRows.first().row();
    QAbstractItemModel *model = ui->tableView->model();
    int idPatient = model->data(model->index(row, 0)).toInt();  // Assuming ID is in column 0

    QMessageBox::StandardButton reply;
    reply = QMessageBox::question(this, "Confirmation", "Êtes-vous sûr de vouloir supprimer cette expérience ?",
                                  QMessageBox::Yes | QMessageBox::No);

    if (reply == QMessageBox::Yes) {
        Patient::Supprimer(idPatient);
        ui->tableView->setModel(Patient::Afficher());
    }


}

void Dialoglistepatient::ExitApp(){
    close();
}

void Dialoglistepatient::sendVaccinationReminders(){
    QSqlQuery query;
    QDate tomorrowDate = QDate::currentDate().addDays(1);

    if (query.exec("SELECT EMAIL, NOM, PRENOM, DATE_VACCIN FROM PATIENT WHERE DATE_VACCIN IS NOT NULL")) {
        while (query.next()) {
            QString email = query.value(0).toString().trimmed();
            QString nom = query.value(1).toString();
            QString prenom = query.value(2).toString();
            QDate vaccinationDate = QDateTime::fromString(query.value(3).toString(), Qt::ISODate).date();

            if (vaccinationDate != tomorrowDate) continue;

            if (!email.contains("@") || email.isEmpty()) {
                QMessageBox::warning(this, tr("Qt Simple SMTP client"), tr("Invalid email address: %1\n\n").arg(email));
                continue;
            }

            QString subject = "Rappel de Vaccination";
            QString message = QString(
                "Cher(e) %1 %2,\r\n\r\n"
                "Nous vous rappelons que votre vaccination est prevue pour demain, le %3.\r\n"
                "Veuillez vous presenter a l'heure convenue.\r\n\r\n"
                "Cordialement,\r\n"
                "L'equipe medicale"
            ).arg(prenom).arg(nom).arg(vaccinationDate.toString("dd/MM/yyyy"));

            Smtp* smtp = new Smtp("takwabelghith03@gmail.com", "xndxbwaviyzgiphh", "smtp.gmail.com", 465);
            connect(smtp, &Smtp::status, this, [this, email, smtp](const QString &status) {
                QTimer::singleShot(0, this, [this, status]() {
                    if (status == "Message sent") {
                        QMessageBox::information(this, tr("Qt Simple SMTP client"), tr("Message sent!\n\n"));
                    } else {
                        QMessageBox::warning(this, tr("Qt Simple SMTP client"), tr("Failed to send email: %1\n\n").arg(status));
                    }
                });
                smtp->deleteLater();
            });

            smtp->sendMail("takwabelghith03@gmail.com", email, subject, message);
        }
    } else {
        QMessageBox::warning(this, tr("Qt Simple SMTP client"), tr("Error executing query: %1\n\n").arg(query.lastError().text()));
    }
}
