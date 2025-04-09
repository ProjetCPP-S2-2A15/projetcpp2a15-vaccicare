#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "patient.h"
#include <QMessageBox>
#include <QSqlError>
#include <QRegularExpressionValidator>
#include <QSqlQuery>
#include <QTextStream>
#include <QFileDialog>
#include <QTextDocument>
#include <QSystemTrayIcon>
#include <QMessageBox>
#include <QRegularExpression>
#include <QtPrintSupport/QPrinter>
#include <QtPrintSupport/QPrintDialog>
#include <QTextDocument>
#include <QFileDialog>
#include <QFileInfo>
#include <QTextStream>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    // Validators for numeric fields
    ui->Pat_Line_ID->setValidator(new QIntValidator(0, 99999999, this));
    ui->Pat_Telephone->setValidator(new QIntValidator(0, 99999999, this)); // Allow only 8 digits
    ui->Pat_Line_Id_Testeur->setValidator(new QIntValidator(0, 99999999, this));

    // Populate combo boxes with predefined values
    ui->Pat_Combo_Sexe->addItems({"Homme", "Femme"}); // Only "Homme" and "Femme"
    ui->Pat_Combo_groupe->addItems({"A+", "A-", "B+", "B-", "AB+", "AB-", "O+", "O-"});
    ui->Pat_Combo_Statut->addItems({"Vacciné", "Non vacciné", "En attente"});

    // Initialize table view and combo box for IDs
    ui->Pat_TableView_Res->setModel(P.Afficher());
    ui->Pat_Combo_IDs->setModel(P.Afficher_ID());
}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::on_Pat_Button_Ajouter_clicked()
{
    int ID_PATIENT = ui->Pat_Line_ID->text().toInt();
        QString ID_String = ui->Pat_Line_ID->text();
        QString NOM = ui->Pat_Line_Nom->text();
        QString PRENOM = ui->Pat_Line_Prenom->text();
        QDate DATE_NAISSANCE = ui->Pat_Date_naissance->date();
        QString SEXE = ui->Pat_Combo_Sexe->currentText();
        QString ADRESSE = ui->Pat_Adresse->text();
        QString TELEPHONE_String = ui->Pat_Telephone->text();
        int TELEPHONE = TELEPHONE_String.toInt();
        QString EMAIL = ui->Pat_Email->text();
        QString GROUPE_SANGUIN = ui->Pat_Combo_groupe->currentText();
        QString STATUT_VACCINAL = ui->Pat_Combo_Statut->currentText();
        QDate DATE_VACCIN = ui->Pat_Date_Vaccin->date();
        int ID_PROJET_TESTER = ui->Pat_Line_Id_Testeur->text().toInt();
        QString ID_PROJET_String = ui->Pat_Line_Id_Testeur->text();

        // Input validation
        // Check if all fields are filled
        if (ID_String.isEmpty() || NOM.isEmpty() || PRENOM.isEmpty() || ADRESSE.isEmpty() ||
            TELEPHONE_String.isEmpty() || EMAIL.isEmpty() || ID_PROJET_String.isEmpty()) {
            ui->Pat_Label_GestionInfo->setText("Erreur : Tous les champs doivent être remplis");
            return;
        }

        // Check if TELEPHONE has exactly 8 digits
        QRegularExpression re("^[0-9]{8}$");
        if (!re.match(TELEPHONE_String).hasMatch()) {
            ui->Pat_Label_GestionInfo->setText("Erreur : Le numéro de téléphone doit contenir exactement 8 chiffres");
            return;
        }

        // Check if ID_PATIENT already exists
        QSqlQuery query;
        query.prepare("SELECT COUNT(*) FROM PATIENT WHERE ID_PATIENT = :ID_PATIENT");
        query.bindValue(":ID_PATIENT", ID_PATIENT);
        if (query.exec() && query.next()) {
            if (query.value(0).toInt() > 0) {
                ui->Pat_Label_GestionInfo->setText("Erreur : Cet ID_PATIENT existe déjà");
                return;
            }
        } else {
            ui->Pat_Label_GestionInfo->setText("Erreur : Impossible de vérifier l'ID_PATIENT");
            return;
        }

        Patient P(ID_PATIENT, NOM, PRENOM, DATE_NAISSANCE, SEXE, ADRESSE, TELEPHONE, EMAIL, GROUPE_SANGUIN, STATUT_VACCINAL, DATE_VACCIN, ID_PROJET_TESTER);
        bool test = P.Ajouter();

        if (test) {
            ui->Pat_Label_GestionInfo->setText("Ajout effectué avec succès");
            ui->Pat_TableView_Res->setModel(P.Afficher());
            ui->Pat_Combo_IDs->setModel(P.Afficher_ID());

            // System tray notification
            QSystemTrayIcon *trayIcon = new QSystemTrayIcon(this);
            trayIcon->setIcon(QIcon(":/icons/success.png"));
            trayIcon->show();
            trayIcon->showMessage(
                "Patient Added",
                QString("Patient %1 %2 has been successfully added.").arg(NOM, PRENOM),
                QSystemTrayIcon::Information,
                3000
            );
        } else {
            ui->Pat_Label_GestionInfo->setText("Ajout non effectué");
        }
}

