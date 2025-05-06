#include "dialogchoixstock.h"
#include "ui_dialogchoixstock.h"
#include "Design.h"

DialogChoixStock::DialogChoixStock(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DialogChoixStock)
{    
    ui->setupUi(this);
    connect(ui->ButtonConsulterVaccin,&QPushButton::clicked,this,&DialogChoixStock::SetChoixVaccin);
    connect(ui->ButtonConsutlerRessource,&QPushButton::clicked,this,&DialogChoixStock::SetChoixRessource);

    StyleButtonBlue(ui->ButtonConsulterVaccin);
    StyleButtonBlue(ui->ButtonConsutlerRessource);
}



DialogChoixStock::~DialogChoixStock()
{
    delete ui;
}



void DialogChoixStock::SetChoixVaccin(){
    result = 2;
    close();
}

void DialogChoixStock::SetChoixRessource(){
    result = 1;
    close();
}
