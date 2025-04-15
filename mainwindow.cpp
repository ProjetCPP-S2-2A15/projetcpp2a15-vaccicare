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
#include <QIntValidator>
#include <QStandardItemModel>

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
    QCheckBox {
        color: #ecf0f1;
        padding: 5px;
    }
    QCheckBox::indicator {
        width: 16px;
        height: 16px;
        background-color: #34495e;
        border: 1px solid #1abc9c;
        border-radius: 3px;
    }
    QCheckBox::indicator:checked {
        background-color: #27ae60;
        border: 1px solid #1abc9c;
    }
    )");

    // Assurer que le mot de passe est masqué
    ui->lineEdit_14->setEchoMode(QLineEdit::Password);

    // Restreindre les champs numériques pour les médecins
    ui->lineEdit_3->setValidator(new QIntValidator(this));
    ui->lineEdit_9->setValidator(new QIntValidator(this));

    // Connexions pour les médecins
    connect(ui->pushButton_3, &QPushButton::clicked, this, &MainWindow::on_pushButton3_ajouter_clicked);
    connect(ui->pushButton_2, &QPushButton::clicked, this, &MainWindow::on_pushButton_2_supprimer_clicked);
    connect(ui->pushButton_4, &QPushButton::clicked, this, &MainWindow::on_pushButton_modifier_clicked);
    connect(ui->lineEdit_rechercher, &QLineEdit::textChanged, this, &MainWindow::on_lineEdit_rechercher_textChanged);
    connect(ui->comboBox_tri, &QComboBox::currentTextChanged, this, &MainWindow::on_comboBox_tri_currentTextChanged);
    connect(ui->pushButton_pdf, &QPushButton::clicked, this, &MainWindow::on_pushButton_pdf_clicked);
    connect(ui->pushButton, &QPushButton::clicked, this, &MainWindow::on_pushButton_clicked);

    // Vérifier si pushButton_5 existe
    if (!ui->pushButton_5) {
        qDebug() << "Erreur : pushButton_5 n'existe pas. Vérifiez l'objectName du bouton Ressources dans mainwindow.ui.";
    } else {
        qDebug() << "pushButton_5 trouvé.";
    }

    // Définir l'onglet de login (tab) comme actif par défaut
    ui->tabWidget->setCurrentIndex(0); // Index 0: tab (Login)

    // Masquer tous les onglets sauf le login au démarrage
    ui->tabWidget->setTabEnabled(1, false); // Index 1: tab_2 (Médecins)
    ui->tabWidget->setTabEnabled(2, false); // Index 2: tab_3
    ui->tabWidget->setTabEnabled(3, false); // Index 3: tab_4 (Ressources)

    // Vérifier le nombre d'onglets pour déboguer
    qDebug() << "Nombre d'onglets dans tabWidget :" << ui->tabWidget->count();
    for (int i = 0; i < ui->tabWidget->count(); ++i) {
        qDebug() << "Onglet" << i << ":" << ui->tabWidget->tabText(i) << "ObjectName:" << ui->tabWidget->widget(i)->objectName();
    }

    // Initialiser le modèle complet des ressources
    fullResourceModel = new QStandardItemModel(this);
    fullResourceModel->setHorizontalHeaderLabels({"ID", "Nom de la ressource", "Quantité"});

    // Ajouter les données de test (Masques, Gants, Désinfectant, Seringues, Bandages)
    QList<QStandardItem *> row1;
    row1 << new QStandardItem("1") << new QStandardItem("Masques") << new QStandardItem("100");
    fullResourceModel->appendRow(row1);

    QList<QStandardItem *> row2;
    row2 << new QStandardItem("2") << new QStandardItem("Gants") << new QStandardItem("200");
    fullResourceModel->appendRow(row2);

    QList<QStandardItem *> row3;
    row3 << new QStandardItem("3") << new QStandardItem("Désinfectant") << new QStandardItem("50");
    fullResourceModel->appendRow(row3);

    QList<QStandardItem *> row4;
    row4 << new QStandardItem("4") << new QStandardItem("Seringues") << new QStandardItem("150");
    fullResourceModel->appendRow(row4);

    QList<QStandardItem *> row5;
    row5 << new QStandardItem("5") << new QStandardItem("Bandages") << new QStandardItem("300");
    fullResourceModel->appendRow(row5);

    // Vérifier si tableView_3 existe
    if (!ui->tableView_3) {
        qDebug() << "Erreur : tableView_3 n'existe pas dans l'onglet tab_4. Vérifiez mainwindow.ui.";
    } else {
        qDebug() << "tableView_3 trouvé, initialisation...";
        // Définir le modèle initial (toutes les ressources visibles par défaut)
        ui->tableView_3->setModel(fullResourceModel);
        ui->tableView_3->resizeColumnsToContents();
    }

    // Vérifier si les cases à cocher existent
    if (!ui->checkBox || !ui->checkBox_2 || !ui->checkBox_3 || !ui->checkBox_4 || !ui->checkBox_5) {
        qDebug() << "Erreur : Une ou plusieurs cases à cocher (checkBox, checkBox_2, checkBox_3, checkBox_4, checkBox_5) n'existent pas. Vérifiez mainwindow.ui.";
    } else {
        // Cocher toutes les cases par défaut
        ui->checkBox->setChecked(true);
        ui->checkBox_2->setChecked(true);
        ui->checkBox_3->setChecked(true);
        ui->checkBox_4->setChecked(true);
        ui->checkBox_5->setChecked(true);

        // Connecter les cases à cocher au slot de filtrage
        connect(ui->checkBox, &QCheckBox::stateChanged, this, &MainWindow::updateResourceFilter);
        connect(ui->checkBox_2, &QCheckBox::stateChanged, this, &MainWindow::updateResourceFilter);
        connect(ui->checkBox_3, &QCheckBox::stateChanged, this, &MainWindow::updateResourceFilter);
        connect(ui->checkBox_4, &QCheckBox::stateChanged, this, &MainWindow::updateResourceFilter);
        connect(ui->checkBox_5, &QCheckBox::stateChanged, this, &MainWindow::updateResourceFilter);
    }

    // Vérifier si pushButton_6 existe
    if (!ui->pushButton_6) {
        qDebug() << "Erreur : pushButton_6 n'existe pas dans tab_4. Vérifiez l'objectName du bouton Générer dans mainwindow.ui.";
    } else {
        qDebug() << "pushButton_6 trouvé.";
        // Connecter le bouton Générer au slot
        connect(ui->pushButton_6, &QPushButton::clicked, this, &MainWindow::on_pushButton_6_clicked);
    }
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_pushButton_6_clicked()
{
    qDebug() << "Bouton Générer cliqué.";
    QMessageBox::information(this, "Succès", "Ressources générées avec succès !");
}

