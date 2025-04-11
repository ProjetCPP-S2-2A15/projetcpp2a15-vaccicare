#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    Vaccin v;

    ui->setupUi(this);
    Cnx.createconnect();
    SetupTable();

    ui->comboBoxAgent->addItems({"Virus","Bactérie","Parasite","Champignon","Autre"});
    ui->comboBoxStatut->addItems({"En recherche","PhaseI","PhaseII","PhaseIII","Approuvé","Suspendu"});
    ui->comboBoxAutorisation->addItems({"FDA","EMA","OMS","ANSM","Autre"});

    connect(ui->pushButtonAjouter, &QPushButton::clicked, this, &MainWindow::on_ajouterButton_clicked);
    connect(ui->PushButtonModifier, &QPushButton::clicked, this, &MainWindow::on_ModifierButton_clicked);
    connect(ui->PushButtonSupprimer, &QPushButton::clicked, this, &MainWindow::on_SupprimerButton_clicked);
    connect(ui->tableViewVaccins, &QTableView::clicked, this, &MainWindow::onRowClicked);
    connect(ui->PushButtonRefresh, &QPushButton::clicked, this, &MainWindow::onClickRefreshTable);


    ui->lineEditTemp->setText("0");

    ui->lineEditStock->setText("0");
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::onClickRefreshTable(){
    FillTable(ui->checkBox_2->isChecked(),ui->checkBox->isChecked());
}
void MainWindow::onRowClicked(const QModelIndex &index) {
    int row = index.row();

    QModelIndex idIndex = ui->tableViewVaccins->model()->index(row, 0); // Assuming ID is in the first column (index 0)
    QModelIndex nomIndex = ui->tableViewVaccins->model()->index(row, 1); // Assuming NOM is in the second column (index 1)
    //QModelIndex typeVaccinIndex = ui->tableViewVaccins->model()->index(row, 2); // ID_TYPE_VACCIN in column 2 (index 2)
    QModelIndex agentCibleIndex = ui->tableViewVaccins->model()->index(row, 2); // AGENT_CIBLE in column 3 (index 3)
    QModelIndex statutDevIndex = ui->tableViewVaccins->model()->index(row, 3); // STATUT_DEVELOPPEMENT in column 4 (index 4)
    QModelIndex dateDevIndex = ui->tableViewVaccins->model()->index(row, 4); // DATE_DEVELOPPEMENT in column 5 (index 5)
    QModelIndex paysOrigineIndex = ui->tableViewVaccins->model()->index(row, 5); // PAYS_ORIGINE in column 6 (index 6)
    QModelIndex tempConservationIndex = ui->tableViewVaccins->model()->index(row, 6); // TEMP_CONSERVATION in column 7 (index 7)
    QModelIndex stockDisponibleIndex = ui->tableViewVaccins->model()->index(row, 7); // STOCK_DISPONIBLE in column 8 (index 8)
    QModelIndex datePeremptionIndex = ui->tableViewVaccins->model()->index(row, 8); // DATE_PEREMPTION in column 9 (index 9)
    QModelIndex autorisationIndex = ui->tableViewVaccins->model()->index(row, 10); // AUTORISATION in column 10 (index 10)

    ui->lineEdit->setText(ui->tableViewVaccins->model()->data(idIndex).toString());
    ui->lineEditNom->setText(ui->tableViewVaccins->model()->data(nomIndex).toString());
    //ui->comboBoxTypeVaccin->setCurrentIndex(ui->tableViewVaccins->model()->data(typeVaccinIndex).toInt());
    ui->comboBoxAgent->setCurrentText(ui->tableViewVaccins->model()->data(agentCibleIndex).toString());
    ui->comboBoxStatut->setCurrentText(ui->tableViewVaccins->model()->data(statutDevIndex).toString());
    ui->dateEditDateDev->setDate(ui->tableViewVaccins->model()->data(dateDevIndex).toDate());
    ui->lineEditPays->setText(ui->tableViewVaccins->model()->data(paysOrigineIndex).toString());
    ui->lineEditTemp->setText(ui->tableViewVaccins->model()->data(tempConservationIndex).toString());
    ui->lineEditStock->setText(ui->tableViewVaccins->model()->data(stockDisponibleIndex).toString());
    ui->dateEditPeremption->setDate(ui->tableViewVaccins->model()->data(datePeremptionIndex).toDate());
    ui->comboBoxAutorisation->setCurrentText(ui->tableViewVaccins->model()->data(autorisationIndex).toString());

}

