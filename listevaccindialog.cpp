#include "listevaccindialog.h"
#include "ui_listevaccindialog.h"
#include "Design.h"

ListeVaccinDialog::ListeVaccinDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ListeVaccinDialog)
{
    ui->setupUi(this);

    connect(ui->PushButtonAjouter, &QPushButton::clicked, this, &ListeVaccinDialog::on_ajouterButton_clicked);
    connect(ui->PushButtonModifier, &QPushButton::clicked, this, &ListeVaccinDialog::on_ModifierButton_clicked);
    connect(ui->PushButtonSupprimer, &QPushButton::clicked, this, &ListeVaccinDialog::on_SupprimerButton_clicked);
    connect(ui->PushButtonRefresh, &QPushButton::clicked, this, &ListeVaccinDialog::onClickRefreshTable);
    connect(ui->pushButton_rechercherNom, &QPushButton::clicked, this, &ListeVaccinDialog::on_PushButton_rechercherNom_clicked);
    connect(ui->pushButton_rechercherPays, &QPushButton::clicked, this, &ListeVaccinDialog::on_PushButton_rechercherPays_clicked);
    connect(ui->ButtonExit, &QPushButton::clicked, this, &ListeVaccinDialog::ExitDialog);
    connect(ui->PushButtonSupprimer_2, &QPushButton::clicked, this, &ListeVaccinDialog::ShowStockHistory);
    connect(ui->AjoutFromPDF, &QPushButton::clicked, this, &ListeVaccinDialog::ImportFromPDF);


    SetupTable();
    setupDesign();

}

ListeVaccinDialog::~ListeVaccinDialog()
{
    delete ui;
}

void ListeVaccinDialog::setupDesign()
{

    StyleLineEdit(ui->lineEditSnom);
    StyleLineEdit(ui->lineEditSpays);

    StyleLabel(ui->label_11);
    StyleLabel(ui->label_12);


    StyleCheckBox(ui->checkBox_tri_DD);
    StyleCheckBox(ui->checkBox_tri_DPrem);

    StyleButtonGreen(ui->PushButtonAjouter);
    StyleButtonBlue(ui->PushButtonModifier);
    StyleButtonDarkblue(ui->PushButtonRefresh);
    StyleButtonRed(ui->PushButtonSupprimer);
    StyleButtonDarkred(ui->ButtonExit);
    StyleButtonDarkblue(ui->PushButtonSupprimer_2);

    StyleButtonGreen(ui->pushButton_rechercherNom);
    StyleButtonGreen(ui->pushButton_rechercherPays);

    StyleTableWidget(ui->tableViewVaccins);


}

void ListeVaccinDialog::on_ajouterButton_clicked(){
    ficheVaccin *NewDialog = new ficheVaccin(this,true,Vaccin::GetLastID() + 1);
    NewDialog->exec();
    Vaccin result = NewDialog->GetResult();
    if(result.id != -1){
        result.ajouter();
        FillTable(ui->checkBox_tri_DD->isChecked(),ui->checkBox_tri_DPrem->isChecked());
    }
}

void ListeVaccinDialog::on_ModifierButton_clicked(){
    int row = ui->tableViewVaccins->currentRow();
    if (row < 0) {
        QMessageBox::warning(this, "Aucune sélection", "Veuillez sélectionner un vaccin à modifier.");
        return;
    }

    int idVaccin = ui->tableViewVaccins->item(row, 0)->text().toInt();
    ficheVaccin *NewDialog = new ficheVaccin(this,false,idVaccin);
    NewDialog->exec();
    Vaccin result = NewDialog->GetResult();
    if(result.id != -1){
        result.modifier();
        FillTable(ui->checkBox_tri_DD->isChecked(),ui->checkBox_tri_DPrem->isChecked());
    }
}

void ListeVaccinDialog::on_SupprimerButton_clicked(){
    int row = ui->tableViewVaccins->currentRow();
    if (row < 0) {
        QMessageBox::warning(this, "Aucune sélection", "Veuillez sélectionner un vaccin à modifier.");
        return;
    }

    int idVaccin = ui->tableViewVaccins->item(row, 0)->text().toInt();

    QMessageBox::StandardButton reply;
    reply = QMessageBox::question(this, "Confirmation", "Êtes-vous sûr de vouloir supprimer cette expérience ?",
                                  QMessageBox::Yes | QMessageBox::No);

    if (reply == QMessageBox::Yes){
        if(Vaccin::supprimer(idVaccin)){
            FillTable(ui->checkBox_tri_DD->isChecked(),ui->checkBox_tri_DPrem->isChecked());
        }
    }
}

