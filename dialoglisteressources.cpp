#include "dialoglisteressources.h"
#include "ui_dialoglisteressources.h"
#include "ficheressource.h"
#include "resources.h"
#include "ui_ficheressource.h"
#include <QDialog>
#include <QMessageBox>
#include <QTableView>
#include <QAbstractItemView>


Dialoglisteressources::Dialoglisteressources(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::Dialoglisteressources)
{
    ui->setupUi(this);
    connect(ui->AjtR,&QPushButton::clicked,this,&Dialoglisteressources::on_buttonAjouter_clicked);
    connect(ui->ModR,&QPushButton::clicked,this,&Dialoglisteressources::on_buttonModifier_clicked);
    connect(ui->SupR,&QPushButton::clicked,this,&Dialoglisteressources::on_buttonSupprimer_clicked);
    //connect(ui->ButtonPDF,&QPushButton::clicked,this,&Dialoglistepatient::on_buttonPDF_clicked);
    //connect(ui->ButtonRetourner,&QPushButton::clicked,this,&::Dialoglisteressources::ExitApp);
}

Dialoglisteressources::~Dialoglisteressources()
{
    delete ui;
}

void Dialoglisteressources::on_buttonAjouter_clicked(){
    ficheressource *NewDialog = new ficheressource(this);
    NewDialog->setOperation("Ajouter");
    NewDialog->exec();
    Resources result = NewDialog->GetResult();
    if(result.id_ressource != -1){
        result.ajouter();
       // FillTable(ui->checkBox_tri_DD->isChecked(),ui->checkBox_tri_DPrem->isChecked());
    }
}

void Dialoglisteressources::on_buttonModifier_clicked(){
    QModelIndex index = ui->tableR->currentIndex();
    int row = index.row();
    if (row < 0) {
        QMessageBox::warning(this, "Aucune sélection", "Veuillez sélectionner un vaccin à modifier.");
        return;
    }

    int id_ressource = ui->tableR->model()->index(row, 0).data().toInt();
    ficheressource *NewDialog = new ficheressource(this);
    NewDialog->setOperation("Modifier");
    NewDialog->exec();
    Resources result = NewDialog->GetResult();
    if(result.id_ressource != -1){
        result.modifier();
        //FillTable(ui->checkBox_tri_DD->isChecked(),ui->checkBox_tri_DPrem->isChecked());
    }
}

void Dialoglisteressources::on_buttonSupprimer_clicked()
{
    QModelIndex index = ui->tableR->currentIndex();
    int row = index.row();
    if (row < 0) {
        QMessageBox::warning(this, "Aucune sélection", "Veuillez sélectionner une ressource à supprimer.");
        return;
    }

    int id_ressource = ui->tableR->model()->index(row, 0).data().toInt();

    ficheressource *f = new ficheressource(this);
    f->setOperation("Supprimer");

    QMessageBox::StandardButton reply;
    reply = QMessageBox::question(this, "Confirmation", "Êtes-vous sûr de vouloir supprimer cette ressource ?",
                                  QMessageBox::Yes | QMessageBox::No);

    if (reply == QMessageBox::Yes) {
        int id_ressource = ui->tableR->model()->index(row, 0).data().toInt();
        bool success = Resources::supprimer(id_ressource); // Appel réel à la méthode de suppression
    }


    delete f;
}


void Dialoglisteressources::ExitApp(){
    close();
}
