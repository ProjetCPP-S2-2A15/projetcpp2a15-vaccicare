#include "projectworkflowdialog.h"
#include "ui_projectworkflowdialog.h"
#include "Design.h"

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
    connect(ui->ButtonImport,&QPushButton::clicked,this,&ProjectWorkFlowDialog::ImportPdf);

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

    setupDesign();

}

ProjectWorkFlowDialog::~ProjectWorkFlowDialog(){
    delete ui;
}

void ProjectWorkFlowDialog::setupDesign()
{

    StyleTextEdit(ui->textEdit);

    StyleLabel(ui->label);
    StyleLabel(ui->label_2);
    StyleLabel(ui->label_3);

    StyleButtonGreen(ui->ButtonAjouter);
    StyleButtonBlue(ui->ButtonModifier);
    StyleButtonDarkred(ui->ButtonExit);
    StyleButtonRed(ui->ButtonSupprimer);

    StyleTableWidget(ui->tableWidget);


}

void ProjectWorkFlowDialog::ShowDialog(Projet WorkingProject){
    QString DescriptionProjet = WorkingProject.GetProjectDescription();
    ui->textEdit->setText(DescriptionProjet);

    //Load Experience table With Data
    //Get all Expereices and put them in a vector
    std::vector<Experience> ListeExperience = Experience::LoadExperienceFromDb(WorkingProject.Data.ID_Projet);

    ui->tableWidget->clear();
    ui->tableWidget->setRowCount(static_cast<int>(ListeExperience.size()));
    ui->tableWidget->setColumnCount(2);
    QStringList headers = {"ID", "Nom"};
    ui->tableWidget->setHorizontalHeaderLabels(headers);

    if(ListeExperience.size() != 0){
        for (int i = 0; i < ListeExperience.size(); ++i) {
            QTableWidgetItem *idItem = new QTableWidgetItem(QString::number(ListeExperience[i].ID_Experience));
            QTableWidgetItem *nameItem = new QTableWidgetItem(ListeExperience[i].Nom);
            idItem->setFlags(idItem->flags() ^ Qt::ItemIsEditable);
            nameItem->setFlags(nameItem->flags() ^ Qt::ItemIsEditable);

            ui->tableWidget->setItem(i, 0, idItem);
            ui->tableWidget->setItem(i, 1, nameItem);
        }
    }


    ui->tableWidget->resizeColumnsToContents();
}

void ProjectWorkFlowDialog::DeleteExperience(){
    int row = ui->tableWidget->currentRow();
    if (row < 0) {
        QMessageBox::warning(this, "Aucune sélection", "Veuillez sélectionner une expérience à supprimer.");
        return;
    }

    int idExperience = ui->tableWidget->item(row, 0)->text().toInt();

    QMessageBox::StandardButton reply;
    reply = QMessageBox::question(this, "Confirmation", "Êtes-vous sûr de vouloir supprimer cette expérience ?",
                                  QMessageBox::Yes | QMessageBox::No);

    if (reply == QMessageBox::Yes) {
        Experience exp = Experience::LoadExperienceDescriptionFromDb(idExperience);
        if (exp.DeleteExperience()) {
            ShowDialog(CurrProject); // Refresh table
        } else {
            QMessageBox::critical(this, "Erreur", "Échec de la suppression de l'expérience.");
        }
    }
}

void ProjectWorkFlowDialog::ModifiyExperience(){
    int row = ui->tableWidget->currentRow();
    if (row < 0) {
        QMessageBox::warning(this, "Aucune sélection", "Veuillez sélectionner une expérience à modifier.");
        return;
    }

    int idExperience = ui->tableWidget->item(row, 0)->text().toInt();

    ficheEpreience *NewDialog = new ficheEpreience(this,false,idExperience);
    NewDialog->exec();
    Experience result = NewDialog->GetResult();
    if(result.ID_Experience != -1){
        result.UpladteExperience();
        ShowDialog(CurrProject); // Refresh the table after modification
    }
}

void ProjectWorkFlowDialog::AddExperience() {
    ficheEpreience *NewDialog = new ficheEpreience(this,true,Experience::GetLastID() +1);
    NewDialog->exec();
    Experience result = NewDialog->GetResult();
    if(result.ID_Experience != -1){
        result.AddExperienceToDb(CurrProject.Data.ID_Projet);
    }
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

void ProjectWorkFlowDialog::ImportPdf(){
    PDFScanner *Scanner = new PDFScanner();
    QString filePath = Scanner->SelectPDF();
    if(filePath != ""){
        bool Scanned;
        Scanned = Scanner->ScanPDF(filePath);
        if(!Scanned){
            QMessageBox::warning(this, "File Error", "Failed to open scanned_output.txt");
        }
    }
    delete Scanner;
    const QString filepath = QCoreApplication::applicationDirPath() + "/scanned_output.txt";
    QFile file(filepath);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QMessageBox::warning(this, "File Error", "Failed to open scanned_output.txt");
        return;
    }
    QTextStream in(&file);
    QString fileContents = in.readAll();
    file.close();
    ui->textEdit->setText(fileContents);
}
