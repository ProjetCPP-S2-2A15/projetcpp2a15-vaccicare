#ifndef CARDREADERDIALOG_H
#define CARDREADERDIALOG_H

#include <QDialog>
#include "cardreader.h"
#include "connection.h"
#include <QSqlQuery>
#include <QMessageBox>
#include <QDebug>

namespace Ui {
class CardReaderDialog;
}

class CardReaderDialog : public QDialog
{
    Q_OBJECT

public:
    explicit CardReaderDialog(QWidget *parent = nullptr);
    ~CardReaderDialog();

private:
    Ui::CardReaderDialog *ui;
        CardReader reader;
        void onCardScanned();
        QString SearchCodeCardInDB(QString cardData);

};

#endif // CARDREADERDIALOG_H
