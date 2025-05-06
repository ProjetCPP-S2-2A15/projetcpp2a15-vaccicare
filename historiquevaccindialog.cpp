#include "historiquevaccindialog.h"
#include "ui_historiquevaccindialog.h"
#include "Design.h"

HistoriqueVaccinDialog::HistoriqueVaccinDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::HistoriqueVaccinDialog)
{
    ui->setupUi(this);
    connect(ui->ButtonExit, &QPushButton::clicked, this, &HistoriqueVaccinDialog::CloseDialog);
    connect(ui->pushButtontxtstock, &QPushButton::clicked, this, &HistoriqueVaccinDialog::on_btnOuvrirDossier_clicked);

    chargerHistoriqueDepuisFichier();

    QTableWidget* tableWidgetHistorique = ui->tableWidgethis;

    tableWidgetHistorique->setColumnCount(3);
    QStringList headers;
    headers << "ID Vaccin" << "Ancien Stock" << "Nouveau Stock";
    tableWidgetHistorique->setHorizontalHeaderLabels(headers);

    tableWidgetHistorique->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);

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

void HistoriqueVaccinDialog::chargerHistoriqueDepuisFichier() {
    QFile file("historique_stock.txt");
    if (!file.exists()) return;

    if (file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QTextStream in(&file);
        while (!in.atEnd()) {
            QString line = in.readLine();
            QStringList parts = line.split(",");
            if (parts.size() == 4) {
                int row = ui->tableWidgethis->rowCount();
                ui->tableWidgethis->insertRow(row);
                for (int i = 0; i < 3; ++i) {
                    ui->tableWidgethis->setItem(row, i, new QTableWidgetItem(parts[i]));
                }
            }
        }
        file.close();
    }
}

QString HistoriqueVaccinDialog::genererHistorique(int id, const VaccinSnapshot &nouveau){
    QString historique;
    if (!copieInitialeVaccins.contains(id))
        return "Aucun historique précédent.";

    VaccinSnapshot ancien = copieInitialeVaccins[id];

    // Comparer uniquement le stock
    if (ancien.stockDisponible != nouveau.stockDisponible)
        historique += "Stock : " + QString::number(ancien.stockDisponible) + " → " + QString::number(nouveau.stockDisponible) + "\n";

    return historique.isEmpty() ? "Aucun changement détecté." : historique;
}


void HistoriqueVaccinDialog::ajouterHistoriqueDansTable(int id, int ancienStock, int nouveauStock) {

    QString filePath = QStandardPaths::writableLocation(QStandardPaths::DesktopLocation) + "/historique_stock.txt";

    QTableWidget* tableWidgetHistorique = ui->tableWidgethis;

    int row = ui->tableWidgethis->rowCount();
    ui->tableWidgethis->insertRow(row);

    tableWidgetHistorique->setItem(row, 0, new QTableWidgetItem(QString::number(id)));  // ID Vaccin
    tableWidgetHistorique->setItem(row, 1, new QTableWidgetItem(QString::number(ancienStock)));  // Old Stock
    tableWidgetHistorique->setItem(row, 2, new QTableWidgetItem(QString::number(nouveauStock)));  // New Stock

    QFile file(filePath);
    if (file.open(QIODevice::Append | QIODevice::Text)) {
        QTextStream out(&file);
        out << "ID: " << id << ", Ancien: " << ancienStock << ", Nouveau: " << nouveauStock << "\n";
        file.close();
    }
}




void HistoriqueVaccinDialog::on_btnOuvrirDossier_clicked()
{
    QString filePath = QStandardPaths::writableLocation(QStandardPaths::DesktopLocation) + "/historique_stock.txt";
    QFileInfo fileInfo(filePath);
    QDesktopServices::openUrl(QUrl::fromLocalFile(fileInfo.absolutePath()));
}

