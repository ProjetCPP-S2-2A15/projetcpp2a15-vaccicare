#ifndef FICHEEPREIENCE_H
#define FICHEEPREIENCE_H

#include <QDialog>
#include "experience.h"

namespace Ui {
class ficheEpreience;
}

class ficheEpreience : public QDialog
{
    Q_OBJECT

public:
    explicit ficheEpreience(QWidget *parent = nullptr, bool IsModeAjout = true,int ID_Experience = -1);
    ~ficheEpreience();
    Experience GetResult() const { return result; };
private:
    bool IsModeAjout;
    Ui::ficheEpreience *ui;
    int ID_Experience;
    QString nomExperience;
    QString descriptionExperience;
    Experience result;
    void SetupUI();
    void Annuler();
    void Valider();

    void setupDesign();
};

#endif // FICHEEPREIENCE_H
