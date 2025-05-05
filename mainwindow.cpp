#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "pdf_scanner.h"
#include "design.h"

#include <QSqlQueryModel>
#include <QSqlError>
#include <QMessageBox>
#include <QSqlRecord>

#include <QPdfWriter>
#include <QTextDocument>
#include <QPainter>
#include <QFileDialog>
#include <QTextTable>
#include <QTextTableFormat>
#include <QTextFrameFormat>
#include <QFont>

#include <StatTvaccin.h>

#include <QStandardPaths>
#include <QFileInfo>
#include <QDesktopServices>
#include <QUrl>



MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    Vaccin v;

    ui->setupUi(this);
    Cnx.createconnect();
    chargerTypesVaccin();
    chargerHistoriqueDepuisFichier();

    SetupTable();

    ui->comboBoxAgent->addItems({"Virus","Bactérie","Parasite","Champignon","Autre"});
    ui->comboBoxStatut->addItems({"En recherche","PhaseI","PhaseII","PhaseIII","Approuvé","Suspendu"});
    ui->comboBoxAutorisation->addItems({"FDA","EMA","OMS","ANSM","Autre"});


    connect(ui->pushButtonAjouter, &QPushButton::clicked, this, &MainWindow::on_ajouterButton_clicked);
    connect(ui->PushButtonModifier, &QPushButton::clicked, this, &MainWindow::on_ModifierButton_clicked);
    connect(ui->PushButtonSupprimer, &QPushButton::clicked, this, &MainWindow::on_SupprimerButton_clicked);
    connect(ui->tableViewVaccins, &QTableView::clicked, this, &MainWindow::onRowClicked);
    connect(ui->PushButtonRefresh, &QPushButton::clicked, this, &MainWindow::onClickRefreshTable);
    connect(ui->pushButton_rechercherNom, &QPushButton::clicked, this, &MainWindow::on_PushButton_rechercherNom_clicked);
    connect(ui->pushButton_rechercherPays, &QPushButton::clicked, this, &MainWindow::on_PushButton_rechercherPays_clicked);
    connect(ui->ButtonExporterPDF, &QPushButton::clicked, this, &MainWindow::ClickExporterPDF);
    connect(ui->pushButtonStats, &QPushButton::clicked, this, &MainWindow::on_btnStats_clicked);
    connect(ui->pushButtontxtstock, &QPushButton::clicked, this, &MainWindow::on_btnOuvrirDossier_clicked);
    connect(ui->pushButtonExtPdf, &QPushButton::clicked, this, &MainWindow::on_btn_importerDepuisTxt_clicked);




    ui->lineEditTemp->setText("0");

    ui->lineEditStock->setText("0");

    QTableWidget* tableWidgetHistorique = ui->tableWidgethis;

    tableWidgetHistorique->setColumnCount(3);
    QStringList headers;
    headers << "ID Vaccin" << "Ancien Stock" << "Nouveau Stock";
    tableWidgetHistorique->setHorizontalHeaderLabels(headers);

    tableWidgetHistorique->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);


}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::onClickRefreshTable(){
    FillTable(ui->checkBox_tri_DD->isChecked(),ui->checkBox_tri_DPrem->isChecked());
}
void MainWindow::onRowClicked(const QModelIndex &index) {
    int row = index.row();

    QModelIndex idIndex = ui->tableViewVaccins->model()->index(row, 0);
    QModelIndex nomIndex = ui->tableViewVaccins->model()->index(row, 1);
    QModelIndex typeVaccinIndex = ui->tableViewVaccins->model()->index(row, 2);
    QModelIndex agentCibleIndex = ui->tableViewVaccins->model()->index(row, 3);
    QModelIndex statutDevIndex = ui->tableViewVaccins->model()->index(row, 4);
    QModelIndex dateDevIndex = ui->tableViewVaccins->model()->index(row, 5);
    QModelIndex paysOrigineIndex = ui->tableViewVaccins->model()->index(row, 6);
    QModelIndex tempConservationIndex = ui->tableViewVaccins->model()->index(row, 7);
    QModelIndex stockDisponibleIndex = ui->tableViewVaccins->model()->index(row, 8);
    QModelIndex datePeremptionIndex = ui->tableViewVaccins->model()->index(row, 9);
    QModelIndex autorisationIndex = ui->tableViewVaccins->model()->index(row, 10);

    ui->lineEditidv->setText(ui->tableViewVaccins->model()->data(idIndex).toString());
    ui->lineEditNom->setText(ui->tableViewVaccins->model()->data(nomIndex).toString());
    ui->comboBoxTypev->setCurrentIndex(getTypeVaccinId(ui->tableViewVaccins->model()->data(typeVaccinIndex).toString()));
    ui->comboBoxAgent->setCurrentText(ui->tableViewVaccins->model()->data(agentCibleIndex).toString());
    ui->comboBoxStatut->setCurrentText(ui->tableViewVaccins->model()->data(statutDevIndex).toString());
    ui->dateEditDateDev->setDate(ui->tableViewVaccins->model()->data(dateDevIndex).toDate());
    ui->lineEditPays->setText(ui->tableViewVaccins->model()->data(paysOrigineIndex).toString());
    ui->lineEditTemp->setText(ui->tableViewVaccins->model()->data(tempConservationIndex).toString());
    ui->lineEditStock->setText(ui->tableViewVaccins->model()->data(stockDisponibleIndex).toString());
    ui->dateEditPeremption->setDate(ui->tableViewVaccins->model()->data(datePeremptionIndex).toDate());
    ui->comboBoxAutorisation->setCurrentText(ui->tableViewVaccins->model()->data(autorisationIndex).toString());

}

