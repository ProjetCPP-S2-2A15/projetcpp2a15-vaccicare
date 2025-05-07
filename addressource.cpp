#include "addressource.h"
#include <QSqlError>
#include <QSqlQuery>
#include <QTableWidgetItem>
#include <QDebug>
#include <QMessageBox>
#include <QFileInfo>
#include <QSqlRecord>
#include <QDir>
#include "qlineedit.h"
#include "qtimer.h"
#include "ui_addressource.h"

AddRessource::AddRessource(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::AddRessource)
{
    ui->setupUi(this);

    // Initialiser la base de données
    if (!initializeDatabase()) {
        QMessageBox::critical(this, "Erreur", "Échec de la connexion à la base de données : " + QSqlDatabase::database().lastError().text());
        return;
    }

    // Importer et afficher les noms des ressources
    QStringList resourceNames = fetchResourcesFromDB();
    if (resourceNames.isEmpty()) {
        qDebug() << "Aucune ressource trouvée ou erreur lors de la récupération.";
        QMessageBox::warning(this, "Avertissement", "Aucune ressource trouvée dans la base de données. Vérifiez le chemin ou la table.");
    } else {
        displayResourcesInTableWidget(ui->tableR, resourceNames);
    }
    connect(ui->Button2, &QPushButton::clicked, this, &AddRessource::ExitApp);
    connect(ui->Button1, &QPushButton::clicked, this, &AddRessource::notif);

}

AddRessource::~AddRessource()
{
    delete ui;
    if (QSqlDatabase::database().isOpen()) {
        QSqlDatabase::database().close();
    }
}

bool AddRessource::initializeDatabase()
{
    if (QSqlDatabase::database().isValid()) {
        qDebug() << "Database already valid, trying to open...";
        if (QSqlDatabase::database().isOpen()) {
            qDebug() << "Database already open.";
            return true;
        }
        if (QSqlDatabase::database().open()) {
            qDebug() << "Database opened successfully.";
            return true;
        } else {
            qDebug() << "Failed to open database:" << QSqlDatabase::database().lastError().text();
            return false;
        }
    }

    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
    // Chemin relatif (répertoire de l'exécutable)
    QString dbPath = QDir::currentPath() + "/vaccicare.db";
    qDebug() << "Trying database path (relative):" << dbPath;

    // Vérifier si le fichier existe
    QFileInfo dbFile(dbPath);
    if (!dbFile.exists()) {
        qDebug() << "Database file does not exist at (relative):" << dbPath;
        // Chemin absolu basé sur votre projet
        dbPath = "C:/Users/Takwa/Documents/GitHub/projetcpp2a15-vaccicare/vaccicare.db";
        qDebug() << "Trying database path (absolute):" << dbPath;
        dbFile = QFileInfo(dbPath);
        if (!dbFile.exists()) {
            qDebug() << "Database file does not exist at (absolute):" << dbPath;
            QMessageBox::warning(this, "Avertissement", "Fichier vaccicare.db introuvable. Placez-le dans le répertoire du projet ou spécifiez le chemin correct.");
            return false;
        }
    }
    db.setDatabaseName(dbPath);

    qDebug() << "Attempting to open database at:" << db.databaseName();
    if (!db.open()) {
        qDebug() << "Échec de l'ouverture de la base de données :" << db.lastError().text();
        return false;
    }
    qDebug() << "Database opened successfully.";

    // Vérifier les tables disponibles
    QSqlQuery testQuery("SELECT name FROM sqlite_master WHERE type='table' AND name='RESSOURCE'");
    if (testQuery.next()) {
        qDebug() << "Table RESSOURCE exists.";
    } else {
        qDebug() << "Table RESSOURCE not found in the database.";
        QSqlQuery caseQuery("SELECT name FROM sqlite_master WHERE type='table'");
        while (caseQuery.next()) {
            qDebug() << "Table found in database:" << caseQuery.value("name").toString();
        }
        return false;
    }

    // Vérifier la structure de la table RESSOURCE
    QSqlQuery columnQuery("PRAGMA table_info(RESSOURCE)");
    bool nomColumnFound = false;
    while (columnQuery.next()) {
        QString columnName = columnQuery.value("name").toString();
        qDebug() << "Column in RESSOURCE:" << columnName;
        if (columnName.toUpper() == "NOM") {
            nomColumnFound = true;
        }
    }
    if (!nomColumnFound) {
        qDebug() << "Column NOM not found in RESSOURCE table.";
        return false;
    }

    return true;
}

QStringList AddRessource::fetchResourcesFromDB()
{
    QStringList resourceNames;
    QSqlQuery query;
    query.prepare("SELECT NOM FROM RESSOURCE");
    query.exec();

    int rowCount = 0;
    while (query.next() && rowCount < 6) {
        QString name = query.value("NOM").toString().trimmed();
        if (name.isEmpty()) {
            name = query.value(1).toString().trimmed(); // Essayer la deuxième colonne si NOM est vide
        }
        if (!name.isEmpty()) {
            qDebug() << "Ressource importée :" << name;
            resourceNames << name;
            rowCount++;
        }
    }

    return resourceNames;
}

void AddRessource::notif() {
    int quantite = 0 ;
    QString nomRessource;
    QSqlQuery query;
    query.prepare("SELECT QUANTITE FROM RESSOURCE WHERE NOM = :nom");
    query.bindValue(":nom", nomRessource);
    if (query.exec() && query.next()) {
        nomRessource = query.value("NOM").toString();  // ou query.value(0)
    }

    if (query.exec()) {
        if (query.next()) {
            int quantiteDispo = query.value(0).toInt();

            if (quantite <= quantiteDispo) {
                ui->notif->setText("✔️ " + nomRessource + " ajouté avec succès !");
                ui->notif->setStyleSheet("color: green; font-weight: bold;");
            } else {
                ui->notif->setText("❌ Quantité insuffisante pour " + nomRessource);
                ui->notif->setStyleSheet("color: red; font-weight: bold;");
            }
        } else {
            ui->notif->setText("❌ Ressource introuvable : " + nomRessource);
            ui->notif->setStyleSheet("color: red; font-weight: bold;");
        }
    } else {
        ui->notif->setText("❌ Erreur lors de la requête pour " + nomRessource);
        ui->notif->setStyleSheet("color: red; font-weight: bold;");
    }

    QTimer::singleShot(3000, this, [=]() {
        ui->notif->clear();
    });
}

void AddRessource::displayResourcesInTableWidget(QTableWidget *tableWidget, const QStringList &resourceNames)
{
    tableWidget->clear();
    tableWidget->setRowCount(resourceNames.size());
    tableWidget->setColumnCount(1);
    tableWidget->setHorizontalHeaderLabels(QStringList() << "Nom de la ressource");

    for (int i = 0; i < resourceNames.size(); ++i) {
        QTableWidgetItem *item = new QTableWidgetItem(resourceNames[i]);
        item->setFlags(item->flags() | Qt::ItemIsUserCheckable | Qt::ItemIsEnabled);
        item->setCheckState(Qt::Unchecked);
        tableWidget->setItem(i, 0, item);
    }

    tableWidget->resizeColumnsToContents();
}

QStringList AddRessource::getCheckedResources(QTableWidget *tableWidget)
{
    QStringList selectedResources;

    for (int i = 0; i < tableWidget->rowCount(); ++i) {
        QTableWidgetItem *item = tableWidget->item(i, 0);
        if (item && item->checkState() == Qt::Checked) {
            selectedResources << item->text();
        }
    }

    return selectedResources;
}

void AddRessource::ExitApp()
{
    close();
}
