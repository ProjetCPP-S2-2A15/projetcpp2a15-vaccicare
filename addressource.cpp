#include "addressource.h"
#include "ui_addressource.h"
#include "Design.h"

AddRessource::AddRessource(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::AddRessource)
{
    ui->setupUi(this);

    StyleCheckBox(ui->checkBox);
    StyleCheckBox(ui->checkBox_2);
    StyleCheckBox(ui->checkBox_3);
    StyleCheckBox(ui->checkBox_4);
    StyleCheckBox(ui->checkBox_5);
    StyleCheckBox(ui->checkBox_6);

}

AddRessource::~AddRessource()
{
    delete ui;
}