void MainWindow::chargerTypesVaccin() {
    QSqlQuery query("SELECT ID_TYPE_VACCIN, TYPE_VACCIN FROM TYPE_VACCIN ORDER BY ID_TYPE_VACCIN");
    while (query.next()) {
        int id = query.value(0).toInt();
        QString name = query.value(1).toString();
        ui->comboBoxTypev->addItem(name, id); // name displayed, ID stored as data
    }
}


QString MainWindow::getTypeVaccinName(int idTypeV) {
    QSqlQuery query;
    query.prepare("SELECT TYPE_VACCIN FROM TYPE_VACCIN WHERE ID_TYPE_VACCIN = :id");
    query.bindValue(":id", idTypeV);

    if (query.exec() && query.next()) {
        return query.value(0).toString();
    }

    return "Inconnu";
}

int MainWindow::getTypeVaccinId(const QString& typeName) {
    QSqlQuery query;
    query.prepare("SELECT ID_TYPE_VACCIN FROM TYPE_VACCIN WHERE TYPE_VACCIN = :typeName");
    query.bindValue(":typeName", typeName);

    if (!query.exec()) {
        qDebug() << "Query failed: " << query.lastError().text();
        return 5;
    }

    if (query.next()) {
        int ID = query.value(0).toInt();
        return ID;
    }

    return -1;  // Si aucun résultat n'est trouvé
}




void MainWindow::on_ajouterButton_clicked(){
    int id = ui->lineEditidv->text().toInt();
    QString nom = ui->lineEditNom->text();
    QString Temp = ui->comboBoxTypev->currentText();
    int idType = getTypeVaccinId(Temp);
    QString agent = ui->comboBoxAgent->currentText();
    QString statut = ui->comboBoxStatut->currentText();
    QDate dateDev = ui->dateEditDateDev->date();
    QString pays = ui->lineEditPays->text();
    float temp = ui->lineEditTemp->text().toFloat();
    int stock = ui->lineEditStock->text().toInt();
    QDate datePer = ui->dateEditPeremption->date();
    QString autorisation = ui->comboBoxAutorisation->currentText();


    Vaccin v(id, nom, idType, agent, statut, dateDev, pays, temp, stock, datePer, autorisation);
    if (v.idExists(id)) {
        QMessageBox::information(this, "Erreur", "L'ID du vaccin existe déjà."); ;
        return;
    }
    if (v.ajouter()) {
        QMessageBox::information(this, "Success", "Ajout Success !");
        FillTable(ui->checkBox_tri_DD->isChecked(),ui->checkBox_tri_DPrem->isChecked());
    } else {
        QMessageBox::warning(this, "Erreur", "Ajout échoué !");
    }
}

