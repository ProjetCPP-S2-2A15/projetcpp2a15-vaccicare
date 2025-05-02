#ifndef LISTEPROJETDIALOG_H
#define LISTEPROJETDIALOG_H

#include <QDialog>
#include "connection.h"
#include "ficheprojet.h"
#include "date.h"
//#include "statsscreen.h"
//#include "exportdialog.h"
#include "Design.h"

namespace Ui {
class ListeProjetDialog;
}

class ListeProjetDialog : public QDialog
{
    Q_OBJECT

public:
    explicit ListeProjetDialog(QWidget *parent = nullptr);
    ~ListeProjetDialog();

private:
    Ui::ListeProjetDialog *ui;
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
    void CloseDialog();
};

#endif // LISTEPROJETDIALOG_H
