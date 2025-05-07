#include "dialogchoixstat.h"
#include "ui_dialogchoixstat.h"

DialogChoixStat::DialogChoixStat(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DialogChoixStat)
{
    ui->setupUi(this);

    connect(ui->ButtonExit,&QPushButton::clicked,this,&DialogChoixStat::CloseDialog);
    connect(ui->ButtonStatProjet,&QPushButton::clicked,this,&DialogChoixStat::OurvirStatProjet);
    connect(ui->ButtonStatMedecin,&QPushButton::clicked,this,&DialogChoixStat::OuvrirStatMedecin);
    connect(ui->ButtonStatVaccin,&QPushButton::clicked,this,&DialogChoixStat::OuvrirStatVaccin);

    StyleButtonRed(ui->ButtonExit);
    StyleButtonBlue(ui->ButtonStatProjet);
    StyleButtonBlue(ui->ButtonStatMedecin);
    StyleButtonBlue(ui->ButtonStatVaccin);

}

DialogChoixStat::~DialogChoixStat()
{
    delete ui;
}

void DialogChoixStat::CloseDialog(){
    close();
}

void DialogChoixStat::OurvirStatProjet(){
    StatsScreen *NewDialog = new StatsScreen();
    NewDialog->exec();
}

void DialogChoixStat::OuvrirStatMedecin(){

    DialogStatMedecin *NewDialog = new DialogStatMedecin();
    NewDialog->exec();
}

void DialogChoixStat::OuvrirStatVaccin(){
    StatVaccin statsWindow;
    statsWindow.exec();
}
