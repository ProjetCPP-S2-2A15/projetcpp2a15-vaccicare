#include "ficheressource.h"
#include "ui_ficheressource.h"
#include "resources.h"
#include <QDialog>
#include <QMessageBox>

ficheressource::ficheressource(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::ficheressource)
{

    ui->setupUi(this);
    connect(ui->ButtonC,&QPushButton::clicked,this,&ficheressource::Valider);
    connect(ui->ButtonR,&QPushButton::clicked,this,&ficheressource::ExitApp);
}

ficheressource::~ficheressource()
{
    delete ui;
}

void ficheressource::setOperation(const QString &op)
{
    operation_actuelle = op;
}

void ficheressource::on_Pat_Button_Confirmer_clicked()
{
    qDebug() << "Operation actuelle:" << operation_actuelle; // Debug line

    if (operation_actuelle == "Ajouter") {
        // Ajouter le patient
        QMessageBox::information(this, "Info", "Ajout effectué");
        accept(); // Ferme le formulaire
    } else if (operation_actuelle == "Modifier") {
        // Modifier le patient
        QMessageBox::information(this, "Info", "Modification effectuée");
        accept(); // Ferme le formulaire
    } else if (operation_actuelle == "Supprimer") {
        // Supprimer le patient
        QMessageBox::information(this, "Info", "Suppression effectuée");
        accept(); // Ferme le formulaire
    } else {
        QMessageBox::warning(this, "Erreur", "Aucune opération définie !");
    }
}

void ficheressource::Valider() {
    // Retrieve values from UI
    result.id_ressource = ui->lineEdit->text().toInt();
    result.nom = ui->lineEdit_2->text();
    result.id_type_ressource = ui->lineEdit_3->text().toInt();
    result.quantite = ui->lineEdit_4->text().toInt();
    result.disponibilite = ui->lineEdit_5->text().toInt();
    result.date_acquisition = ui->lineEdit_6->text().toInt();
    result.id_fournisseur = ui->lineEdit_7->text().toInt();
    result.cout_acquisition = ui->lineEdit_8->text().toInt();

    QSqlQuery query;

    qDebug() << "Operation actuelle:" << operation_actuelle; // Debug line

    if (operation_actuelle == "Ajouter") {
        // Insertion: If the operation is "Add"
        query.prepare("INSERT INTO ressources (id_ressource, nom, id_type_ressource, quantite, disponibilite, date_acquisition, id_fournisseur, cout_acquisition)"
                      "VALUES (:id_ressource, :nom, :id_type_ressource, :quantite, :disponibilite, :date_acquisition, :id_fournisseur, :cout_acquisition)");

        // Bind values to the query
        query.bindValue(":id_ressource", result.id_ressource);
        query.bindValue(":nom", result.nom);
        query.bindValue(":id_type_ressource", result.id_type_ressource);
        query.bindValue(":quantite", result.quantite);
        query.bindValue(":disponibilite", result.disponibilite);
        query.bindValue(":date_acquisition", result.date_acquisition);
        query.bindValue(":id_fournisseur", result.id_fournisseur);
        query.bindValue(":cout_acquisition", result.cout_acquisition);

        if (query.exec()) {
            QMessageBox::information(this, "Succès", "Ressource ajoutée avec succès");
        } else {
            QMessageBox::warning(this, "Erreur", "Ressource ne peut pas être ajoutée");
        }
    } else if (operation_actuelle == "Modifier") {
        // Modification: If the operation is "Modify"
        query.prepare("UPDATE ressources SET nom = :nom, id_type_ressource = :id_type_ressource, quantite = :quantite, "
                      "disponibilite = :disponibilite, date_acquisition = :date_acquisition, id_fournisseur = :id_fournisseur, "
                      "cout_acquisition = :cout_acquisition WHERE id_ressource = :id_ressource");

        // Bind values to the query
        query.bindValue(":id_ressource", result.id_ressource);
        query.bindValue(":nom", result.nom);
        query.bindValue(":id_type_ressource", result.id_type_ressource);
        query.bindValue(":quantite", result.quantite);
        query.bindValue(":disponibilite", result.disponibilite);
        query.bindValue(":date_acquisition", result.date_acquisition);
        query.bindValue(":id_fournisseur", result.id_fournisseur);
        query.bindValue(":cout_acquisition", result.cout_acquisition);

        if (query.exec()) {
            QMessageBox::information(this, "Succès", "Ressource modifiée avec succès");
        } else if(!query.exec()) {
            qDebug() << "Erreur SQL:" << query.lastError().text();
            QMessageBox::warning(this, "Erreur", "Ressource ne peut pas être modifiée");
        }
    } else {
        QMessageBox::warning(this, "Erreur", "Aucune opération définie !");
    }

    // Close the dialog
    close();
}

void ficheressource::Annuler() {
    result.id_ressource = -1;
    close();
}

void ficheressource::ExitApp(){
    close();
}
