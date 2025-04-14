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

    // Appliquer un style CSS
    this->setStyleSheet(R"(
    QWidget {
        background-color: #2c3e50;
        font-family: 'Segoe UI', sans-serif;
        font-size: 13px;
        color: #ecf0f1;
    }
    QLineEdit {
        background-color: #34495e;
        border: 1px solid #1abc9c;
        border-radius: 6px;
        padding: 6px;
        color: #ecf0f1;
    }
    QLineEdit:focus {
        border: 1px solid #3498db;
        background-color: #3c5a72;
    }
    QComboBox {
        background-color: #34495e;
        color: #ecf0f1;
        border: 1px solid #1abc9c;
        border-radius: 6px;
        padding: 6px;
    }
    QComboBox:hover {
        border: 1px solid #3498db;
    }
    QTableView {
        background-color: #3c5a72;
        border: 1px solid #1abc9c;
        border-radius: 6px;
        gridline-color: #16a085;
        color: #ecf0f1;
        selection-background-color: #27ae60;
    }
    QHeaderView::section {
        background-color: #2980b9;
        color: #ecf0f1;
        padding: 5px;
        border: 1px solid #1abc9c;
    }
    QPushButton#pushButton_2 {
        background-color: #e74c3c;
        color: #ecf0f1;
        border: none;
        border-radius: 6px;
        padding: 8px 16px;
        font-weight: bold;
    }
    QPushButton#pushButton_2:hover {
        background-color: #c0392b;
    }
    QPushButton#pushButton_pdf {
        background-color: #27ae60;
        color: #ecf0f1;
        border: none;
        border-radius: 6px;
        padding: 8px 16px;
        font-weight: bold;
    }
    QPushButton#pushButton_pdf:hover {
        background-color: #1e8449;
    }
    QPushButton {
        background-color: #3498db;
        color: #ecf0f1;
        border: none;
        border-radius: 6px;
        padding: 8px 16px;
        font-weight: bold;
    }
    QPushButton:hover {
        background-color: #2980b9;
    }
    QTabWidget::pane {
        border: none;
    }
    QTabBar::tab {
        background: #16a085;
        border: 1px solid #1abc9c;
        padding: 6px 12px;
        border-top-left-radius: 6px;
        border-top-right-radius: 6px;
        color: #ecf0f1;
    }
    QTabBar::tab:selected {
        background: #1abc9c;
        color: #2c3e50;
    }
    )");

    // Assurer que le mot de passe est masqué
    ui->lineEdit_14->setEchoMode(QLineEdit::Password);


    // Connexions pour les autres fonctionnalités
    connect(ui->pushButton_3, &QPushButton::clicked, this, &MainWindow::on_pushButton3_ajouter_clicked);
    connect(ui->pushButton_2, &QPushButton::clicked, this, &MainWindow::on_pushButton_2_supprimer_clicked);
    connect(ui->pushButton_4, &QPushButton::clicked, this, &MainWindow::on_pushButton_modifier_clicked);
    connect(ui->lineEdit_rechercher, &QLineEdit::textChanged, this, &MainWindow::on_lineEdit_rechercher_textChanged);
    connect(ui->comboBox_tri, &QComboBox::currentTextChanged, this, &MainWindow::on_comboBox_tri_currentTextChanged);
    connect(ui->pushButton_pdf, &QPushButton::clicked, this, &MainWindow::on_pushButton_pdf_clicked);

    // Connexion pour le bouton de login
    connect(ui->pushButton, &QPushButton::clicked, this, &MainWindow::on_pushButton_clicked);

    // Masquer les autres onglets au démarrage (sauf l'onglet de login, qui est à l'index 0)
    ui->tabWidget->setTabEnabled(1, false); // Désactiver "Tab 2"
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_pushButton_clicked()
{
    if (attempts <= 0) {
        QMessageBox::critical(this, "Erreur", "Trop de tentatives échouées. L'application va se fermer.");
        close();
        return;
    }

    QString login = ui->lineEdit->text().trimmed();
    QString password = ui->lineEdit_14->text();

    if (login.isEmpty() || password.isEmpty()) {
        QMessageBox::warning(this, "Erreur", "Veuillez remplir tous les champs !");
        return;
    }

    Medecin medecin;
    if (medecin.verifierLogin(login, password)) {
        QMessageBox::information(this, "Succès", "Connexion réussie !");
        // Activer les autres onglets après connexion
        ui->tabWidget->setTabEnabled(1, true); // Activer "Tab 2"
        ui->tabWidget->setCurrentIndex(1); // Basculer vers "Tab 2"
        ui->tabWidget->setTabEnabled(0, false); // Masquer l'onglet de login
        refreshTableView(); // Rafraîchir la table
    } else {
        attempts--;
        QMessageBox::warning(this, "Erreur", QString("Login ou mot de passe incorrect ! %1 tentative(s) restante(s).").arg(attempts));
        ui->lineEdit_14->clear();
        ui->lineEdit->setFocus();
    }
}

