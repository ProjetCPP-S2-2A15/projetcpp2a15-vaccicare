#ifndef PROJECTWORKFLOWDIALOG_H
#define PROJECTWORKFLOWDIALOG_H

#include <QDialog>
#include "projet.h"
#include <vector>
#include "selectprojectdialog.h"
#include <QMessageBox>

namespace Ui {
class ProjectWorkFlowDialog;
}

class ProjectWorkFlowDialog : public QDialog
{
    Q_OBJECT

public:
    explicit ProjectWorkFlowDialog(QWidget *parent = nullptr,int User_Id = 0);
    ~ProjectWorkFlowDialog();

private:
    Ui::ProjectWorkFlowDialog *ui;
    int UserId;
    std::vector<Projet> ProjectsForUser;
    Projet CurrProject;
    void ShowDialog(Projet WorkingProject);
    void CloseDialog();

};

#endif // PROJECTWORKFLOWDIALOG_H
