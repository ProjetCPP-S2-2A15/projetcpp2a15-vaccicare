#include "dialoglistemedecin.h"
#include "qsqlerror.h"
#include "qsqlquery.h"
#include "qtablewidget.h"
#include "ui_dialoglistemedecin.h"
#include <QDialog>
#include "QTableView"
#include "Design.h"
#include <QMessageBox>
#include<QMessageBox>

DialogListeMedecin::DialogListeMedecin(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::DialogListeMedecin)
{
    ui->setupUi(this);

    connect(ui->Ajoutermed,&QPushButton::clicked,this,&::DialogListeMedecin::OuvrirAjouter);
    connect(ui->Supprimermed,&QPushButton::clicked,this,&DialogListeMedecin::on_buttonSupprimer_clicked);
    connect(ui->Modifiermed,&QPushButton::clicked,this,&DialogListeMedecin::on_buttonModifier_clicked);
    connect(ui->ButtonExit,&QPushButton::clicked,this,&DialogListeMedecin::ExitDialog);

    setupDesign();

    refreshtableView();
}

DialogListeMedecin::~DialogListeMedecin()
{
    delete ui;
}

void DialogListeMedecin::setupDesign()
{

    StyleLineEdit(ui->lineEdit_rechercher);

    StyleComboBox(ui->comboBox_tri);
    StyleComboBox(ui->comboBox);

    StyleButtonGreen(ui->Ajoutermed);
    StyleButtonRed(ui->Supprimermed);
    StyleButtonBlue(ui->Modifiermed);
    StyleButtonDarkred(ui->ButtonExit);

    StyleTableView(ui->tableview);
}

void DialogListeMedecin::OuvrirAjouter() {
    ficheMedecin *NewDialog = new ficheMedecin(this,true,Medecin::GetLastID() + 1);  // Set parent for cleanup
    NewDialog->exec();
    Medecin result = NewDialog->GetResult();  // Get the Medecin object from the form
    if(result.id_medecin != -1){
        result.ajouter();
        refreshtableView();

    }


}

void DialogListeMedecin::on_buttonModifier_clicked()
{
    QModelIndex index = ui->tableview->currentIndex();
    int row = index.row();

    if (row < 0) {
        QMessageBox::warning(this, "Aucune sélection", "Veuillez sélectionner un médecin à supprimer.");
        return;
    }

    int idMedecin = ui->tableview->model()->index(row, 0).data().toInt();

    ficheMedecin *NewDialog = new ficheMedecin(this,false,idMedecin);
    NewDialog->exec();
    Medecin result = NewDialog->GetResult();  // Get the Medecin object from the form
    if(result.id_medecin != -1){
        result.modifier(idMedecin);
        refreshtableView();
    }
}

void DialogListeMedecin::on_buttonSupprimer_clicked()
{
    QModelIndex index = ui->tableview->currentIndex();
    int row = index.row();

    if (row < 0) {
        QMessageBox::warning(this, "Aucune sélection", "Veuillez sélectionner un médecin à supprimer.");
        return;
    }

    // Supposons que la colonne 0 contient l'ID du médecin
    int idMedecin = ui->tableview->model()->index(row, 0).data().toInt();

    QMessageBox::StandardButton reply;
    reply = QMessageBox::question(this, "Confirmation", "Êtes-vous sûr de vouloir supprimer ce médecin ?",
                                  QMessageBox::Yes | QMessageBox::No);

    if (reply == QMessageBox::Yes) {
        if (Medecin::supprimer(idMedecin)) {
            refreshtableView();
        }
    }

}

void DialogListeMedecin::refreshtableView()
{
    Medecin medecin;
    QString searchText = ui->lineEdit_rechercher->text().trimmed();
    QString criterion = ui->comboBox->currentText();
    QString sortCriterion = ui->comboBox_tri->currentText();
    qDebug() << "Refreshing table view with searchText:" << searchText << "and criterion:" << criterion << "and sortCriterion:" << sortCriterion;

    QSqlQueryModel* model = medecin.afficher(searchText, criterion, sortCriterion);

    if (model) {
        ui->tableview->setModel(model);

        model->setHeaderData(0, Qt::Horizontal, tr("ID Médecin"));
        model->setHeaderData(1, Qt::Horizontal, tr("Nom"));
        model->setHeaderData(2, Qt::Horizontal, tr("Spécialité"));
        model->setHeaderData(3, Qt::Horizontal, tr("Grade"));
        model->setHeaderData(4, Qt::Horizontal, tr("Email"));
        model->setHeaderData(5, Qt::Horizontal, tr("Téléphone"));
        model->setHeaderData(6, Qt::Horizontal, tr("Disponibilité"));
        model->setHeaderData(7, Qt::Horizontal, tr("Expérience"));
        model->setHeaderData(8, Qt::Horizontal, tr("Statut"));
        model->setHeaderData(9, Qt::Horizontal, tr("Login"));
        model->setHeaderData(10, Qt::Horizontal, tr("Mot de Passe"));

        ui->tableview->resizeColumnsToContents();
        qDebug() << "Table view updated successfully. Rows displayed:" << model->rowCount();

        if (model->rowCount() == 0) {
            if (searchText.isEmpty()) {
                QMessageBox::information(this, "Information", "Aucun médecin n'est enregistré dans la base de données.");
            } else {
                QMessageBox::information(this, "Information", "Aucun médecin trouvé pour les critères donnés.");
            }
        }
    } else {
        qDebug() << "Failed to update table view: Model is nullptr.";
        QMessageBox::critical(this, "Erreur", "Impossible d'afficher les médecins.");
    }
}
void DialogListeMedecin::on_lineEdit_rechercher_textChanged(const QString &text)
{
    Q_UNUSED(text);
    refreshtableView();
}

void DialogListeMedecin::on_comboBox_tri_currentTextChanged(const QString &text)
{
    Q_UNUSED(text);
    refreshtableView();
}

void DialogListeMedecin::ExitDialog(){
    close();
}