void ListeVaccinDialog::onClickRefreshTable(){
    FillTable(ui->checkBox_tri_DD->isChecked(),ui->checkBox_tri_DPrem->isChecked());
}

void ListeVaccinDialog::SetupTable(){
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

void ListeVaccinDialog::FillTable(bool triParDateDev,bool triParDatePrem){
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

        QString typeVaccinName = Vaccin::getTypeVaccinName(ListVaccin[i].idTypeV);
        ui->tableViewVaccins->setItem(i, 2, new QTableWidgetItem(typeVaccinName));

    }
}

void ListeVaccinDialog::on_PushButton_rechercherNom_clicked() {
    QString nomRecherche = ui->lineEditSnom->text().trimmed();

    if (nomRecherche.isEmpty()) {
        QMessageBox::warning(this, "vide", "Veuillez entrer un nom à rechercher.");
        return;
    }

    //qDebug() << "Nom recherché:" << nomRecherche;
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

        QString typeVaccinName = Vaccin::getTypeVaccinName(ListVaccin[i].idTypeV);
        ui->tableViewVaccins->setItem(i, 2, new QTableWidgetItem(typeVaccinName));
    }
}

void ListeVaccinDialog::on_PushButton_rechercherPays_clicked() {
    QString paysRecherche = ui->lineEditSpays->text().trimmed();

    if (paysRecherche.isEmpty()) {
        QMessageBox::warning(this, "vide", "Veuillez entrer un pays à rechercher.");
        return;
    }

    //qDebug() << "Pays recherché:" << paysRecherche;
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

        QString typeVaccinName = Vaccin::getTypeVaccinName(ListVaccin[i].idTypeV);
        ui->tableViewVaccins->setItem(i, 2, new QTableWidgetItem(typeVaccinName));


    }
}

void ListeVaccinDialog::ExitDialog(){
    close();
}

void ListeVaccinDialog::ShowStockHistory(){
    HistoriqueVaccinDialog *NewDialog = new HistoriqueVaccinDialog();
    NewDialog->exec();
}

void ListeVaccinDialog::ImportFromPDF(){
    PDFScanner *Scanner = new PDFScanner();
    QString filePath = Scanner->SelectPDF();
    if(filePath != ""){
        bool Scanned;
        Scanned = Scanner->ScanPDF(filePath);
        if(!Scanned){
            QMessageBox::warning(this, "File Error", "Failed to open scanned_output.txt");
        }
    }

    const QString filepath = QCoreApplication::applicationDirPath() + "/scanned_output.txt";
    QFile file(filepath);
        if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
            QMessageBox::warning(this, "File Error", "Failed to open scanned_output.txt");
            return;
        }

        QTextStream in(&file);
        while (!in.atEnd()) {
            QString line = in.readLine().trimmed();
            if (line.isEmpty()) continue;

            QStringList fields = line.split(';');
            if (fields.size() < 11) {
                QMessageBox::warning(this, "Format Error", "Invalid line: " + line);
                continue;
            }

            // Parse fields
            int id = fields[0].toInt();
            QString nom = fields[1];
            int idTypeV = fields[2].toInt();
            QString agentCible = fields[3];
            QString statutDev = fields[4];
            QDate dateDev = QDate::fromString(fields[5], "yyyyMMdd");
            QString paysOrigine = fields[6];
            int tempConservation = fields[7].toInt();
            int stockDisponible = fields[8].toInt();
            QDate datePeremption = QDate::fromString(fields[9], "yyyyMMdd");
            QString autorisation = fields[10];

            Vaccin vaccin(id, nom, idTypeV, agentCible, statutDev, dateDev,
                          paysOrigine, tempConservation, stockDisponible,
                          datePeremption, autorisation);

            // Check for duplicate ID before inserting
            if (vaccin.idExists(id)) {
                QMessageBox::information(this, "Duplicate ID", "Vaccine ID already exists: " + QString::number(id));
                continue;
            }

            if (!vaccin.ajouter()) {
                QMessageBox::warning(this, "Database Error", "Failed to insert vaccine: " + nom);
            }
        }

        file.close();

        // Refresh the table after import
        FillTable(ui->checkBox_tri_DD->isChecked(), ui->checkBox_tri_DPrem->isChecked());
}
