#ifndef ADDRESSOURCE_H
#define ADDRESSOURCE_H

#include <QDialog>

namespace Ui {
class AddRessource;
}

class AddRessource : public QDialog
{
    Q_OBJECT

public:
    explicit AddRessource(QWidget *parent = nullptr);
    ~AddRessource();

private:
    Ui::AddRessource *ui;
};

#endif // ADDRESSOURCE_H