void MainWindow::on_Pat_Button_Modifier_clicked()
{
    int ID_PATIENT = ui->Pat_Line_ID->text().toInt();
        QString ID_String = ui->Pat_Line_ID->text();
        QString NOM = ui->Pat_Line_Nom->text();
        QString PRENOM = ui->Pat_Line_Prenom->text();
        QDate DATE_NAISSANCE = ui->Pat_Date_naissance->date();
        QString SEXE = ui->Pat_Combo_Sexe->currentText();
        QString ADRESSE = ui->Pat_Adresse->text();
        QString TELEPHONE_String = ui->Pat_Telephone->text();
        int TELEPHONE = TELEPHONE_String.toInt();
        QString EMAIL = ui->Pat_Email->text();
        QString GROUPE_SANGUIN = ui->Pat_Combo_groupe->currentText();
        QString STATUT_VACCINAL = ui->Pat_Combo_Statut->currentText();
        QDate DATE_VACCIN = ui->Pat_Date_Vaccin->date();
        int ID_PROJET_TESTER = ui->Pat_Line_Id_Testeur->text().toInt();
        QString ID_PROJET_String = ui->Pat_Line_Id_Testeur->text();

        // Input validation
        // Check if all fields are filled
        if (ID_String.isEmpty() || NOM.isEmpty() || PRENOM.isEmpty() || ADRESSE.isEmpty() ||
            TELEPHONE_String.isEmpty() || EMAIL.isEmpty() || ID_PROJET_String.isEmpty()) {
            ui->Pat_Label_GestionInfo->setText("Erreur : Tous les champs doivent être remplis");
            return;
        }

        // Check if TELEPHONE has exactly 8 digits
        QRegularExpression re("^[0-9]{8}$");
        if (!re.match(TELEPHONE_String).hasMatch()) {
            ui->Pat_Label_GestionInfo->setText("Erreur : Le numéro de téléphone doit contenir exactement 8 chiffres");
            return;
        }

        // Check if ID_PATIENT exists
        QSqlQuery query;
        query.prepare("SELECT COUNT(*) FROM PATIENT WHERE ID_PATIENT = :ID_PATIENT");
        query.bindValue(":ID_PATIENT", ID_PATIENT);
        if (query.exec() && query.next()) {
            if (query.value(0).toInt() == 0) { // If ID_PATIENT does not exist
                ui->Pat_Label_GestionInfo->setText("Erreur : Cet ID_PATIENT n'existe pas");
                return;
            }
        } else {
            ui->Pat_Label_GestionInfo->setText("Erreur : Impossible de vérifier l'ID_PATIENT");
            return;
        }

        Patient P(ID_PATIENT, NOM, PRENOM, DATE_NAISSANCE, SEXE, ADRESSE, TELEPHONE, EMAIL, GROUPE_SANGUIN, STATUT_VACCINAL, DATE_VACCIN, ID_PROJET_TESTER);
        bool test = P.Modifier();

        if (test) {
            ui->Pat_Label_GestionInfo->setText("Modification effectuée avec succès");
            ui->Pat_TableView_Res->setModel(P.Afficher());
            ui->Pat_Combo_IDs->setModel(P.Afficher_ID());
        } else {
            ui->Pat_Label_GestionInfo->setText("Modification non effectuée");
        }
}

void MainWindow::on_Pat_Button_Supprimer_clicked()
{
    Patient P;
        P.setID_PATIENT(ui->Pat_Combo_IDs->currentText().toInt());
        bool test = P.Supprimer(P.getID_PATIENT());
        if (test) {
            ui->Pat_Label_GestionInfo->setText("Suppression effectuée avec succès");
            ui->Pat_TableView_Res->setModel(P.Afficher());
            ui->Pat_Combo_IDs->setModel(P.Afficher_ID());
        } else {
            ui->Pat_Label_GestionInfo->setText("Suppression non effectuée");
        }
}