void MainWindow::updateResourceFilter()
{
    qDebug() << "Mise à jour du filtre des ressources...";

    // Créer un nouveau modèle pour les ressources filtrées
    QStandardItemModel *filteredModel = new QStandardItemModel(this);
    filteredModel->setHorizontalHeaderLabels({"ID", "Nom de la ressource", "Quantité"});

    // Parcourir le modèle complet et ajouter les lignes correspondant aux cases cochées
    bool masquesChecked = ui->checkBox->isChecked();
    bool gantsChecked = ui->checkBox_2->isChecked();
    bool desinfectantChecked = ui->checkBox_3->isChecked();
    bool seringuesChecked = ui->checkBox_4->isChecked();
    bool bandagesChecked = ui->checkBox_5->isChecked();

    for (int row = 0; row < fullResourceModel->rowCount(); ++row) {
        QString resourceName = fullResourceModel->data(fullResourceModel->index(row, 1)).toString();
        bool shouldShow = false;

        if (resourceName == "Masques" && masquesChecked) {
            shouldShow = true;
        } else if (resourceName == "Gants" && gantsChecked) {
            shouldShow = true;
        } else if (resourceName == "Désinfectant" && desinfectantChecked) {
            shouldShow = true;
        } else if (resourceName == "Seringues" && seringuesChecked) {
            shouldShow = true;
        } else if (resourceName == "Bandages" && bandagesChecked) {
            shouldShow = true;
        }

        if (shouldShow) {
            QList<QStandardItem *> rowItems;
            for (int col = 0; col < fullResourceModel->columnCount(); ++col) {
                rowItems << new QStandardItem(fullResourceModel->data(fullResourceModel->index(row, col)).toString());
            }
            filteredModel->appendRow(rowItems);
        }
    }

    // Mettre à jour tableView_3 avec le modèle filtré
    ui->tableView_3->setModel(filteredModel);
    ui->tableView_3->resizeColumnsToContents();

    // Afficher un message si aucune ressource n'est sélectionnée
    if (filteredModel->rowCount() == 0) {
        QMessageBox::information(this, "Information", "Aucune ressource sélectionnée à afficher.");
    }

    qDebug() << "Filtre appliqué. Nombre de lignes affichées :" << filteredModel->rowCount();
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
        ui->tabWidget->setTabEnabled(1, true); // Activer tab_2 (Médecins)
        ui->tabWidget->setTabEnabled(2, true); // Activer tab_3
        ui->tabWidget->setTabEnabled(3, true); // Activer tab_4 (Ressources)
        ui->tabWidget->setCurrentIndex(1); // Basculer vers tab_2 après connexion
        ui->tabWidget->setTabEnabled(0, false); // Désactiver tab (Login)
        refreshTableView();
    } else {
        attempts--;
        QMessageBox::warning(this, "Erreur", QString("Login ou mot de passe incorrect ! %1 tentative(s) restante(s).").arg(attempts));
        ui->lineEdit_14->clear();
        ui->lineEdit->setFocus();
    }
}

