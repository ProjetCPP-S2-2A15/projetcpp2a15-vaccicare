#include "ficheressource.h"
#include "ui_ficheressource.h"
#include "resources.h"
#include <QDialog>
#include <QMessageBox>
#include "Design.h"

ficheressource::ficheressource(QWidget *parent,bool IsModeAjout,int ID_RESSOURCE)
    : QDialog(parent)
    , IsModeAjout(true)
    , ID_Ressource(-1)
    , ui(new Ui::ficheressource)
{
    ui->setupUi(this);
    connect(ui->ButtonC, &QPushButton::clicked, this, &ficheressource::Valider);
    connect(ui->ButtonR, &QPushButton::clicked, this, &ficheressource::Annuler);

    ShowDialog();
    SetupDesign();
}


ficheressource::~ficheressource()
{
    delete ui;
}

void ficheressource::ShowDialog() {
    if(IsModeAjout){
        ui->lineEdit_3->setText(QString::number(ID_Ressource));
    } else {
        QSqlQuery query;
        query.prepare("SELECT * FROM ressources WHERE id_ressource = :id");
        query.bindValue(":id", ID_Ressource);

        if (query.exec() && query.next()) {
            ui->lineEdit->setText(query.value("id_ressource").toString());
            ui->lineEdit_2->setText(query.value("nom").toString());
            ui->lineEdit_3->setText(query.value("id_type_ressource").toString());
            ui->lineEdit_4->setText(query.value("quantite").toString());
            ui->lineEdit_5->setText(query.value("disponibilite").toString());
            ui->lineEdit_6->setText(query.value("date_acquisition").toString());
            ui->lineEdit_7->setText(query.value("id_fournisseur").toString());
            ui->lineEdit_8->setText(query.value("cout_acquisition").toString());
        } else {
            QMessageBox::warning(this, "Erreur", "Impossible de récupérer les données de la ressource.");
        }
    }
}

void ficheressource::Valider() {
    QString idText = ui->lineEdit->text().trimmed();
    QString nom = ui->lineEdit_2->text().trimmed();
    QString idTypeText = ui->lineEdit_3->text().trimmed();
    QString quantiteText = ui->lineEdit_4->text().trimmed();
    QString dispoText = ui->lineEdit_5->text().trimmed();
    QString dateText = ui->lineEdit_6->text().trimmed();
    QString idFournisseurText = ui->lineEdit_7->text().trimmed();
    QString coutText = ui->lineEdit_8->text().trimmed();

    QStringList emptyFields;
    if (idText.isEmpty()) emptyFields << "ID";
    if (nom.isEmpty()) emptyFields << "Nom";
    if (idTypeText.isEmpty()) emptyFields << "Type";
    if (quantiteText.isEmpty()) emptyFields << "Quantité";
    if (dispoText.isEmpty()) emptyFields << "Disponibilité";
    if (dateText.isEmpty()) emptyFields << "Date d'acquisition";
    if (idFournisseurText.isEmpty()) emptyFields << "Fournisseur";
    if (coutText.isEmpty()) emptyFields << "Coût";

    if (!emptyFields.isEmpty()) {
        QMessageBox::warning(this, "Champs vides", "Les champs suivants sont vides : " + emptyFields.join(", ") + " !");
        return;
    }

    bool idOk, typeOk, qteOk, dispoOk, dateOk, fournOk, coutOk;
    int id = idText.toInt(&idOk);
    int idType = idTypeText.toInt(&typeOk);
    int quantite = quantiteText.toInt(&qteOk);
    int disponibilite = dispoText.toInt(&dispoOk);
    int date = dateText.toInt(&dateOk);
    int fournisseur = idFournisseurText.toInt(&fournOk);
    int cout = coutText.toInt(&coutOk);

    if (!idOk) {
        QMessageBox::warning(this, "Erreur", "L'ID doit être un entier !");
        ui->lineEdit->setFocus();
        return;
    }
    if (!typeOk || !qteOk || !dispoOk || !dateOk || !fournOk || !coutOk) {
        QMessageBox::warning(this, "Erreur", "Tous les champs numériques doivent contenir des entiers valides !");
        return;
    }

    // Vérifier si l'ID existe déjà
    QSqlQuery checkQuery;
    checkQuery.prepare("SELECT COUNT(*) FROM ressources WHERE id_ressource = :id");
    checkQuery.bindValue(":id", id);

    if (!checkQuery.exec() || !checkQuery.next()) {
        QMessageBox::warning(this, "Erreur SQL", "Impossible de vérifier l'existence de l'ID : " + checkQuery.lastError().text());
        return;
    }

    bool existe = checkQuery.value(0).toInt() > 0;

    QSqlQuery query;
    if (existe) {
        // Modifier
        query.prepare("UPDATE ressources SET nom=:nom, id_type_ressource=:type, quantite=:quantite, disponibilite=:dispo, "
                      "date_acquisition=:date, id_fournisseur=:fournisseur, cout_acquisition=:cout WHERE id_ressource=:id");
    } else {
        // Ajouter
        query.prepare("INSERT INTO ressources (id_ressource, nom, id_type_ressource, quantite, disponibilite, date_acquisition, id_fournisseur, cout_acquisition) "
                      "VALUES (:id, :nom, :type, :quantite, :dispo, :date, :fournisseur, :cout)");
    }

    query.bindValue(":id", id);
    query.bindValue(":nom", nom);
    query.bindValue(":type", idType);
    query.bindValue(":quantite", quantite);
    query.bindValue(":dispo", disponibilite);
    query.bindValue(":date", date);
    query.bindValue(":fournisseur", fournisseur);
    query.bindValue(":cout", cout);
    if (query.exec()) {
        //QString op = existe ? "Modification" : "Ajout";
        //QMessageBox::information(this, "Succès", op + " réussie !");
        result.id_ressource = id;
        result.nom = nom;
        result.id_type_ressource = idType;
        result.quantite = quantite;
        result.disponibilite = disponibilite;
        result.date_acquisition = date;
        result.id_fournisseur = fournisseur;
        result.cout_acquisition = cout;
        close();
    } else {
        QString op = existe ? "modification" : "ajout";
        QMessageBox::warning(this, "Erreur SQL", "Échec de la " + op + " : " + query.lastError().text());
    }
}


void ficheressource::Annuler() {
    result.id_ressource = -1;
    close();
}


void ficheressource::SetupDesign(){

    StyleLineEdit(ui->lineEdit);
    StyleLineEdit(ui->lineEdit_2);
    StyleLineEdit(ui->lineEdit_3);
    StyleLineEdit(ui->lineEdit_4);
    StyleLineEdit(ui->lineEdit_5);
    StyleLineEdit(ui->lineEdit_6);
    StyleLineEdit(ui->lineEdit_7);
    StyleLineEdit(ui->lineEdit_8);

    StyleButtonGreen(ui->ButtonC);
    StyleButtonRed(ui->ButtonR);


}


