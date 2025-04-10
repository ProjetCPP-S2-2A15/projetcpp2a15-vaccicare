#include "MainWindow.h"
#include "ui_MainWindow.h"
#include "medecin.h"
#include "connection.h"
#include <QMessageBox>
#include <QDebug>
#include <QSqlQueryModel>
#include <QTimer>
#include <QSqlQuery>
#include <QPdfWriter>
#include <QPainter>
#include <QFileDialog>
#include <QDesktopServices>
#include <QUrl>
#include <QPageLayout>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    connect(ui->pushButton_3, &QPushButton::clicked, this, &MainWindow::on_pushButton3_ajouter_clicked);
    connect(ui->pushButton_2, &QPushButton::clicked, this, &MainWindow::on_pushButton_2_supprimer_clicked);
    connect(ui->pushButton_4, &QPushButton::clicked, this, &MainWindow::on_pushButton_modifier_clicked);
    connect(ui->lineEdit_rechercher, &QLineEdit::textChanged, this, &MainWindow::on_lineEdit_rechercher_textChanged);
    connect(ui->comboBox_tri, &QComboBox::currentTextChanged, this, &MainWindow::on_comboBox_tri_currentTextChanged);
    connect(ui->pushButton_pdf, &QPushButton::clicked, this, &MainWindow::on_pushButton_pdf_clicked);



    // Delay the initial table view refresh until the UI is fully initialized
    QTimer::singleShot(100, this, &MainWindow::refreshTableView);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_pushButton3_ajouter_clicked()
{
    qDebug() << "Bouton Ajouter pressé !";

    bool idOk, expOk;
    int id_medecin = ui->lineEdit_3->text().toInt(&idOk);
    QString nom = ui->lineEdit_4->text();
    QString specialite = ui->lineEdit_5->text();
    QString grade = ui->lineEdit_7->text();
    QString email = ui->lineEdit_6->text();
    QString telephone = ui->lineEdit_10->text();
    QString disponibilite = ui->lineEdit_12->text();
    int experience = ui->lineEdit_9->text().toInt(&expOk);
    QString statut = ui->lineEdit_8->text();
    QString login = ui->lineEdit_13->text();
    QString mot_de_passe = ui->lineEdit_11->text();

    // Validate inputs
    if (!idOk || !expOk) {
        QMessageBox::warning(this, "Erreur de saisie", "Veuillez entrer un ID ou une expérience valide !");
        return;
    }

    // Create a Medecin object
    Medecin M(id_medecin, nom, specialite, grade, email, telephone, experience, statut, disponibilite, login, mot_de_passe);

    // Add to the database
    bool test = M.ajouter();
    if (test) {
        QMessageBox::information(this, "Succès", "Médecin ajouté avec succès !");
        refreshTableView(); // Refresh the table view
    } else {
        QMessageBox::critical(this, "Erreur", "Ajout non effectué !");
        qDebug() << "Erreur lors de l'ajout du médecin.";
    }
}

void MainWindow::on_pushButton_2_supprimer_clicked()
{
    bool idOk;
    int id_cherch = ui->lineEdit_2->text().toInt(&idOk);

    if (!idOk) {
        QMessageBox::warning(this, "Erreur", "Veuillez entrer un ID valide !");
        return;
    }

    Medecin M;
    bool success = M.supprimer(id_cherch);
    if (success) {
        QMessageBox::information(this, "Succès", "Médecin supprimé avec succès !");
        qDebug() << "Médecin supprimé avec succès.";
        refreshTableView(); // Refresh the table view
    } else {
        QMessageBox::critical(this, "Erreur", "Échec de la suppression du médecin.");
        qDebug() << "Erreur lors de la suppression du médecin.";
    }
}

