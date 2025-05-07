#include "dialoglisteressources.h"
#include "ui_dialoglisteressources.h"
#include "ficheressource.h"
#include "resources.h"
#include <QMessageBox>
#include <QDebug>
#include "Design.h"
#include <QAbstractItemModel>

Dialoglisteressources::Dialoglisteressources(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::Dialoglisteressources)
{
    ui->setupUi(this);
    connect(ui->AjtR, &QPushButton::clicked, this, &Dialoglisteressources::on_buttonAjouter_clicked);
    connect(ui->ModR, &QPushButton::clicked, this, &Dialoglisteressources::on_buttonModifier_clicked);
    connect(ui->SupR, &QPushButton::clicked, this, &Dialoglisteressources::on_buttonSupprimer_clicked);
    connect(ui->ButtonRetourner, &QPushButton::clicked, this, &Dialoglisteressources::ExitApp);
    ui->tableview->setModel(Resources::Afficher());

    setupDesign();
     ui->tableview->setModel(Resources::Afficher());
}

Dialoglisteressources::~Dialoglisteressources()
{
    delete ui;
}

void Dialoglisteressources::on_buttonAjouter_clicked()
{
    ficheressource *dialog = new ficheressource(this, true, Resources::getLastId() + 1);
    dialog->exec();
    Resources result = dialog->GetResult();
    if(result.id_ressource != -1){
        result.ajouter(); // Ajoute directement à la base
        ui->tableview->setModel(Resources::Afficher());
    }
}

void Dialoglisteressources::on_buttonModifier_clicked()
{
    QModelIndexList selectedRows = ui->tableview->selectionModel()->selectedRows();
    if (selectedRows.isEmpty()) {
        QMessageBox::warning(this, "Aucune sélection", "Veuillez sélectionner une ressource à modifier.");
        return;
    }

    int row = selectedRows.first().row();
    QAbstractItemModel *model = ui->tableview->model();
    int idRessource = model->data(model->index(row, 0)).toInt();  // Supposé que l'ID est en colonne 0

    ficheressource *dialog = new ficheressource(this, false, idRessource);
    dialog->exec();
    Resources result = dialog->GetResult();
    if(result.id_ressource != -1){
        result.modifier(); // Met à jour la ressource dans la base
        ui->tableview->setModel(Resources::Afficher());
    }
}

void Dialoglisteressources::on_buttonSupprimer_clicked()
{
    QModelIndexList selectedRows = ui->tableview->selectionModel()->selectedRows();
    if (selectedRows.isEmpty()) {
        QMessageBox::warning(this, "Aucune sélection", "Veuillez sélectionner une ressource à supprimer.");
        return;
    }

    int row = selectedRows.first().row();
    QAbstractItemModel *model = ui->tableview->model();
    int idRessource = model->data(model->index(row, 0)).toInt();  // Supposé que l'ID est en colonne 0

    QMessageBox::StandardButton reply;
    reply = QMessageBox::question(this, "Confirmation", "Êtes-vous sûr de vouloir supprimer cette ressource ?",
                                  QMessageBox::Yes | QMessageBox::No);

    if (reply == QMessageBox::Yes) {
        if (Resources::supprimer(idRessource)) {
            ui->tableview->setModel(Resources::Afficher());
        }
    }
}


void Dialoglisteressources::ExitApp()
{
    this->close();
}


void Dialoglisteressources::setupDesign(){

    StyleButtonBlue(ui->ModR);
    StyleButtonGreen(ui->AjtR);
    StyleButtonRed(ui->SupR);

    StyleComboBox(ui->comboBox);
    StyleComboBox(ui->comboBox_tri);

    StyleTextEdit(ui->lineEdit_rechercher);
    StyleTableView(ui->tableview);
}
