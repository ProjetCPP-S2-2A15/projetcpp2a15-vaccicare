#ifndef FICHEPROJET_H
#define FICHEPROJET_H

#include <QDialog>
#include <QMessageBox>
#include "projet.h"
#include <vector>
#include "date.h"

namespace Ui {
class FicheProjet;
}

class FicheProjet : public QDialog
{
    Q_OBJECT

public:
    enum Result { Accepted, Canceled };
    explicit FicheProjet(int projectID,bool IsModeAjouter, QWidget *parent);
    ~FicheProjet();
    Result getResult() const { return result_; }


protected:
    void accept() override;

    void cancel();
private:
    Result result_ = Canceled;
    Ui::FicheProjet *ui;
    int m_projectID;
    bool m_IsModeAjout;
    void SetupUI(int ID,bool IsModeAjout);
    void CheckBoxChecked();
};

#endif // FICHEPROJET_H