void MainWindow::on_pushButton_modifier_clicked()
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
        // Update with new values from the UI
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
            refreshTableView(); // Refresh the table view
        } else {
            QMessageBox::critical(this, "Erreur", "Échec de la modification du médecin.");
            qDebug() << "Erreur lors de la modification du médecin.";
        }
    } else {
        QMessageBox::warning(this, "Erreur", "Médecin non trouvé avec cet ID.");
    }
}

void MainWindow::on_lineEdit_rechercher_textChanged(const QString &text)
{
    Q_UNUSED(text); // We don't use the text directly here, but it's passed by the signal
    refreshTableView();
}

void MainWindow::on_comboBox_tri_currentTextChanged(const QString &text)
{
    Q_UNUSED(text); // We don't use the text directly here, but it's passed by the signal
    refreshTableView();
}

void MainWindow::refreshTableView()
{
    Medecin medecin;
    QString searchText = ui->lineEdit_rechercher->text().trimmed();
    QString criterion = ui->comboBox->currentText();
    QString sortCriterion = ui->comboBox_tri->currentText();
    qDebug() << "Refreshing table view with searchText:" << searchText << "and criterion:" << criterion << "and sortCriterion:" << sortCriterion;

    QSqlQueryModel* model = medecin.afficher(searchText, criterion, sortCriterion);

    if (model) {
        ui->tableView->setModel(model);

        // Set column headers based on Medecin attributes
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

        ui->tableView->resizeColumnsToContents();
        qDebug() << "Table view updated successfully. Rows displayed:" << model->rowCount();

        // Show a message if no doctors are found
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

void MainWindow::on_pushButton_pdf_clicked()
{
    // Get the model from the table view
    QAbstractItemModel* model = ui->tableView->model();
    if (!model || model->rowCount() == 0) {
        QMessageBox::warning(this, "Erreur", "Aucune donnée à exporter dans le PDF.");
        return;
    }

    // Prompt the user to select a file location
    QString filePath = QFileDialog::getSaveFileName(this, tr("Enregistrer le PDF"), "", tr("Fichiers PDF (*.pdf)"));
    if (filePath.isEmpty()) {
        return; // User canceled the dialog
    }

    // Ensure the file has a .pdf extension
    if (!filePath.endsWith(".pdf", Qt::CaseInsensitive)) {
        filePath += ".pdf";
    }

    // Create a PDF writer and set to landscape mode
    QPdfWriter pdfWriter(filePath);
    QPageLayout layout(QPageSize(QPageSize::A4), QPageLayout::Landscape, QMarginsF(20, 20, 20, 20), QPageLayout::Millimeter);
    pdfWriter.setPageLayout(layout);

    // Create a painter for the PDF
    QPainter painter(&pdfWriter);
    painter.setRenderHint(QPainter::Antialiasing);

    // Define dimensions for the table (in points, since PDF uses 72 DPI)
    const int pageWidth = pdfWriter.width(); // Width in points (now wider in landscape)
    const int pageHeight = pdfWriter.height(); // Height in points (shorter in landscape)
    const int margin = 20 * 72 / 25.4; // Convert 20mm to points (1 inch = 25.4mm, 1 inch = 72 points)
    const int rowHeight = 30; // Height of each row in points
    const int columnCount = model->columnCount();
    const int rowCount = model->rowCount();

    // Set font for the table
    QFont font("Arial", 7);
    painter.setFont(font);
    QFontMetrics fontMetrics(font, &pdfWriter);

    // Calculate column widths based on content
    QVector<int> columnWidths(columnCount, 0);
    int totalContentWidth = 0;
    for (int col = 0; col < columnCount; ++col) {
        // Get the header width
        QString headerText = model->headerData(col, Qt::Horizontal).toString();
        int headerWidth = fontMetrics.boundingRect(headerText).width() + 20; // Add padding

        // Check the width of each cell in this column
        int maxCellWidth = headerWidth;
        for (int row = 0; row < rowCount; ++row) {
            QString cellText = model->data(model->index(row, col)).toString();
            int cellWidth = fontMetrics.boundingRect(cellText).width() + 20; // Add padding
            maxCellWidth = qMax(maxCellWidth, cellWidth);
        }
        columnWidths[col] = maxCellWidth;
        totalContentWidth += maxCellWidth;
    }

    // Scale column widths to fit the page if necessary
    int availableWidth = pageWidth - 2 * margin;
    if (totalContentWidth > availableWidth) {
        double scaleFactor = static_cast<double>(availableWidth) / totalContentWidth;
        for (int col = 0; col < columnCount; ++col) {
            columnWidths[col] = static_cast<int>(columnWidths[col] * scaleFactor);
        }
        totalContentWidth = availableWidth;
    }

    // Debug column widths
    qDebug() << "Column widths:" << columnWidths;
    qDebug() << "Total content width:" << totalContentWidth << "Available width:" << availableWidth;

    // Title
    QFont titleFont("Arial", 12, QFont::Bold);
    painter.setFont(titleFont);
    painter.setPen(Qt::black);
    QString title = "Liste des Médecins";
    int titleHeight = fontMetrics.height();
    painter.drawText(margin, margin, title);

    // Reset font for table
    painter.setFont(font);

    // Starting Y position for the table
    int y = margin + titleHeight + 20;

    // Draw table headers
    painter.setPen(Qt::black);
    painter.setBrush(QBrush(Qt::lightGray));
    int x = margin;
    for (int col = 0; col < columnCount; ++col) {
        painter.drawRect(x, y, columnWidths[col], rowHeight);
        QString headerText = model->headerData(col, Qt::Horizontal).toString();
        // Truncate text if too long
        QString displayText = fontMetrics.elidedText(headerText, Qt::ElideRight, columnWidths[col] - 20);
        // Center text vertically
        int textY = y + (rowHeight - fontMetrics.height()) / 2 + fontMetrics.ascent();
        painter.drawText(x + 10, textY, displayText);
        x += columnWidths[col];
    }
    y += rowHeight;

    // Draw table data
    painter.setBrush(Qt::NoBrush);
    for (int row = 0; row < rowCount; ++row) {
        // Check for page break
        if (y + rowHeight > pageHeight - margin) {
            pdfWriter.newPage();
            y = margin;
            // Redraw headers on new page
            painter.setBrush(QBrush(Qt::lightGray));
            x = margin;
            for (int col = 0; col < columnCount; ++col) {
                painter.drawRect(x, y, columnWidths[col], rowHeight);
                QString headerText = model->headerData(col, Qt::Horizontal).toString();
                QString displayText = fontMetrics.elidedText(headerText, Qt::ElideRight, columnWidths[col] - 20);
                int textY = y + (rowHeight - fontMetrics.height()) / 2 + fontMetrics.ascent();
                painter.drawText(x + 10, textY, displayText);
                x += columnWidths[col];
            }
            y += rowHeight;
            painter.setBrush(Qt::NoBrush);
        }

        // Draw row data
        x = margin;
        for (int col = 0; col < columnCount; ++col) {
            painter.drawRect(x, y, columnWidths[col], rowHeight);
            QString cellText = model->data(model->index(row, col)).toString();
            // Truncate text if too long
            QString displayText = fontMetrics.elidedText(cellText, Qt::ElideRight, columnWidths[col] - 20);
            // Center text vertically
            int textY = y + (rowHeight - fontMetrics.height()) / 2 + fontMetrics.ascent();
            painter.drawText(x + 10, textY, displayText);
            x += columnWidths[col];
        }
        y += rowHeight;
    }

    // End painting
    painter.end();

    // Notify the user
    QMessageBox::information(this, "Succès", "Le PDF a été généré avec succès !");
    qDebug() << "PDF generated at:" << filePath;

    // Optionally open the PDF
    QDesktopServices::openUrl(QUrl::fromLocalFile(filePath));
}
