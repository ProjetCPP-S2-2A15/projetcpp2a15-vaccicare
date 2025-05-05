#include "dialogpatientdis.h"
#include "ui_dialogpatientdis.h"

dialogpatientdis::dialogpatientdis(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::dialogpatientdis)
{
    ui->setupUi(this);
    //ui->Calendrier_Info->isReadOnly();
    //connect(ui->ButtonExit,&QPushButton::clicked,this,&calendrierDialog::ExitApp);
}

dialogpatientdis::~dialogpatientdis()
{
    delete ui;
}
//recherchepatient
void dialogpatientdis::ExitApp(){
    close();
}
