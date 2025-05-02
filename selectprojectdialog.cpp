#include "selectprojectdialog.h"
#include "ui_selectprojectdialog.h"

SelectProjectDialog::SelectProjectDialog(QWidget *parent, std::vector<Projet> ListeProjet)
    : QDialog(parent),
      ui(new Ui::SelectProjectDialog),
      m_projets(std::move(ListeProjet))
{
    ui->setupUi(this);
    connect(ui->tableWidget, &QTableWidget::cellDoubleClicked,
            this, &SelectProjectDialog::onTableRowSelected);
    connect(ui->ButtonExit,&QPushButton::clicked,this,&SelectProjectDialog::CloseDialog);

    // Fill the table
    ui->tableWidget->setRowCount(m_projets.size());
    ui->tableWidget->setColumnCount(2); // Adjust based on your data

    for (int i = 0; i < m_projets.size(); ++i) {
        ui->tableWidget->setItem(i, 0, new QTableWidgetItem(m_projets[i].Data.ID_Projet));
        ui->tableWidget->setItem(i, 1, new QTableWidgetItem(m_projets[i].Data.Nom));
    }
}

SelectProjectDialog::~SelectProjectDialog()
{
    delete ui;
}

Projet SelectProjectDialog::GetProjectSelected() const {
    return m_selectedProject;
}

void SelectProjectDialog::onTableRowSelected(int row){
    m_selectedProject = m_projets[row];
    accept();  // Closes the dialog with QDialog::Accepted
}

void SelectProjectDialog::CloseDialog(){
    close();
}
