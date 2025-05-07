#include "ficheepreience.h"
#include "ui_ficheepreience.h"
#include "Design.h"

ficheEpreience::ficheEpreience(QWidget *parent,bool IsModeAjout,int ID_Experience) :
    QDialog(parent),
    ui(new Ui::ficheEpreience)
{
    ui->setupUi(this);
    this->IsModeAjout = IsModeAjout;
    this->ID_Experience = ID_Experience;

    connect(ui->ButtonValider,&QPushButton::clicked,this,&ficheEpreience::Valider);
    connect(ui->ButtonAnnuler,&QPushButton::clicked,this,&ficheEpreience::Annuler);
    connect(ui->ButtonImport,&QPushButton::clicked,this,&ficheEpreience::ImportPdf);

    SetupUI();
    setupDesign();
}

ficheEpreience::~ficheEpreience()
{
    delete ui;
}

void ficheEpreience::SetupUI(){
    if(IsModeAjout){
        ui->LineEditNom->setReadOnly(false);
        ui->LineEditId->setReadOnly(false);
        ui->LineEditId->setText(QString::number(ID_Experience));
        ui->LineEditNom->setText("");
        ui->TextEditDescription->setText("");
    }else{
        result = Experience::LoadExperienceDescriptionFromDb(ID_Experience);
        ui->LineEditId->setReadOnly(true);
        ui->LineEditId->setText(QString::number(result.ID_Experience));
        ui->LineEditNom->setReadOnly(true);
        ui->LineEditNom->setText(result.Nom);
        ui->TextEditDescription->setText(result.Description);
    }
}

void ficheEpreience::setupDesign()
{
    StyleLabel(ui->label);
    StyleLabel(ui->label_2);
    StyleLabel(ui->label_3);

    StyleLineEdit(ui->LineEditId);
    StyleLineEdit(ui->LineEditNom);

    StyleButtonGreen(ui->ButtonValider);
    StyleButtonRed(ui->ButtonAnnuler);

    StyleTextEdit(ui->TextEditDescription);
}


void ficheEpreience::Valider(){
    result.Description = ui->TextEditDescription->toPlainText();
    result.ID_Experience = ui->LineEditId->text().toInt();
    result.Nom = ui->LineEditNom->text();
    close();
}

void ficheEpreience::Annuler(){
    result.ID_Experience = -1;
    close();
}

void ficheEpreience::ImportPdf(){
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
    ui->TextEditDescription->setText(fileContents);
}

