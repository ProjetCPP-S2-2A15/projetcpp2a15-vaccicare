#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "connection.h"
#include "ficheprojet.h"
#include "date.h"
#include "statsscreen.h"
#include "exportdialog.h"
#include "Design.h"
#include "cardreader.h"


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
private:
    Connection Cnx;
    Ui::MainWindow *ui;

    void SetupTable();
    void FillTable(bool AFFICHER_CLOTURER,QString NomChercheur,QString NomProjet);
    void AddProject();
    void ModifyProject();
    void DeleteProject();
    void CloseProject();
    void SearchProjects();
    void ExportProjectList();
    void ShowStatistics();
    void SetupDesign();
    void onCardScanned();
    void OpenDialog();

};
#endif // MAINWINDOW_H

