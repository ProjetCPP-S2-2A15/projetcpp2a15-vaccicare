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
#include <QtCharts>
#include <QtCharts/QChartView>
#include <QtCharts/QChart>
#include <QtCharts/QPieSeries>
#include <QPainter>
#include <QSqlQuery>
#include <QSqlError>
#include <QtCharts/QPieSlice>
#include <QSsl>
#include <QSslSocket> // Add this for SSL support
#include <QMessageBox>

#include <QScopedPointer>
#include "smtp.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    // Validators for numeric fields
    ui->Pat_Line_ID->setValidator(new QIntValidator(0, 99999999, this));
    ui->Pat_Telephone->setValidator(new QIntValidator(0, 99999999, this)); // Allow only 8 digits
    ui->Pat_Line_Id_Testeur->setValidator(new QIntValidator(0, 99999999, this));

    // Initialize table view and combo box for IDs
    ui->Pat_TableView_Res->setModel(P.Afficher());
    ui->Pat_Combo_IDs->setModel(P.Afficher_ID());
    highlightDates();
    sendVaccinationReminders();
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

void MainWindow::on_Pat_Button_Statistique_clicked()
{   clearSpLabelStats();
    QChartView *chartView;
        QSqlQuery query;
        qreal tot = 0, aPositive = 0, aNegative = 0, bPositive = 0, bNegative = 0, abPositive = 0, abNegative = 0, oPositive = 0, oNegative = 0;

        // Step 1: Get the total count of patients
        query.prepare("SELECT COUNT(*) FROM PATIENT");
        if (query.exec() && query.next()) {
            tot = query.value(0).toDouble();
        } else {
            qDebug() << "Error getting total count:" << query.lastError().text();
            ui->Pat_Label_Stats->setText("❌ Erreur lors du calcul du total des patients.");
            return;
        }

        // Step 2: Count patients by blood group
        query.prepare(
            "SELECT GROUPE_SANGUIN, COUNT(*) "
            "FROM PATIENT "
            "WHERE GROUPE_SANGUIN IS NOT NULL "
            "GROUP BY GROUPE_SANGUIN"
        );

        if (query.exec()) {
            while (query.next()) {
                QString bloodGroup = query.value(0).toString();
                int count = query.value(1).toInt();
                if (bloodGroup == "A+") {
                    aPositive = count;
                } else if (bloodGroup == "A-") {
                    aNegative = count;
                } else if (bloodGroup == "B+") {
                    bPositive = count;
                } else if (bloodGroup == "B-") {
                    bNegative = count;
                } else if (bloodGroup == "AB+") {
                    abPositive = count;
                } else if (bloodGroup == "AB-") {
                    abNegative = count;
                } else if (bloodGroup == "O+") {
                    oPositive = count;
                } else if (bloodGroup == "O-") {
                    oNegative = count;
                }
            }
        } else {
            qDebug() << "Error calculating blood groups:" << query.lastError().text();
            ui->Pat_Label_Stats->setText("❌ Erreur lors du calcul des groupes sanguins: " + query.lastError().text());
            return;
        }

        // Step 3: Calculate percentages
        qreal pApos = (tot > 0) ? (aPositive / tot) : 0;
        qreal pAneg = (tot > 0) ? (aNegative / tot) : 0;
        qreal pBpos = (tot > 0) ? (bPositive / tot) : 0;
        qreal pBneg = (tot > 0) ? (bNegative / tot) : 0;
        qreal pABpos = (tot > 0) ? (abPositive / tot) : 0;
        qreal pABneg = (tot > 0) ? (abNegative / tot) : 0;
        qreal pOpos = (tot > 0) ? (oPositive / tot) : 0;
        qreal pOneg = (tot > 0) ? (oNegative / tot) : 0;

        // Step 4: Create the pie series
        QPieSeries *series = new QPieSeries();
        series->append("A+", pApos);
        series->append("A-", pAneg);
        series->append("B+", pBpos);
        series->append("B-", pBneg);
        series->append("AB+", pABpos);
        series->append("AB-", pABneg);
        series->append("O+", pOpos);
        series->append("O-", pOneg);

        // Set labels for each slice
        for (QPieSlice *slice : series->slices()) {
            slice->setLabelVisible();
            slice->setLabel(QString("%1: %2%").arg(slice->label()).arg(slice->percentage() * 100, 0, 'f', 1));
        }

        // Step 5: Create the chart
        QChart *chart = new QChart();
        chart->addSeries(series);
        chart->setTitle("Statistiques des Patients par Groupe Sanguin");
        chart->legend()->show();
        chart->setAnimationOptions(QChart::AllAnimations);
        chart->setTheme(QChart::ChartThemeQt);

        // Step 6: Create the chart view
        chartView = new QChartView(chart, ui->Pat_Label_Stats);
        chartView->setRenderHint(QPainter::Antialiasing);
        chartView->setMinimumSize(570, 570);

        // Add the chart view to the layout
        QVBoxLayout *layout = new QVBoxLayout(ui->Pat_Label_Stats);
        layout->addWidget(chartView);
        ui->Pat_Label_Stats->setLayout(layout);

        chartView->show();

}

