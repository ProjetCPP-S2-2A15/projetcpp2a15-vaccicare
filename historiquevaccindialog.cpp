#include "historiquevaccindialog.h"
#include "ui_historiquevaccindialog.h"
#include "Design.h"

HistoriqueVaccinDialog::HistoriqueVaccinDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::HistoriqueVaccinDialog)
{
    ui->setupUi(this);
    connect(ui->ButtonExit, &QPushButton::clicked, this, &HistoriqueVaccinDialog::CloseDialog);

    chargerHistorique();

    setupDesign();
}

HistoriqueVaccinDialog::~HistoriqueVaccinDialog()
{
    delete ui;
}

void HistoriqueVaccinDialog::setupDesign() {

    StyleButtonDarkred(ui->ButtonExit);
    StyleButtonDarkblue(ui->pushButtontxtstock);

    StyleTableWidget(ui->tableWidgethis);

}

void HistoriqueVaccinDialog::CloseDialog(){
    close();
}

void HistoriqueVaccinDialog::chargerHistorique() {
    QTableWidget* tableWidgetHistorique = ui->tableWidgethis;

    tableWidgetHistorique->setRowCount(0);  // Clear existing rows
    tableWidgetHistorique->setColumnCount(4);
    QStringList headers = { "ID Vaccin", "Ancien Stock", "Date Changement", "Nouveau Stock" };
    tableWidgetHistorique->setHorizontalHeaderLabels(headers);
    tableWidgetHistorique->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);

    QSqlQuery query;
    if (!query.exec("SELECT ID_VACCIN, ANCIEN_STOCK, DATE_CHANGEMENT, NOUVEAU_STOCK FROM HISTORIQUE_STOCK_VACCIN ORDER BY DATE_CHANGEMENT DESC")) {
        QMessageBox::critical(this, "Database Error", query.lastError().text());
        return;
    }

    int row = 0;
    while (query.next()) {
        int idVaccin = query.value("ID_VACCIN").toInt();
        int ancienStock = query.value("ANCIEN_STOCK").toInt();
        int nouveauStock = query.value("NOUVEAU_STOCK").toInt();
        int dateInt = query.value("DATE_CHANGEMENT").toInt();

        // Convert YYYYMMDD int to readable date
        QDate dateStr = Date::ConvertIntToDate(dateInt);

        tableWidgetHistorique->insertRow(row);
        tableWidgetHistorique->setItem(row, 0, new QTableWidgetItem(QString::number(idVaccin)));
        tableWidgetHistorique->setItem(row, 1, new QTableWidgetItem(QString::number(ancienStock)));
        tableWidgetHistorique->setItem(row, 2, new QTableWidgetItem(dateStr.toString()));
        tableWidgetHistorique->setItem(row, 3, new QTableWidgetItem(QString::number(nouveauStock)));

        row++;
    }
}
