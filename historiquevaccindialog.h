#ifndef HISTORIQUEVACCINDIALOG_H
#define HISTORIQUEVACCINDIALOG_H

#include <QDialog>
#include "vaccin.h"
#include <QMap>
#include <QFile>
#include <QStringList>
#include <QTableWidgetItem>
#include <QTextStream>
#include <QStandardPaths>
#include <QFileInfo>
#include <QDesktopServices>
#include <QUrl>

namespace Ui {
class HistoriqueVaccinDialog;
}

class HistoriqueVaccinDialog : public QDialog
{
    Q_OBJECT

public:
    explicit HistoriqueVaccinDialog(QWidget *parent = nullptr);
    ~HistoriqueVaccinDialog();
    typedef struct VaccinSnapshot {
            QString nom;
            int idTypeV;
            QString agentCible;
            QString statutDev;
            QDate dateDev;
            QString paysOrigine;
            int tempConservation;
            int stockDisponible;
            QDate datePeremption;
            QString autorisation;
        }VaccinSnapshot;

    QMap<int, VaccinSnapshot> copieInitialeVaccins;
private:
    Ui::HistoriqueVaccinDialog *ui;
    void CloseDialog();
    void chargerHistoriqueDepuisFichier();
    QString genererHistorique(int id, const VaccinSnapshot &nouveau);
    void ajouterHistoriqueDansTable(int id, int ancienStock, int nouveauStock);
    void on_btnOuvrirDossier_clicked();

    void setupDesign();
};

#endif // HISTORIQUEVACCINDIALOG_H