void MainWindow::on_Pat_Button_Statistique_2_clicked()
{   clearSpLabelStats();
    QChartView *chartView;
        QSqlQuery query;
        qreal tot = 0, vaccinated = 0, notVaccinated = 0, pending = 0;

        // Step 1: Get the total count of patients
        query.prepare("SELECT COUNT(*) FROM PATIENT");
        if (query.exec() && query.next()) {
            tot = query.value(0).toDouble();
        } else {
            qDebug() << "Error getting total count:" << query.lastError().text();
            ui->Pat_Label_Stats->setText("❌ Erreur lors du calcul du total des patients.");
            return;
        }

        // Step 2: Count patients by vaccination status
        query.prepare(
            "SELECT STATUT_VACCINAL, COUNT(*) "
            "FROM PATIENT "
            "WHERE STATUT_VACCINAL IS NOT NULL "
            "GROUP BY STATUT_VACCINAL"
        );

        if (query.exec()) {
            while (query.next()) {
                QString status = query.value(0).toString();
                int count = query.value(1).toInt();
                if (status == "Vaccine") {
                    vaccinated = count;
                } else if (status == "Non Vaccine") {
                    notVaccinated = count;
                } else if (status == "En attente") {
                    pending = count;
                }
            }
        } else {
            qDebug() << "Error calculating vaccination status:" << query.lastError().text();
            ui->Pat_Label_Stats->setText("❌ Erreur lors du calcul des statuts vaccinaux: " + query.lastError().text());
            return;
        }

        // Step 3: Calculate percentages
        qreal pVaccinated = (tot > 0) ? (vaccinated / tot) : 0;
        qreal pNotVaccinated = (tot > 0) ? (notVaccinated / tot) : 0;
        qreal pPending = (tot > 0) ? (pending / tot) : 0;

        // Step 4: Create the pie series
        QPieSeries *series = new QPieSeries();
        series->append("Vacciné", pVaccinated);
        series->append("Non Vacciné", pNotVaccinated);
        series->append("En attente", pPending);

        // Set labels for each slice
        for (QPieSlice *slice : series->slices()) {
            slice->setLabelVisible();
            slice->setLabel(QString("%1: %2%").arg(slice->label()).arg(slice->percentage() * 100, 0, 'f', 1));
        }

        // Step 5: Create the chart
        QChart *chart = new QChart();
        chart->addSeries(series);
        chart->setTitle("Statistiques des Patients par Statut Vaccinal");
        chart->legend()->show();
        chart->setAnimationOptions(QChart::AllAnimations);
        chart->setTheme(QChart::ChartThemeQt);

        // Step 6: Create the chart view
        chartView = new QChartView(chart, ui->Pat_Label_Stats);
        chartView->setRenderHint(QPainter::Antialiasing);
        chartView->setMinimumSize(570, 570);

        // Add the chart view to the layout
        QVBoxLayout *layout = new QVBoxLayout(ui->Pat_Label_Stats);
        layout->addWidget(chartView);
        ui->Pat_Label_Stats->setLayout(layout);

        chartView->show();

}
void MainWindow::clearSpLabelStats() {
    // Check if the label has a layout
    if (ui->Pat_Label_Stats->layout()) {
        QLayout *layout = ui->Pat_Label_Stats->layout();

        // Remove all widgets from the layout
        QLayoutItem *item;
        while ((item = layout->takeAt(0)) != nullptr) {
            if (item->widget()) {
                delete item->widget(); // Delete the widget (e.g., chart view)
            }
            delete item; // Delete the layout item
        }

        // Delete the layout itself
        delete layout;
    }

    // Clear any text in the label
    ui->Pat_Label_Stats->setText("");
}

void MainWindow::highlightDates() {
    QSqlQuery query;
    // Query the PATIENT table for DATE_VACCIN
    if (query.exec("SELECT DATE_VACCIN FROM PATIENT")) {
        while (query.next()) {
            // Retrieve the date as a string and convert to QDateTime
            QString dateVaccinString = query.value(0).toString();
            QDateTime dateVaccin = QDateTime::fromString(dateVaccinString, Qt::ISODate);
            QDate dateVaccinDate = dateVaccin.date();

            // Highlight DATE_VACCIN in red (similar to DATE_FIN in the sponsor function)
            if (dateVaccinDate.isValid()) {
                QTextCharFormat format;
                format.setBackground(QBrush(QColor("#a35941"))); // Red background (using the same red as DATE_FIN in the sponsor function)
                format.setForeground(QBrush(Qt::black));
                format.setFontWeight(QFont::Bold);
                ui->calendarWidget_pat->setDateTextFormat(dateVaccinDate, format);
            }
        }
    } else {
        // Handle query failure
        qDebug() << "Error executing query:" << query.lastError().text();
    }
}

