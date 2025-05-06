#include "calendrierdialog.h"
#include "qdatetime.h"
#include "qsqlerror.h"
#include "qsqlquery.h"
#include "qtextformat.h"
#include "ui_calendrierdialog.h"
#include "Design.h"
#include <QListView>

calendrierDialog::calendrierDialog(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::calendrierDialog)
{
    ui->setupUi(this);
    ui->Calendrier_Info->isReadOnly();
    connect(ui->ButtonExit,&QPushButton::clicked,this,&calendrierDialog::ExitApp);
    StyleButtonRed(ui->ButtonExit);
}

calendrierDialog::~calendrierDialog()
{
    delete ui;
}
void calendrierDialog::highlightDates() {
    QSqlQuery query;
    // Query the PATIENT table for DATE_VACCIN
    if (query.exec("SELECT DATE_VACCIN FROM PATIENT")) {
        while (query.next()) {
            // Retrieve the date as a string and convert to QDateTime
            QString dateVaccinString = query.value(0).toString();
            QDateTime dateVaccin = QDateTime::fromString(dateVaccinString, Qt::ISODate);
            QDate dateVaccinDate = dateVaccin.date();

            // Highlight DATE_VACCIN in red (similar to DATE_FIN in the sponsor function)
            if (dateVaccinDate.isValid()) {
                QTextCharFormat format;
                format.setBackground(QBrush(QColor("#a35941"))); // Red background (using the same red as DATE_FIN in the sponsor function)
                format.setForeground(QBrush(Qt::black));
                format.setFontWeight(QFont::Bold);
                ui->calendarWidget_pat->setDateTextFormat(dateVaccinDate, format);
            }
        }
    } else {
        // Handle query failure
        //qDebug() << "Error executing query:" << query.lastError().text();
    }
}

void calendrierDialog::on_calendarWidget_pat_selectionChanged()
{
    QDate selectedDate = ui->calendarWidget_pat->selectedDate();
    QSqlQuery query;
    bool dateFound = false;

    // Query the PATIENT table for all columns
    if (query.exec("SELECT * FROM PATIENT")) {
        while (query.next()) {
            // Map the columns based on the PATIENT table structure
            QString idPatient = query.value(0).toString(); // ID_PATIENT
            QString nom = query.value(1).toString(); // NOM
            QString prenom = query.value(2).toString(); // PRENOM
            QDate dateNaissance = query.value(3).toDate(); // DATE_NAISSSANCE
            QString sexe = query.value(4).toString(); // SEXE
            QString adresse = query.value(5).toString(); // ADRESSE
            QString telephone = query.value(6).toString(); // TELEPHONE
            QString email = query.value(7).toString(); // EMAIL
            QString groupeSanguin = query.value(8).toString(); // GROUPE_SANGUIN
            QString statutVaccinal = query.value(9).toString(); // STATUT_VACCINAL
            QDate dateVaccin = query.value(10).toDate(); // DATE_VACCIN
            QString idProjetTesteur = query.value(11).toString(); // ID_PROJET_TESTEUR

            // Check if the selected date matches DATE_VACCIN
            if (selectedDate == dateVaccin) {
                ui->Calendrier_Info->setText(
                    "Patient Details:<br>"
                    "<b>ID Patient:</b> " + idPatient + "<br>" +
                    "<b>Name:</b> " + nom + " " + prenom + "<br>" +
                    "<b>Date of Birth:</b> " + dateNaissance.toString("dd/MM/yyyy") + "<br>" +
                    "<b>Sex:</b> " + sexe + "<br>" +
                    "<b>Address:</b> " + adresse + "<br>" +
                    "<b>Phone:</b> " + telephone + "<br>" +
                    "<b>Email:</b> " + email + "<br>" +
                    "<b>Blood Group:</b> " + groupeSanguin + "<br>" +
                    "<b>Vaccination Status:</b> " + statutVaccinal + "<br>" +
                    "<b>Vaccination Date:</b> " + dateVaccin.toString("dd/MM/yyyy") + "<br>" +
                    "<b>Project Tester ID:</b> " + idProjetTesteur + "<br>"
                    );
                dateFound = true;
                break;
            }
        }
        if (!dateFound) {
            ui->Calendrier_Info->setText("No patient found for this date.");
        }
    } else {
        // Handle query failure
        ui->Calendrier_Info->setText("Error retrieving patient data.");
        //qDebug() << "Error executing query:" << query.lastError().text();
    }
}
void calendrierDialog::ExitApp(){
    close();
}
