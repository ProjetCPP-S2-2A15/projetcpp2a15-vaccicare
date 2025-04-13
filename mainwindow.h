#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "QMessageBox"
#include "connection.h"
#include "vaccin.h"
#include <QSqlTableModel>
#include <QMap>

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    void on_ajouterButton_clicked();
    void on_ModifierButton_clicked();
    void on_SupprimerButton_clicked();
    void onRowClicked(const QModelIndex &index);
    void onClickRefreshTable();
    void chargerTypesVaccin();


private:
    int getTypeVaccinId(const QString& typeName);
    QString getTypeVaccinName(int idTypeV);
    void ClickExporterPDF();
    void on_PushButton_rechercherNom_clicked();
    void on_PushButton_rechercherPays_clicked();
    void FillTable(bool triParDateDev,bool triParDatePrem);
    void SetupTable();
    Ui::MainWindow *ui;
    Connection Cnx;
    QMap<int, QString> typeVaccinsMap;

};
#endif // MAINWINDOW_H