void MainWindow::on_ajouterButton_clicked(){
    int id = ui->lineEdit->text().toInt();
    QString nom = ui->lineEditNom->text();
    int idType = 0;
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
        FillTable(ui->checkBox_2->isChecked(),ui->checkBox->isChecked());
    } else {
        QMessageBox::warning(this, "Erreur", "Ajout échoué !");
    }
}

void MainWindow::SetupTable(){
    ui->tableViewVaccins->setColumnCount(10);

    QStringList headers;
    headers << "id" << "nom" << "agent" << "statut" << "dateDev"<< "pays"<< "temp"<< "stock" << "datePer"<< "autorisation";
    ui->tableViewVaccins->setHorizontalHeaderLabels(headers);

    //ui->tableViewVaccins->setColumnWidth(0, 200);
    ui->tableViewVaccins->setColumnWidth(1, 80);
    ui->tableViewVaccins->setColumnWidth(2, 80);
    ui->tableViewVaccins->setColumnWidth(3, 80);
    ui->tableViewVaccins->setColumnWidth(4, 80);
    ui->tableViewVaccins->setColumnWidth(5, 80);
    ui->tableViewVaccins->setColumnWidth(6, 80);
    ui->tableViewVaccins->setColumnWidth(7, 80);
    ui->tableViewVaccins->setColumnWidth(8, 80);
    ui->tableViewVaccins->setColumnWidth(9, 80);


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
        ui->tableViewVaccins->setItem(i, 2, new QTableWidgetItem(ListVaccin[i].agentCible));
        ui->tableViewVaccins->setItem(i, 3, new QTableWidgetItem(ListVaccin[i].statutDev));
        ui->tableViewVaccins->setItem(i, 4, new QTableWidgetItem(ListVaccin[i].dateDev.toString("yyyy-MM-dd")));
        ui->tableViewVaccins->setItem(i, 5, new QTableWidgetItem(ListVaccin[i].paysOrigine));
        ui->tableViewVaccins->setItem(i, 6, new QTableWidgetItem(QString::number(ListVaccin[i].tempConservation)));
        ui->tableViewVaccins->setItem(i, 7, new QTableWidgetItem(QString::number(ListVaccin[i].stockDisponible)));
        ui->tableViewVaccins->setItem(i, 8, new QTableWidgetItem(ListVaccin[i].datePeremption.toString("yyyy-MM-dd")));
        ui->tableViewVaccins->setItem(i, 9, new QTableWidgetItem(ListVaccin[i].autorisation));

    }
}
void MainWindow::on_ModifierButton_clicked() {
    int id = ui->lineEdit->text().toInt();
    QString nom = ui->lineEditNom->text();
    int idType = 0;
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

    v = Vaccin(id, nom, idType, agent, statut, dateDev, pays, temp, stock, datePer, autorisation);
    if (v.modifier()) {
        QMessageBox::information(this, "Success", "Modification Success !");
        FillTable(ui->checkBox_2->isChecked(),ui->checkBox->isChecked());
    } else {
        QMessageBox::warning(this, "Erreur", "Modification échouée !");
    }
}

void MainWindow::on_SupprimerButton_clicked() {
    int id = ui->lineEdit->text().toInt();
    Vaccin v(id, "", 0, "", "", QDate(), "", 0, 0, QDate(), "");
    if (v.supprimer(id)) {
        QMessageBox::information(this, "Success", "Suppression Success !");
        FillTable(ui->checkBox_2->isChecked(),ui->checkBox->isChecked());
    } else {
        QMessageBox::warning(this, "Erreur", "Suppression échouée !");
    }
}
