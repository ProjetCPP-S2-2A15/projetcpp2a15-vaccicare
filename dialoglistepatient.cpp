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
