#ifndef DIALOGCHOIXSTAT_H
#define DIALOGCHOIXSTAT_H

#include <QDialog>
#include "statsscreen.h"
#include "dialogstatmedecin.h"
#include "StatTvaccin.h"
#include "Design.h"

namespace Ui {
class DialogChoixStat;
}

class DialogChoixStat : public QDialog
{
    Q_OBJECT

public:
    explicit DialogChoixStat(QWidget *parent = nullptr);
    ~DialogChoixStat();

private:
    Ui::DialogChoixStat *ui;
    void CloseDialog();
    void OurvirStatProjet();
    void OuvrirStatMedecin();
    void OuvrirStatVaccin();
};

#endif // DIALOGCHOIXSTAT_H
