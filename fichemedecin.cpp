#include "fichemedecin.h"
#include "Design.h"

ficheMedecin::ficheMedecin(QWidget *parent,bool IsModeAjout,int Id_Medecin)
    : QDialog(parent)
    , ui(new Ui::ficheMedecin)
{
    ui->setupUi(this);

    connect(ui->Confirmer,&QPushButton::clicked,this,&ficheMedecin::Comfirmer);
    connect(ui->Retourner,&QPushButton::clicked,this,&ficheMedecin::Annuler);
    ui->lineEdit_3->setReadOnly(true);
    this->IsModeAjout = IsModeAjout;
    this->ID_Medecin = Id_Medecin;

    ShowDialog();
    setupDesign();
}

ficheMedecin::~ficheMedecin()
{
    delete ui;
}

void ficheMedecin::setupDesign() {

    StyleLineEdit(ui->lineEdit_3);
    StyleLineEdit(ui->lineEdit_4);
    StyleLineEdit(ui->lineEdit_5);
    StyleLineEdit(ui->lineEdit_6);
    StyleLineEdit(ui->lineEdit_7);
    StyleLineEdit(ui->lineEdit_8);
    StyleLineEdit(ui->lineEdit_9);
    StyleLineEdit(ui->lineEdit_10);
    StyleLineEdit(ui->lineEdit_11);
    StyleLineEdit(ui->lineEdit_12);
    StyleLineEdit(ui->lineEdit_13);

    StyleButtonGreen(ui->Confirmer);
    StyleButtonRed(ui->Retourner);

}


void ficheMedecin::ShowDialog(){

    if(IsModeAjout){
        ui->lineEdit_3->setText(QString::number(ID_Medecin));
    }else{
        Medecin M = Medecin::fetchMedecinById(ID_Medecin);
        ui->lineEdit_3->setText(QString::number(ID_Medecin));
        ui->lineEdit_4->setText(M.nom);
        ui->lineEdit_5->setText(M.specialite);
        ui->lineEdit_7->setText(M.grade);
        ui->lineEdit_6->setText(M.email);
        ui->lineEdit_10->setText(M.telephone);
        ui->lineEdit_12->setText(M.disponibilite);
        ui->lineEdit_9->setText(QString::number(M.experience));
        ui->lineEdit_8->setText(M.statut);
        ui->lineEdit_13->setText(M.login);
        ui->lineEdit_11->setText(M.mot_de_passe);
    }
}

void ficheMedecin::Comfirmer() {

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
    result = M2;
    close();
}

void ficheMedecin::Annuler() {
    result.id_medecin = -1;  // Utilise le vrai nom du setter ici
    close();
}