void MainWindow::on_Pat_Combo_IDs_currentIndexChanged(int index)
{
    int id = ui->Pat_Combo_IDs->currentText().toInt();
        QString id_1 = QString::number(id);
        QSqlQuery query1;
        query1.prepare("SELECT * FROM PATIENT WHERE ID_PATIENT='" + id_1 + "'");
        if (query1.exec()) {
            while (query1.next()) {
                ui->Pat_Line_ID->setText(query1.value(0).toString());
                ui->Pat_Line_Nom->setText(query1.value(1).toString());
                ui->Pat_Line_Prenom->setText(query1.value(2).toString());
                ui->Pat_Date_naissance->setDate(query1.value(3).toDate());
                ui->Pat_Combo_Sexe->setCurrentText(query1.value(4).toString());
                ui->Pat_Adresse->setText(query1.value(5).toString());
                ui->Pat_Telephone->setText(query1.value(6).toString());
                ui->Pat_Email->setText(query1.value(7).toString());
                ui->Pat_Combo_groupe->setCurrentText(query1.value(8).toString());
                ui->Pat_Combo_Statut->setCurrentText(query1.value(9).toString());
                ui->Pat_Date_Vaccin->setDate(query1.value(10).toDate());
                ui->Pat_Line_Id_Testeur->setText(query1.value(11).toString());
            }
        } else {
            ui->Pat_Label_GestionInfo->setText("Erreur de chargement");
        }
}

void MainWindow::on_Pat_Button_Tri_Nom_clicked()
{
    ui->Pat_Label_InfoAffichage->setText("Tri par NOM effectué");
        ui->Pat_TableView_Res->setModel(P.Afficher_Tri_NOM());
}

void MainWindow::on_Pat_Button_Tri_Statut_clicked()
{
    ui->Pat_Label_InfoAffichage->setText("Tri par STATUT_VACCINAL effectué");
        ui->Pat_TableView_Res->setModel(P.Afficher_Tri_STATUT_VACCINAL());
}

void MainWindow::on_Pat_Button_Tri_Type_clicked()
{
    ui->Pat_Label_InfoAffichage->setText("Tri par GROUPE_SANGUIN effectué");
        ui->Pat_TableView_Res->setModel(P.Afficher_Tri_GROUPE_SANGUIN());
}

void MainWindow::on_Pat_Line_Recherche_textChanged(const QString &arg1)
{
    P.clearTable(ui->Pat_TableView_Res);
        P.Recherche(ui->Pat_TableView_Res, arg1);
}

void MainWindow::on_Pat_Button_ExportPDF_clicked()
{
    QString strStream;
        QTextStream out(&strStream);

        const int rowCount = ui->Pat_TableView_Res->model()->rowCount();
        const int columnCount = ui->Pat_TableView_Res->model()->columnCount();

        out << "<html>\n"
               "<head>\n"
               "<meta Content=\"Text/html; charset=Windows-1251\">\n"
               << QString("<title>%1</title>\n").arg("Export PDF")
               << "</head>\n"
               "<body bgcolor=#ffffff link=#5000A0>\n"
               "<center><h1>Liste des Recherches</h1><br><br>\n"
               "<table border=1 cellspacing=0 cellpadding=2>\n";

        // Table headers
        out << "<thead><tr bgcolor=#f0f0f0><th>Numéro</th>";
        for (int column = 0; column < columnCount; ++column) {
            if (!ui->Pat_TableView_Res->isColumnHidden(column)) {
                out << QString("<th>%1</th>").arg(
                    ui->Pat_TableView_Res->model()->headerData(column, Qt::Horizontal).toString());
            }
        }
        out << "</tr></thead>\n";

        // Table data
        for (int row = 0; row < rowCount; ++row) {
            out << "<tr><td>" << row + 1 << "</td>";
            for (int column = 0; column < columnCount; ++column) {
                if (!ui->Pat_TableView_Res->isColumnHidden(column)) {
                    QString data = ui->Pat_TableView_Res->model()
                                       ->data(ui->Pat_TableView_Res->model()->index(row, column))
                                       .toString()
                                       .simplified();
                    out << QString("<td>%1</td>").arg(data.isEmpty() ? "&nbsp;" : data);
                }
            }
            out << "</tr>\n";
        }

        out << "</table></center>\n</body>\n</html>\n";

        QString fileName = QFileDialog::getSaveFileName(this, "Sauvegarder en PDF", "", "*.pdf");
        if (QFileInfo(fileName).suffix().isEmpty())
            fileName.append(".pdf");

        QPrinter printer(QPrinter::HighResolution);
        printer.setOutputFormat(QPrinter::PdfFormat);
        printer.setPageSize(QPageSize(QPageSize::A4));
        printer.setOutputFileName(fileName);

        QTextDocument doc;
        doc.setHtml(strStream);
        doc.print(&printer);
}
