#include <poppler-qt5.h> // ou poppler-qt6.h
#include <QFileDialog>
#include <QTextStream>
#include <QSqlQuery>
#include <QMessageBox>

void MainWindow::on_btn_importerPDF_clicked() {
    QString fileName = QFileDialog::getOpenFileName(this, "Choisir un fichier PDF", "", "Fichiers PDF (*.pdf)");
    if (fileName.isEmpty()) return;

    Poppler::Document* document = Poppler::Document::load(fileName);
    if (!document || document->isLocked()) {
        QMessageBox::warning(this, "Erreur", "Impossible de lire le PDF.");
        return;
    }

    QSqlQuery query;

    for (int i = 0; i < document->numPages(); ++i) {
        Poppler::Page* page = document->page(i);
        if (!page) continue;

        QString text = page->text(); // tout le texte brut
        QStringList lignes = text.split('\n');

        for (const QString& ligne : lignes) {
            // Exemple de ligne : "1;Pfizer;1;Virus;Approuvé;20201201;USA;-20;10000;20231231;Oui"
            QStringList fields = ligne.split(';');
            if (fields.size() != 11) continue;

            query.prepare("INSERT INTO VACCIN VALUES (:id, :nom, :type, :agent, :statut, :date_dev, :pays, :temp, :stock, :date_per, :auto)");
            query.bindValue(":id", fields[0].toInt());
            query.bindValue(":nom", fields[1]);
            query.bindValue(":type", fields[2].toInt());
            query.bindValue(":agent", fields[3]);
            query.bindValue(":statut", fields[4]);
            query.bindValue(":date_dev", fields[5].toInt());
            query.bindValue(":pays", fields[6]);
            query.bindValue(":temp", fields[7].toInt());
            query.bindValue(":stock", fields[8].toInt());
            query.bindValue(":date_per", fields[9].toInt());
            query.bindValue(":auto", fields[10]);

            if (!query.exec()) {
                qDebug() << "Erreur d'insertion: " << query.lastError().text();
            }
        }

        delete page;
    }

    delete document;

    QMessageBox::information(this, "Importation", "Importation terminée avec succès !");
    afficherVaccins(); // Recharge le tableau
}
#include <poppler-qt5.h> // ou poppler-qt6.h
#include <QFileDialog>
#include <QTextStream>
#include <QSqlQuery>
#include <QMessageBox>

void MainWindow::on_btn_importerPDF_clicked() {
    QString fileName = QFileDialog::getOpenFileName(this, "Choisir un fichier PDF", "", "Fichiers PDF (*.pdf)");
    if (fileName.isEmpty()) return;

    Poppler::Document* document = Poppler::Document::load(fileName);
    if (!document || document->isLocked()) {
        QMessageBox::warning(this, "Erreur", "Impossible de lire le PDF.");
        return;
    }

    QSqlQuery query;

    for (int i = 0; i < document->numPages(); ++i) {
        Poppler::Page* page = document->page(i);
        if (!page) continue;

        QString text = page->text(); // tout le texte brut
        QStringList lignes = text.split('\n');

        for (const QString& ligne : lignes) {
            // Exemple de ligne : "1;Pfizer;1;Virus;Approuvé;20201201;USA;-20;10000;20231231;Oui"
            QStringList fields = ligne.split(';');
            if (fields.size() != 11) continue;

            query.prepare("INSERT INTO VACCIN VALUES (:id, :nom, :type, :agent, :statut, :date_dev, :pays, :temp, :stock, :date_per, :auto)");
            query.bindValue(":id", fields[0].toInt());
            query.bindValue(":nom", fields[1]);
            query.bindValue(":type", fields[2].toInt());
            query.bindValue(":agent", fields[3]);
            query.bindValue(":statut", fields[4]);
            query.bindValue(":date_dev", fields[5].toInt());
            query.bindValue(":pays", fields[6]);
            query.bindValue(":temp", fields[7].toInt());
            query.bindValue(":stock", fields[8].toInt());
            query.bindValue(":date_per", fields[9].toInt());
            query.bindValue(":auto", fields[10]);

            if (!query.exec()) {
                qDebug() << "Erreur d'insertion: " << query.lastError().text();
            }
        }

        delete page;
    }

    delete document;

    QMessageBox::information(this, "Importation", "Importation terminée avec succès !");
    afficherVaccins(); // Recharge le tableau
}