// Le reste de ton code (on_pushButton3_ajouter_clicked, on_pushButton_2_supprimer_clicked, etc.) reste inchangé
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

    if (!idOk || !expOk) {
        QMessageBox::warning(this, "Erreur de saisie", "Veuillez entrer un ID ou une expérience valide !");
        return;
    }

    Medecin M(id_medecin, nom, specialite, grade, email, telephone, experience, statut, disponibilite, login, mot_de_passe);

    bool test = M.ajouter();
    if (test) {
        QMessageBox::information(this, "Succès", "Médecin ajouté avec succès !");
        refreshTableView();
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
        refreshTableView();
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
            refreshTableView();
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
    Q_UNUSED(text);
    refreshTableView();
}

void MainWindow::on_comboBox_tri_currentTextChanged(const QString &text)
{
    Q_UNUSED(text);
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
    QAbstractItemModel* model = ui->tableView->model();
    if (!model || model->rowCount() == 0) {
        QMessageBox::warning(this, "Erreur", "Aucune donnée à exporter dans le PDF.");
        return;
    }

    QString filePath = QFileDialog::getSaveFileName(this, tr("Enregistrer le PDF"), "", tr("Fichiers PDF (*.pdf)"));
    if (filePath.isEmpty()) {
        return;
    }

    if (!filePath.endsWith(".pdf", Qt::CaseInsensitive)) {
        filePath += ".pdf";
    }

    QPdfWriter pdfWriter(filePath);
    QPageLayout layout(QPageSize(QPageSize::A4), QPageLayout::Landscape, QMarginsF(20, 20, 20, 20), QPageLayout::Millimeter);
    pdfWriter.setPageLayout(layout);

    QPainter painter(&pdfWriter);
    painter.setRenderHint(QPainter::Antialiasing);

    const int pageWidth = pdfWriter.width();
    const int pageHeight = pdfWriter.height();
    const int margin = 20 * 72 / 25.4;
    const int rowHeight = 30;
    const int columnCount = model->columnCount();
    const int rowCount = model->rowCount();

    QFont font("Arial", 7);
    painter.setFont(font);
    QFontMetrics fontMetrics(font, &pdfWriter);

    QVector<int> columnWidths(columnCount, 0);
    int totalContentWidth = 0;
    for (int col = 0; col < columnCount; ++col) {
        QString headerText = model->headerData(col, Qt::Horizontal).toString();
        int headerWidth = fontMetrics.boundingRect(headerText).width() + 20;

        int maxCellWidth = headerWidth;
        for (int row = 0; row < rowCount; ++row) {
            QString cellText = model->data(model->index(row, col)).toString();
            int cellWidth = fontMetrics.boundingRect(cellText).width() + 20;
            maxCellWidth = qMax(maxCellWidth, cellWidth);
        }
        columnWidths[col] = maxCellWidth;
        totalContentWidth += maxCellWidth;
    }

    int availableWidth = pageWidth - 2 * margin;
    if (totalContentWidth > availableWidth) {
        double scaleFactor = static_cast<double>(availableWidth) / totalContentWidth;
        for (int col = 0; col < columnCount; ++col) {
            columnWidths[col] = static_cast<int>(columnWidths[col] * scaleFactor);
        }
        totalContentWidth = availableWidth;
    }

    qDebug() << "Column widths:" << columnWidths;
    qDebug() << "Total content width:" << totalContentWidth << "Available width:" << availableWidth;

    QFont titleFont("Arial", 12, QFont::Bold);
    painter.setFont(titleFont);
    painter.setPen(Qt::black);
    QString title = "Liste des Médecins";
    int titleHeight = fontMetrics.height();
    painter.drawText(margin, margin, title);

    painter.setFont(font);

    int y = margin + titleHeight + 20;

    painter.setPen(Qt::black);
    painter.setBrush(QBrush(Qt::lightGray));
    int x = margin;
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
    for (int row = 0; row < rowCount; ++row) {
        if (y + rowHeight > pageHeight - margin) {
            pdfWriter.newPage();
            y = margin;
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

        x = margin;
        for (int col = 0; col < columnCount; ++col) {
            painter.drawRect(x, y, columnWidths[col], rowHeight);
            QString cellText = model->data(model->index(row, col)).toString();
            QString displayText = fontMetrics.elidedText(cellText, Qt::ElideRight, columnWidths[col] - 20);
            int textY = y + (rowHeight - fontMetrics.height()) / 2 + fontMetrics.ascent();
            painter.drawText(x + 10, textY, displayText);
            x += columnWidths[col];
        }
        y += rowHeight;
    }

    painter.end();

    QMessageBox::information(this, "Succès", "Le PDF a été généré avec succès !");
    qDebug() << "PDF generated at:" << filePath;

    QDesktopServices::openUrl(QUrl::fromLocalFile(filePath));
}
