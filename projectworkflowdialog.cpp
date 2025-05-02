#include "projectworkflowdialog.h"
#include "ui_projectworkflowdialog.h"

ProjectWorkFlowDialog::ProjectWorkFlowDialog(QWidget *parent ,int User_Id) :
    QDialog(parent),
    ui(new Ui::ProjectWorkFlowDialog)
{

    ui->setupUi(this);
    this->UserId = User_Id;

    connect(ui->ButtonExit,&QPushButton::clicked,this,&ProjectWorkFlowDialog::CloseDialog);

    ProjectsForUser = Projet::ReadProjectListFromDB(UserId);
    if(ProjectsForUser.size() == 0){
        close();
    }else if(ProjectsForUser.size() == 1){
        CurrProject = ProjectsForUser[0];
    }else{
        SelectProjectDialog *NewDialog = new SelectProjectDialog(this,ProjectsForUser);
        NewDialog->exec();
        CurrProject = NewDialog->GetProjectSelected();
    }
    ShowDialog(CurrProject);

}

ProjectWorkFlowDialog::~ProjectWorkFlowDialog(){
    delete ui;
}

void ProjectWorkFlowDialog::ShowDialog(Projet WorkingProject){

}

void ProjectWorkFlowDialog::CloseDialog(){

    if(ProjectsForUser.size() == 1){
        close();
    }else{
        close();
        SelectProjectDialog *NewDialog = new SelectProjectDialog(this,ProjectsForUser);
        NewDialog->exec();
        CurrProject = NewDialog->GetProjectSelected();
    }
    ShowDialog(CurrProject);

}