void MainWindow::on_pushButton_5_clicked()
{
    qDebug() << "Bouton Ressources cliqué.";
    qDebug() << "Nombre d'onglets :" << ui->tabWidget->count();
    qDebug() << "Onglet actif avant redirection :" << ui->tabWidget->currentIndex() << " (" << ui->tabWidget->tabText(ui->tabWidget->currentIndex()) << ")";
    for (int i = 0; i < ui->tabWidget->count(); ++i) {
        qDebug() << "Onglet" << i << ":" << ui->tabWidget->tabText(i) << "Enabled:" << ui->tabWidget->isTabEnabled(i);
        qDebug() << "  ObjectName de l'onglet" << i << ":" << ui->tabWidget->widget(i)->objectName();
    }

    const int targetIndex = 3; // tab_4 est à l'index 3



    // Vérifier si l'onglet est désactivé, et l'activer si nécessaire
    if (!ui->tabWidget->isTabEnabled(targetIndex)) {
        qDebug() << "L'onglet tab_4 (index" << targetIndex << ") est désactivé, activation...";
        ui->tabWidget->setTabEnabled(targetIndex, true);
    }

    // Rediriger vers l'onglet tab_4
    ui->tabWidget->setCurrentIndex(targetIndex);

    qDebug() << "Redirection effectuée.";
    qDebug() << "Onglet actif après redirection :" << ui->tabWidget->currentIndex() << " (" << ui->tabWidget->tabText(ui->tabWidget->currentIndex()) << ")";

    // Vérifier si tableView_3 est affiché
    if (ui->tableView_3) {
        qDebug() << "tableView_3 est affiché dans l'onglet tab_4.";
        ui->tableView_3->resizeColumnsToContents();
    } else {
        qDebug() << "Erreur : tableView_3 n'est pas trouvé dans l'onglet tab_4.";
    }
}

void MainWindow::on_pushButton3_ajouter_clicked()
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
    QDesktopServices::openUrl(QUrl::fromLocalFile(filePath));
}