void MainWindow::SetupTable(){
    ui->tableViewVaccins->setColumnCount(10);

    QStringList headers;
    headers << "id" << "nom" << "type vaccin" <<"agent" << "statut" << "dateDev"<< "pays"<< "temp"<< "stock" << "datePer"<< "autorisation";
    ui->tableViewVaccins->setHorizontalHeaderLabels(headers);

    ui->tableViewVaccins->setColumnWidth(0, 80);
    ui->tableViewVaccins->setColumnWidth(1, 80);
    ui->tableViewVaccins->setColumnWidth(2, 80);
    ui->tableViewVaccins->setColumnWidth(3, 80);
    ui->tableViewVaccins->setColumnWidth(4, 80);
    ui->tableViewVaccins->setColumnWidth(5, 80);
    ui->tableViewVaccins->setColumnWidth(6, 80);
    ui->tableViewVaccins->setColumnWidth(7, 80);
    ui->tableViewVaccins->setColumnWidth(8, 80);
    ui->tableViewVaccins->setColumnWidth(9, 80);
    ui->tableViewVaccins->setColumnWidth(10, 80);


    //Hidden Column for Id to use in selecting a row
    ui->tableViewVaccins->setColumnHidden(0, true);

    ui->tableViewVaccins->setEditTriggers(QAbstractItemView::NoEditTriggers);

    // Set the table size (for example, you can resize it to fill the available space)
    ui->tableViewVaccins->resize(500, 400);

    //Fill Table data from database
    FillTable(false,false);
}
void MainWindow::FillTable(bool triParDateDev,bool triParDatePrem){
    std::vector<Vaccin> ListVaccin;

    if(triParDateDev){
        ListVaccin = Vaccin::afficherTriParDateDev();
    }else if(triParDatePrem){
        ListVaccin = Vaccin::afficherTriParDatePrem();
    }else{
        ListVaccin = Vaccin::afficher();
    }

    // Clear the table before adding new data (opt)

    ui->tableViewVaccins->clearContents();
    ui->tableViewVaccins->setRowCount(ListVaccin.size());

    // Fill the table
    for (size_t i = 0; i < ListVaccin.size(); ++i) {
        ui->tableViewVaccins->setItem(i, 0, new QTableWidgetItem(QString::number(ListVaccin[i].id)));
        ui->tableViewVaccins->setItem(i, 1, new QTableWidgetItem(ListVaccin[i].nom));
        ui->tableViewVaccins->setItem(i, 3, new QTableWidgetItem(ListVaccin[i].agentCible));
        ui->tableViewVaccins->setItem(i, 4, new QTableWidgetItem(ListVaccin[i].statutDev));
        ui->tableViewVaccins->setItem(i, 5, new QTableWidgetItem(ListVaccin[i].dateDev.toString("yyyy-MM-dd")));
        ui->tableViewVaccins->setItem(i, 6, new QTableWidgetItem(ListVaccin[i].paysOrigine));
        ui->tableViewVaccins->setItem(i, 7, new QTableWidgetItem(QString::number(ListVaccin[i].tempConservation)));
        ui->tableViewVaccins->setItem(i, 8, new QTableWidgetItem(QString::number(ListVaccin[i].stockDisponible)));
        ui->tableViewVaccins->setItem(i, 9, new QTableWidgetItem(ListVaccin[i].datePeremption.toString("yyyy-MM-dd")));
        ui->tableViewVaccins->setItem(i, 10, new QTableWidgetItem(ListVaccin[i].autorisation));

        QString typeVaccinName = getTypeVaccinName(ListVaccin[i].idTypeV);
        ui->tableViewVaccins->setItem(i, 2, new QTableWidgetItem(typeVaccinName));



    }
}

