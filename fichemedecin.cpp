#include "fichemedecin.h"
#include "medecin.h"
#include "qsqlerror.h"
#include "qsqlquery.h"
#include "ui_fichemedecin.h"
#include<QMessageBox>

ficheMedecin::ficheMedecin(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::ficheMedecin)
{
    ui->setupUi(this);

    connect(ui->Confirmer,&QPushButton::clicked,this,&ficheMedecin::on_pushButton_Confirmer_clicked);
    connect(ui->Retourner,&QPushButton::clicked,this,&ficheMedecin::ExitApp);
}

ficheMedecin::~ficheMedecin()
{
    delete ui;
}

void ficheMedecin::Operation(const QString &op)
{
    operation_actuelle = op;
}

void ficheMedecin::on_Pat_Button_Confirmer_clicked()
{
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
    } else {
        QMessageBox::warning(this, "Erreur", "Aucune opération définie !");
    }

}

void ficheMedecin::on_pushButton_Confirmer_clicked()
{
    QSqlQuery query;

    QString id = ui->lineEdit_3->text();
    QString nom = ui->lineEdit_4->text();
    QString specialite = ui->lineEdit_5->text();
    QString grade = ui->lineEdit_6->text();
    QString email = ui->lineEdit_7->text();
    QString telephone = ui->lineEdit_8->text();
    QString disponibilite = ui->lineEdit_9->text();
    QString experience = ui->lineEdit_10->text();
    QString statut = ui->lineEdit_11->text();
    QString login = ui->lineEdit_12->text();
    QString mot_de_passe = ui->lineEdit_13->text();

    if (operation_actuelle == "Ajouter") {
        query.prepare("INSERT INTO chercheur (ID_CHERCHEUR, NOM, SPECIALITE, GRADE, EMAIL, TELEPHONE, DISPONIBILITE, EXPERIENCE, STATUT, LOGIN, MOT_DE_PASSE, DROIT) "
                      "VALUES (:id, :nom, :specialite, :grade, :email, :telephone, :disponibilite, :experience, :statut, :login, :mot_de_passe, 1)");
    } else if (operation_actuelle == "Modifier") {
        query.prepare("UPDATE chercheur SET NOM=:nom, SPECIALITE=:specialite, GRADE=:grade, EMAIL=:email, TELEPHONE=:telephone, "
                      "DISPONIBILITE=:disponibilite, EXPERIENCE=:experience, STATUT=:statut, LOGIN=:login, "
                      "MOT_DE_PASSE=:mot_de_passe, DROIT= 1 WHERE ID_CHERCHEUR=:id");
    } else if (operation_actuelle == "Supprimer") {
        query.prepare("DELETE FROM chercheur WHERE ID_CHERCHEUR=:id");
    } else {
        QMessageBox::warning(this, "Erreur", "Aucune opération définie !");
        return;
    }

    if (operation_actuelle == "Modifier") {
    query.prepare("UPDATE chercheur SET "
                  "NOM = :nom, "
                  "SPECIALITE = :specialite, "
                  "GRADE = :grade, "
                  "EMAIL = :email, "
                  "TELEPHONE = :telephone, "
                  "DISPONIBILITE = :disponibilite, "
                  "EXPERIENCE = :experience, "
                  "STATUT = :statut, "
                  "LOGIN = :login, "
                  "MOT_DE_PASSE = :mot_de_passe, "
                  "DROIT = :droit "
                  "WHERE ID_CHERCHEUR = :id");

    query.bindValue(":id", id);
    query.bindValue(":nom", nom);
    query.bindValue(":specialite", specialite);
    query.bindValue(":grade", grade);
    query.bindValue(":email", email);
    query.bindValue(":telephone", telephone);
    query.bindValue(":disponibilite", disponibilite);
    query.bindValue(":experience", experience);
    query.bindValue(":statut", statut);
    query.bindValue(":login", login);
    query.bindValue(":mot_de_passe", mot_de_passe);
    query.bindValue(":droit", 1);

    if (query.exec()) {
        QMessageBox::information(this, "Info", "Modification effectuée");
        accept();
    } else {
        QMessageBox::critical(this, "Erreur", "Erreur lors de la modification : " + query.lastError().text());
    }
}
    if(operation_actuelle != "Supprimer") {
        query.bindValue(":nom", nom);
        query.bindValue(":specialite", specialite);
        query.bindValue(":grade", grade);
        query.bindValue(":email", email);
        query.bindValue(":telephone", telephone);
        query.bindValue(":disponibilite", disponibilite);
        query.bindValue(":experience", experience);
        query.bindValue(":statut", statut);
        query.bindValue(":login", login);
        query.bindValue(":mot_de_passe", mot_de_passe);
        query.bindValue(":droit", 1);
    }
    query.bindValue(":id", id);

    // Execute and show result
    if (query.exec()) {
        QString msg = (operation_actuelle == "Ajouter") ? "Ajout effectué" :
                          (operation_actuelle == "Modifier") ? "Modification effectuée" :
                          "Suppression effectuée";
        QMessageBox::information(this, "Info", msg);
        accept(); // Ferme le formulaire
    } else {
        QMessageBox::critical(this, "Erreur", "Erreur SQL : " + query.lastError().text());
    }

    close();
}

void ficheMedecin::Annuler() {
    result.id_medecin = -1;  // Utilise le vrai nom du setter ici
    close();
}
void ficheMedecin::ExitApp(){
    close();
}


