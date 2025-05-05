#include "fichemedecin.h"
#include "ui_fichemedecin.h"
#include<QMessageBox>

ficheMedecin::ficheMedecin(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::ficheMedecin)
{
    ui->setupUi(this);
}

ficheMedecin::~ficheMedecin()
{
    delete ui;
}
void ficheMedecin::on_pushButton3_ajouter_clicked()
{
    qDebug() << "Bouton Ajouter pressé !";

    QString idText = ui->lineEdit_3->text().trimmed();
    QString nom = ui->lineEdit_4->text().trimmed();
    QString specialite = ui->lineEdit_5->text().trimmed();
    QString grade = ui->lineEdit_7->text().trimmed();
    QString email = ui->lineEdit_6->text().trimmed();
    QString telephone = ui->lineEdit_10->text().trimmed();
    QString disponibilite = ui->lineEdit_12->text().trimmed();
    QString experienceText = ui->lineEdit_9->text().trimmed();
    QString statut = ui->lineEdit_8->text().trimmed();
    QString login = ui->lineEdit_13->text().trimmed();
    QString mot_de_passe = ui->lineEdit_11->text().trimmed();

    QStringList emptyFields;
    if (idText.isEmpty()) emptyFields << "ID";
    if (nom.isEmpty()) emptyFields << "Nom";
    if (specialite.isEmpty()) emptyFields << "Spécialité";
    if (grade.isEmpty()) emptyFields << "Grade";
    if (email.isEmpty()) emptyFields << "Email";
    if (telephone.isEmpty()) emptyFields << "Téléphone";
    if (disponibilite.isEmpty()) emptyFields << "Disponibilité";
    if (experienceText.isEmpty()) emptyFields << "Expérience";
    if (statut.isEmpty()) emptyFields << "Statut";
    if (login.isEmpty()) emptyFields << "Login";
    if (mot_de_passe.isEmpty()) emptyFields << "Mot de passe";

    if (!emptyFields.isEmpty()) {
        QMessageBox::warning(this, "Erreur de saisie", "Les champs suivants sont vides : " + emptyFields.join(", ") + " !");
        return;
    }

    bool idOk, expOk;
    int id_medecin = idText.toInt(&idOk);
    int experience = experienceText.toInt(&expOk);

    if (!idOk) {
        QMessageBox::warning(this, "Erreur de saisie", "L'ID doit être un nombre entier valide !");
        ui->lineEdit_3->setFocus();
        return;
    }

    if (!expOk) {
        QMessageBox::warning(this, "Erreur de saisie", "L'expérience doit être un nombre entier valide !");
        ui->lineEdit_9->setFocus();
        return;
    }

    Medecin M2(id_medecin, nom, specialite, grade, email, telephone, experience, statut, disponibilite, login, mot_de_passe);
    bool test = M2.ajouter();
    if (test) {
        QMessageBox::information(this, "Succès", "Médecin ajouté avec succès !");
        //refreshtableView();
    } else {
        QMessageBox::critical(this, "Erreur", "Ajout non effectué !");
        qDebug() << "Erreur lors de l'ajout du médecin.";
    }
}
void ficheMedecin::on_pushButton_modifier_clicked()
{
    bool idOk, expOk;
    int id_medecin = ui->lineEdit_3->text().toInt(&idOk);
    QString nom, specialite, grade, email, telephone, disponibilite, statut, login, mot_de_passe;
    int experience;

    if (!idOk) {
        QMessageBox::warning(this, "Erreur de saisie", "Veuillez entrer un ID valide !");
        return;
    }

    Medecin M;
    if (M.fetchMedecinById(id_medecin, nom, specialite, grade, email, telephone, disponibilite, experience, statut, login, mot_de_passe)) {
        nom = ui->lineEdit_4->text();
        specialite = ui->lineEdit_5->text();
        grade = ui->lineEdit_7->text();
        email = ui->lineEdit_6->text();
        telephone = ui->lineEdit_10->text();
        disponibilite = ui->lineEdit_12->text();
        experience = ui->lineEdit_9->text().toInt(&expOk);
        statut = ui->lineEdit_8->text();
        login = ui->lineEdit_13->text();
        mot_de_passe = ui->lineEdit_11->text();

        if (!expOk) {
            QMessageBox::warning(this, "Erreur de saisie", "Veuillez entrer une expérience valide !");
            return;
        }

        Medecin updatedMedecin(id_medecin, nom, specialite, grade, email, telephone, experience, statut, disponibilite, login, mot_de_passe);
        bool success = updatedMedecin.modifier(id_medecin);
        if (success) {
            QMessageBox::information(this, "Succès", "Médecin modifié avec succès !");
            qDebug() << "Le médecin a été modifié avec succès.";
            //refreshtableView();
        } else {
            QMessageBox::critical(this, "Erreur", "Échec de la modification du médecin.");
            qDebug() << "Erreur lors de la modification du médecin.";
        }
    } else {
        QMessageBox::warning(this, "Erreur", "Médecin non trouvé avec cet ID.");
    }
}