void MainWindow::chargerHistoriqueDepuisFichier() {
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

void MainWindow::on_ModifierButton_clicked() {
    int id = ui->lineEditidv->text().toInt();
    QString nom = ui->lineEditNom->text();
    QString Temp = ui->comboBoxTypev->currentText();
    int idType = getTypeVaccinId(Temp);
    QString agent = ui->comboBoxAgent->currentText();
    QString statut = ui->comboBoxStatut->currentText();
    QDate dateDev = ui->dateEditDateDev->date();
    QString pays = ui->lineEditPays->text();
    float temp = ui->lineEditTemp->text().toFloat();
    int stock = ui->lineEditStock->text().toInt();
    QDate datePer = ui->dateEditPeremption->date();
    QString autorisation = ui->comboBoxAutorisation->currentText();

    Vaccin v;
    if (!v.existe(id)) {
        QMessageBox::warning(this, "Erreur", "ID inexistant ! Impossible de modifier.");
        return;
    }

    // Récupérer l'ancien stock
    int ancienStock = v.getStock(id);

    v = Vaccin(id, nom, idType, agent, statut, dateDev, pays, temp, stock, datePer, autorisation);

    if (v.modifier()) {
        // Ajouter l'historique des modifications dans la table
        ajouterHistoriqueDansTable(id, ancienStock, stock);

        QMessageBox::information(this, "Success", "Modification réussie !");
        FillTable(ui->checkBox_tri_DD->isChecked(), ui->checkBox_tri_DPrem->isChecked());
    } else {
        QMessageBox::warning(this, "Erreur", "Modification échouée !");
    }
}


void MainWindow::on_SupprimerButton_clicked() {
    int id = ui->lineEditidv->text().toInt();
    Vaccin v(id, "", 0, "", "", QDate(), "", 0, 0, QDate(), "");
    if (v.supprimer(id)) {
        QMessageBox::information(this, "Success", "Suppression Success !");
        FillTable(ui->checkBox_tri_DD->isChecked(),ui->checkBox_tri_DPrem->isChecked());
    } else {
        QMessageBox::warning(this, "Erreur", "Suppression échouée !");
    }
}



void MainWindow::on_PushButton_rechercherNom_clicked() {
    QString nomRecherche = ui->lineEditSnom->text().trimmed();

    if (nomRecherche.isEmpty()) {
        QMessageBox::warning(this, "vide", "Veuillez entrer un nom à rechercher.");
        return;
    }

    qDebug() << "Nom recherché:" << nomRecherche;
    std::vector<Vaccin> ListVaccin = Vaccin::rechercherParNom(nomRecherche);

    if (ListVaccin.empty()) {
        QMessageBox::information(this, "Résultat", "Aucun vaccin trouvé avec ce nom.");
        ui->tableViewVaccins->clearContents();
        ui->tableViewVaccins->setRowCount(0);
        return;
    }

    ui->tableViewVaccins->clearContents();
    ui->tableViewVaccins->setRowCount(ListVaccin.size());

    for (size_t i = 0; i < ListVaccin.size(); ++i) {
        ui->tableViewVaccins->setItem(i, 0, new QTableWidgetItem(QString::number(ListVaccin[i].id)));
        ui->tableViewVaccins->setItem(i, 1, new QTableWidgetItem(ListVaccin[i].nom));
        ui->tableViewVaccins->setItem(i, 3, new QTableWidgetItem(ListVaccin[i].agentCible));
        ui->tableViewVaccins->setItem(i, 4, new QTableWidgetItem(ListVaccin[i].statutDev));
        ui->tableViewVaccins->setItem(i, 5, new QTableWidgetItem(ListVaccin[i].dateDev.toString("yyyy-MM-dd")));
        ui->tableViewVaccins->setItem(i, 6, new QTableWidgetItem(ListVaccin[i].paysOrigine));
        ui->tableViewVaccins->setItem(i, 7, new QTableWidgetItem(QString::number(ListVaccin[i].tempConservation)));
        ui->tableViewVaccins->setItem(i, 8, new QTableWidgetItem(QString::number(ListVaccin[i].stockDisponible)));
        ui->tableViewVaccins->setItem(i, 9, new QTableWidgetItem(ListVaccin[i].datePeremption.toString("yyyy-MM-dd")));
        ui->tableViewVaccins->setItem(i, 10, new QTableWidgetItem(ListVaccin[i].autorisation));

        QString typeVaccinName = getTypeVaccinName(ListVaccin[i].idTypeV);
        ui->tableViewVaccins->setItem(i, 2, new QTableWidgetItem(typeVaccinName));
    }
}

void MainWindow::on_PushButton_rechercherPays_clicked() {
    QString paysRecherche = ui->lineEditSpays->text().trimmed();

    if (paysRecherche.isEmpty()) {
        QMessageBox::warning(this, "vide", "Veuillez entrer un pays à rechercher.");
        return;
    }

    qDebug() << "Pays recherché:" << paysRecherche;
    std::vector<Vaccin> ListVaccin = Vaccin::rechercherParPays(paysRecherche);

    if (ListVaccin.empty()) {
        QMessageBox::information(this, "Résultat", "Aucun vaccin trouvé avec ce pays.");
        ui->tableViewVaccins->clearContents();
        ui->tableViewVaccins->setRowCount(0);
        return;
    }

    ui->tableViewVaccins->clearContents();
    ui->tableViewVaccins->setRowCount(ListVaccin.size());

    for (size_t i = 0; i < ListVaccin.size(); ++i) {
        ui->tableViewVaccins->setItem(i, 0, new QTableWidgetItem(QString::number(ListVaccin[i].id)));
        ui->tableViewVaccins->setItem(i, 1, new QTableWidgetItem(ListVaccin[i].nom));
        ui->tableViewVaccins->setItem(i, 3, new QTableWidgetItem(ListVaccin[i].agentCible));
        ui->tableViewVaccins->setItem(i, 4, new QTableWidgetItem(ListVaccin[i].statutDev));
        ui->tableViewVaccins->setItem(i, 5, new QTableWidgetItem(ListVaccin[i].dateDev.toString("yyyy-MM-dd")));
        ui->tableViewVaccins->setItem(i, 6, new QTableWidgetItem(ListVaccin[i].paysOrigine));
        ui->tableViewVaccins->setItem(i, 7, new QTableWidgetItem(QString::number(ListVaccin[i].tempConservation)));
        ui->tableViewVaccins->setItem(i, 8, new QTableWidgetItem(QString::number(ListVaccin[i].stockDisponible)));
        ui->tableViewVaccins->setItem(i, 9, new QTableWidgetItem(ListVaccin[i].datePeremption.toString("yyyy-MM-dd")));
        ui->tableViewVaccins->setItem(i, 10, new QTableWidgetItem(ListVaccin[i].autorisation));

        QString typeVaccinName = getTypeVaccinName(ListVaccin[i].idTypeV);
        ui->tableViewVaccins->setItem(i, 2, new QTableWidgetItem(typeVaccinName));


    }
}

void MainWindow::on_btnStats_clicked()
{
    StatVaccin statsWindow;
    statsWindow.exec();
}


void MainWindow::ClickExporterPDF() {
    QString filePath = QFileDialog::getSaveFileName(this, "Exporter en PDF", "", "PDF Files (*.pdf)");
    if (filePath.isEmpty()) return;

    QPdfWriter writer(filePath);
    writer.setPageSize(QPageSize::A4);
    writer.setResolution(300);
    writer.setPageOrientation(QPageLayout::Landscape);

    QPainter painter(&writer);
    if (!painter.isActive()) {
        QMessageBox::warning(this, "Erreur", "Impossible d'ouvrir le fichier PDF.");
        return;
    }

    QFont font("Arial", 9);
    QFont titleFont("Arial", 13, QFont::Bold);
    painter.setFont(font);

    int rowCount = ui->tableViewVaccins->rowCount();
    int colCount = ui->tableViewVaccins->columnCount();

    QSizeF pageSize = writer.pageLayout().fullRectPixels(writer.resolution()).size();

    int margin = 50;
    int titleHeight = 40;
    int startX = margin;
    int startY = margin + titleHeight;

    int availableWidth = pageSize.width() - 2 * margin;

    int cellWidth = availableWidth / colCount;
    int cellHeight = 220;

    auto drawHeader = [&](int y) {
        painter.setFont(titleFont);
        painter.drawText(startX, y - 20, "Liste des Vaccins");
        painter.setFont(font);

        for (int col = 0; col < colCount; ++col) {
            QString header = ui->tableViewVaccins->horizontalHeaderItem(col)->text();
            int x = startX + col * cellWidth;
            painter.drawRect(x, y, cellWidth, cellHeight);
            painter.drawText(QRectF(x + 5, y, cellWidth - 10, cellHeight),
                             Qt::AlignVCenter | Qt::AlignLeft, header);
        }
    };

    int currentY = startY;
    drawHeader(currentY);
    currentY += cellHeight;

    for (int row = 0; row < rowCount; ++row) {
        // New page if needed
        if (currentY + cellHeight > pageSize.height() - margin) {
            writer.newPage();
            currentY = startY;
            drawHeader(currentY);
            currentY += cellHeight;
        }

        for (int col = 0; col < colCount; ++col) {
            QTableWidgetItem* item = ui->tableViewVaccins->item(row, col);
            QString text = item ? item->text() : "";

            int x = startX + col * cellWidth;
            painter.drawRect(x, currentY, cellWidth, cellHeight);
            painter.drawText(QRectF(x + 5, currentY, cellWidth - 10, cellHeight),
                             Qt::AlignVCenter | Qt::AlignLeft, text);
        }

        currentY += cellHeight;
    }

    painter.end();
    QMessageBox::information(this, "Succès", "Exportation vaccins PDF avec succès !");



}

void MainWindow::chargerVaccins()
{
    ui->tableWidgethis->clearContents();
    ui->tableWidgethis->setRowCount(0);

    QSqlQuery query("SELECT * FROM VACCIN");

    int row = 0;

    while (query.next()) {
        ui->tableWidgethis->insertRow(row);

        int id = query.value("ID_VACCIN").toInt();
        ui->tableWidgethis->setItem(row, 0, new QTableWidgetItem(QString::number(id)));
        ui->tableWidgethis->setItem(row, 1, new QTableWidgetItem(query.value("NOM").toString()));
        ui->tableWidgethis->setItem(row, 2, new QTableWidgetItem(query.value("ID_TYPE_VACCIN").toString()));
        ui->tableWidgethis->setItem(row, 3, new QTableWidgetItem(query.value("AGENT_CIBLE").toString()));
        ui->tableWidgethis->setItem(row, 4, new QTableWidgetItem(query.value("STATUT_DEVELOPPEMENT").toString()));
        ui->tableWidgethis->setItem(row, 5, new QTableWidgetItem(query.value("DATE_DEVELOPPEMENT").toString()));
        ui->tableWidgethis->setItem(row, 6, new QTableWidgetItem(query.value("PAYS_ORIGINE").toString()));
        ui->tableWidgethis->setItem(row, 7, new QTableWidgetItem(query.value("TEMP_CONSERVATION").toString()));
        ui->tableWidgethis->setItem(row, 8, new QTableWidgetItem(query.value("STOCK_DISPONIBLE").toString()));
        ui->tableWidgethis->setItem(row, 9, new QTableWidgetItem(query.value("DATE_PEREMPTION").toString()));
        ui->tableWidgethis->setItem(row, 10, new QTableWidgetItem(query.value("AUTORISATION").toString()));

        VaccinSnapshot snap;
        snap.nom = query.value("NOM").toString();
        snap.idTypeV = query.value("ID_TYPE_VACCIN").toInt();
        snap.agentCible = query.value("AGENT_CIBLE").toString();
        snap.statutDev = query.value("STATUT_DEVELOPPEMENT").toString();
        snap.dateDev = Date::ConvertIntToDate(query.value("DATE_DEVELOPPEMENT").toInt());
        snap.paysOrigine = query.value("PAYS_ORIGINE").toString();
        snap.tempConservation = query.value("TEMP_CONSERVATION").toFloat();
        snap.stockDisponible = query.value("STOCK_DISPONIBLE").toInt();
        snap.datePeremption = Date::ConvertIntToDate(query.value("DATE_PEREMPTION").toInt());
        snap.autorisation = query.value("AUTORISATION").toString();
        copieInitialeVaccins[id] = snap;

        row++;
    }
}

QString MainWindow::genererHistorique(int id, const VaccinSnapshot &nouveau)
{
    QString historique;
    if (!copieInitialeVaccins.contains(id))
        return "Aucun historique précédent.";

    VaccinSnapshot ancien = copieInitialeVaccins[id];

    // Comparer uniquement le stock
    if (ancien.stockDisponible != nouveau.stockDisponible)
        historique += "Stock : " + QString::number(ancien.stockDisponible) + " → " + QString::number(nouveau.stockDisponible) + "\n";

    return historique.isEmpty() ? "Aucun changement détecté." : historique;
}


void MainWindow::ajouterHistoriqueDansTable(int id, int ancienStock, int nouveauStock) {

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




void MainWindow::on_btnOuvrirDossier_clicked()
{
    QString filePath = QStandardPaths::writableLocation(QStandardPaths::DesktopLocation) + "/historique_stock.txt";
    QFileInfo fileInfo(filePath);
    QDesktopServices::openUrl(QUrl::fromLocalFile(fileInfo.absolutePath()));
}

void MainWindow::on_btn_importerDepuisTxt_clicked() {
    PDFScanner scanner;
    QString pdfPath = scanner.SelectPDF();

    if (pdfPath.isEmpty()) return;

    if (!scanner.ScanPDF(pdfPath)) {
        QMessageBox::warning(this, "Erreur", "Impossible de scanner le PDF.");
        return;
    }

    QString txtPath = QCoreApplication::applicationDirPath() + "/scanned_output.txt";
    QFile txtFile(txtPath);
    if (!txtFile.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QMessageBox::warning(this, "Erreur", "Fichier texte introuvable.");
        return;
    }

    QTextStream in(&txtFile);
    QSqlQuery query;

    while (!in.atEnd()) {
        QString line = in.readLine().trimmed();
        if (line.isEmpty()) continue;

        // Format attendu : "1;Pfizer;1;Virus;Approuvé;20201201;USA;-20;10000;20231231;Oui"
        QStringList fields = line.split(';');
        if (fields.size() != 11) continue;

        query.prepare("INSERT INTO VACCIN VALUES (:id, :nom, :type, :agent, :statut, :date_dev, :pays, :temp, :stock, :date_per, :auto)");
        query.bindValue(":id", fields[0].toInt());
        query.bindValue(":nom", fields[1]);
        query.bindValue(":type", fields[2].toInt());
        query.bindValue(":agent", fields[3]);
        query.bindValue(":statut", fields[4]);
        query.bindValue(":date_dev", fields[5].toInt());
        query.bindValue(":pays", fields[6]);
        query.bindValue(":temp", fields[7].toInt());
        query.bindValue(":stock", fields[8].toInt());
        query.bindValue(":date_per", fields[9].toInt());
        query.bindValue(":auto", fields[10]);

        if (!query.exec()) {
            qDebug() << "❌ Erreur insertion : " << query.lastError().text();
        }
    }

    txtFile.close();

    QMessageBox::information(this, "Importation", "Importation terminée avec succès !");
    FillTable(false, false);

}
