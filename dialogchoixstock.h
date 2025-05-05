#ifndef DIALOGCHOIXSTOCK_H
#define DIALOGCHOIXSTOCK_H

#include <QDialog>

namespace Ui {
class DialogChoixStock;
}

class DialogChoixStock : public QDialog
{
    Q_OBJECT

public:
    explicit DialogChoixStock(QWidget *parent = nullptr);
    ~DialogChoixStock();
    int GetResult() const {return result;};
private:
    Ui::DialogChoixStock *ui;
    int result;
    void SetChoixVaccin();
    void SetChoixRessource();
};

#endif // DIALOGCHOIXSTOCK_H
