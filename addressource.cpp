#include "addressource.h"
#include "ui_addressource.h"

AddRessource::AddRessource(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::AddRessource)
{
    ui->setupUi(this);
}

AddRessource::~AddRessource()
{
    delete ui;
}
