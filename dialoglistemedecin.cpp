#include "dialoglistemedecin.h"
#include "qsqlerror.h"
#include "qsqlquery.h"
#include "qtablewidget.h"
#include "ui_dialoglistemedecin.h"
#include<QMessageBox>
#include "fichemedecin.h"
#include <QDialog>
#include "QTableView"

DialogListeMedecin::DialogListeMedecin(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::DialogListeMedecin)
{
    ui->setupUi(this);
    connect(ui->Ajoutermed,&QPushButton::clicked,this,&::DialogListeMedecin::on_buttonModifier_clicked);
    connect(ui->Supprimermed,&QPushButton::clicked,this,&DialogListeMedecin::on_buttonModifier_clicked);
    connect(ui->Modifiermed,&QPushButton::clicked,this,&DialogListeMedecin::on_buttonSupprimer_clicked);
}

DialogListeMedecin::~DialogListeMedecin()
{
    delete ui;
}
void DialogListeMedecin::OuvrirAjouter() {
    ficheMedecin *NewDialog = new ficheMedecin(this);  // Set parent for cleanup
    if (NewDialog->exec() == QDialog::Accepted) {
        Medecin result = NewDialog->GetResult();  // Get the Medecin object from the form
    }

    delete NewDialog;  // Clean up memory
}

void DialogListeMedecin::on_buttonModifier_clicked()
{
    ficheMedecin *dialog = new ficheMedecin(this);
    dialog->exec();
    //delete dialog;
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
           // FillTable(ui->comboBox_tri, ui->comboBox_tri());
        }
    }

}
//void DialogListeMedecin::on_pushButton_2_supprimer_clicked()
//{
   // bool idOk;
    //int id_cherch = ui->lineEdit_2->text().toInt(&idOk);

    //if (!idOk) {
      //  QMessageBox::warning(this, "Erreur", "Veuillez entrer un ID valide !");
        //return;
    //}

    //QSqlQuery query;
    //query.prepare("DELETE FROM MEDECIN WHERE ID_MEDECIN = :id");
    //query.bindValue(":id", id_cherch);

    //if (query.exec()) {
      //  QMessageBox::information(this, "Succès", "Médecin supprimé avec succès !");
      //  qDebug() << "Médecin supprimé avec succès.";
        //refreshtableView(); // Recharge les données de la table
    //} else {
      //  QMessageBox::critical(this, "Erreur", "Échec de la suppression du médecin.");
        //qDebug() << "Erreur lors de la suppression du médecin : " << query.lastError().text();
    //}
//}
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

// void DialogListeMedecin::on_pushButton_5_clicked()
// {

//     qDebug() << "Bouton Ressources cliqué.";
//     qDebug() << "Nombre d'onglets :" << ui->tabWidget->count();
//     qDebug() << "Onglet actif avant redirection :" << ui->tabWidget->currentIndex() << " (" << ui->tabWidget->tabText(ui->tabWidget->currentIndex()) << ")";
//     for (int i = 0; i < ui->tabWidget->count(); ++i) {
//         qDebug() << "Onglet" << i << ":" << ui->tabWidget->tabText(i) << "Enabled:" << ui->tabWidget->isTabEnabled(i);
//         qDebug() << "  ObjectName de l'onglet" << i << ":" << ui->tabWidget->widget(i)->objectName();
//     }

//     const int targetIndex = 3;



//     if (!ui->tabWidget->isTabEnabled(targetIndex)) {
//         qDebug() << "L'onglet tab_4 (index" << targetIndex << ") est désactivé, activation...";
//         ui->tabWidget->setTabEnabled(targetIndex, true);
//     }


//     ui->tabWidget->setCurrentIndex(targetIndex);

//     qDebug() << "Redirection effectuée.";
//     qDebug() << "Onglet actif après redirection :" << ui->tabWidget->currentIndex() << " (" << ui->tabWidget->tabText(ui->tabWidget->currentIndex()) << ")";


//     if (ui->tableView_3) {
//         qDebug() << "tableView_3 est affiché dans l'onglet tab_4.";
//         ui->tableView_3->resizeColumnsToContents();
//     } else {
//         qDebug() << "Erreur : tableView_3 n'est pas trouvé dans l'onglet tab_4.";
//     }
// }

