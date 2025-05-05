#include "listevaccindialog.h"
#include "ui_listevaccindialog.h"

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

    SetupTable();
}

ListeVaccinDialog::~ListeVaccinDialog()
{
    delete ui;
}

void ListeVaccinDialog::on_ajouterButton_clicked(){
    ficheVaccin *NewDialog = new ficheVaccin(this,true,Vaccin::GetLastID() + 1);
    NewDialog->exec();
    Vaccin result = NewDialog->GetResult();
    result.ajouter();
}

void ListeVaccinDialog::on_ModifierButton_clicked(){

    ficheVaccin *NewDialog = new ficheVaccin(this,false,1);
    NewDialog->exec();
    Vaccin result = NewDialog->GetResult();
    result.modifier();
}

void ListeVaccinDialog::on_SupprimerButton_clicked(){

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
