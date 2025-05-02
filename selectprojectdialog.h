#ifndef SELECTPROJECTDIALOG_H
#define SELECTPROJECTDIALOG_H

#include <QDialog>
#include "projet.h"
#include <QTableWidgetItem>

namespace Ui {
class SelectProjectDialog;
}

class SelectProjectDialog : public QDialog
{
    Q_OBJECT

public:
    explicit SelectProjectDialog(QWidget *parent = nullptr,std::vector<Projet> ListeProjet = std::vector<Projet>());
    ~SelectProjectDialog();
    Projet GetProjectSelected() const;
private:
    Ui::SelectProjectDialog *ui;
    std::vector<Projet> m_projets;
    Projet m_selectedProject;
    void onTableRowSelected(int row);
    void CloseDialog();
};

#endif // SELECTPROJECTDIALOG_H