void MainWindow::on_calendarWidget_pat_selectionChanged()
{
    QDate selectedDate = ui->calendarWidget_pat->selectedDate();
    QSqlQuery query;
    bool dateFound = false;

    // Query the PATIENT table for all columns
    if (query.exec("SELECT * FROM PATIENT")) {
        while (query.next()) {
            // Map the columns based on the PATIENT table structure
            QString idPatient = query.value(0).toString(); // ID_PATIENT
            QString nom = query.value(1).toString(); // NOM
            QString prenom = query.value(2).toString(); // PRENOM
            QDate dateNaissance = query.value(3).toDate(); // DATE_NAISSSANCE
            QString sexe = query.value(4).toString(); // SEXE
            QString adresse = query.value(5).toString(); // ADRESSE
            QString telephone = query.value(6).toString(); // TELEPHONE
            QString email = query.value(7).toString(); // EMAIL
            QString groupeSanguin = query.value(8).toString(); // GROUPE_SANGUIN
            QString statutVaccinal = query.value(9).toString(); // STATUT_VACCINAL
            QDate dateVaccin = query.value(10).toDate(); // DATE_VACCIN
            QString idProjetTesteur = query.value(11).toString(); // ID_PROJET_TESTEUR

            // Check if the selected date matches DATE_VACCIN
            if (selectedDate == dateVaccin) {
                ui->Calendrier_Info->setText(
                    "Patient Details:<br>"
                    "<b>ID Patient:</b> " + idPatient + "<br>" +
                    "<b>Name:</b> " + nom + " " + prenom + "<br>" +
                    "<b>Date of Birth:</b> " + dateNaissance.toString("dd/MM/yyyy") + "<br>" +
                    "<b>Sex:</b> " + sexe + "<br>" +
                    "<b>Address:</b> " + adresse + "<br>" +
                    "<b>Phone:</b> " + telephone + "<br>" +
                    "<b>Email:</b> " + email + "<br>" +
                    "<b>Blood Group:</b> " + groupeSanguin + "<br>" +
                    "<b>Vaccination Status:</b> " + statutVaccinal + "<br>" +
                    "<b>Vaccination Date:</b> " + dateVaccin.toString("dd/MM/yyyy") + "<br>" +
                    "<b>Project Tester ID:</b> " + idProjetTesteur + "<br>"
                );
                dateFound = true;
                break;
            }
        }
        if (!dateFound) {
            ui->Calendrier_Info->setText("No patient found for this date.");
        }
    } else {
        // Handle query failure
        ui->Calendrier_Info->setText("Error retrieving patient data.");
        qDebug() << "Error executing query:" << query.lastError().text();
    }
}




#include <QMessageBox>


#include <QTimer>

#include <QMessageBox>
#include <QTimer>

void MainWindow::sendVaccinationReminders() {
    QSqlQuery query;
    QDate tomorrowDate = QDate::currentDate().addDays(1);

    if (query.exec("SELECT EMAIL, NOM, PRENOM, DATE_VACCIN FROM PATIENT WHERE DATE_VACCIN IS NOT NULL")) {
        while (query.next()) {
            QString email = query.value(0).toString().trimmed();
            QString nom = query.value(1).toString();
            QString prenom = query.value(2).toString();
            QDate vaccinationDate = QDateTime::fromString(query.value(3).toString(), Qt::ISODate).date();

            if (vaccinationDate != tomorrowDate) continue;

            if (!email.contains("@") || email.isEmpty()) {
                QMessageBox::warning(this, tr("Qt Simple SMTP client"), tr("Invalid email address: %1\n\n").arg(email));
                continue;
            }

            QString subject = "Rappel de Vaccination";
            QString message = QString(
                "Cher(e) %1 %2,\r\n\r\n"
                "Nous vous rappelons que votre vaccination est prevue pour demain, le %3.\r\n"
                "Veuillez vous presenter a l'heure convenue.\r\n\r\n"
                "Cordialement,\r\n"
                "L'equipe medicale"
            ).arg(prenom).arg(nom).arg(vaccinationDate.toString("dd/MM/yyyy"));

            Smtp* smtp = new Smtp("takwabelghith03@gmail.com", "xndxbwaviyzgiphh", "smtp.gmail.com", 465);
            connect(smtp, &Smtp::status, this, [this, email, smtp](const QString &status) {
                QTimer::singleShot(0, this, [this, status]() {
                    if (status == "Message sent") {
                        QMessageBox::information(this, tr("Qt Simple SMTP client"), tr("Message sent!\n\n"));
                    } else {
                        QMessageBox::warning(this, tr("Qt Simple SMTP client"), tr("Failed to send email: %1\n\n").arg(status));
                    }
                });
                smtp->deleteLater();
            });

            smtp->sendMail("takwabelghith03@gmail.com", email, subject, message);
        }
    } else {
        QMessageBox::warning(this, tr("Qt Simple SMTP client"), tr("Error executing query: %1\n\n").arg(query.lastError().text()));
    }
}
