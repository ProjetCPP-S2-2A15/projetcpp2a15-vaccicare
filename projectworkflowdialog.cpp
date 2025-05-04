#include "projectworkflowdialog.h"
#include "ui_projectworkflowdialog.h"

ProjectWorkFlowDialog::ProjectWorkFlowDialog(QWidget *parent ,int User_Id) :
    QDialog(parent),
    ui(new Ui::ProjectWorkFlowDialog)
{

    ui->setupUi(this);
    this->UserId = User_Id;

    connect(ui->ButtonExit,&QPushButton::clicked,this,&ProjectWorkFlowDialog::CloseDialog);
    connect(ui->ButtonAjouter,&QPushButton::clicked,this,&ProjectWorkFlowDialog::AddExperience);
    connect(ui->ButtonModifier,&QPushButton::clicked,this,&ProjectWorkFlowDialog::ModifiyExperience);
    connect(ui->ButtonSupprimer,&QPushButton::clicked,this,&ProjectWorkFlowDialog::DeleteExperience);

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
    QString DescriptionProjet = WorkingProject.GetProjectDescription();
    ui->textEdit->setText(DescriptionProjet);

    //Load Experience table With Data
    std::vector<Experience> ListeExperience = Experience::LoadExperienceFromDb(WorkingProject.Data.ID_Projet);
}

void ProjectWorkFlowDialog::DeleteExperience(){

}

void ProjectWorkFlowDialog::ModifiyExperience(){

}

void ProjectWorkFlowDialog::AddExperience() {

}

void ProjectWorkFlowDialog::CloseDialog(){
    bool Saved = CurrProject.SaveProjectDescription(ui->textEdit->toPlainText());

    if (Saved) {
        close();
    } else {
        QMessageBox::warning(this, tr("Save Failed"),
                             tr("The project description could not be saved. Please try again."),
                             QMessageBox::Ok);
        close();
    }
}
