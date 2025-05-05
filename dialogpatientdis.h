#ifndef DIALOGPATIENTDIS_H
#define DIALOGPATIENTDIS_H
#include <QDialog>
#include "qtmetamacros.h"
#include "ui_dialogpatientdis.h"
namespace Ui {
class dialogpatientdis;
}

class dialogpatientdis : public QDialog
{
    Q_OBJECT

public:
    explicit dialogpatientdis(QWidget *parent = nullptr);
    ~dialogpatientdis();

private:
    Ui::dialogpatientdis *ui;
    void ExitApp();
};
#endif // DIALOGPATIENTDIS_H
