#include "cardreaderdialog.h"
#include "ui_cardreaderdialog.h"

CardReaderDialog::CardReaderDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::CardReaderDialog)
{
    ui->setupUi(this);

        connect(ui->cardInput, &QLineEdit::returnPressed, this, &CardReaderDialog::onCardScanned);

    if (reader.InitArduino()) {
        reader.ReturnResultToArduino("YELLOW");
    }
}

CardReaderDialog::~CardReaderDialog()
{
    delete ui;
}

void CardReaderDialog::onCardScanned(){
    QString cardData = ui->cardInput->text().trimmed();
    qDebug() << cardData;
       ui->cardInput->clear();
       if (!cardData.isEmpty()) {
           QString User = SearchCodeCardInDB(cardData);
            if (!User.isEmpty()) {
                reader.ReturnResultToArduino("GREEN");
                //MSG COMFIRM
                QMessageBox::information(this, "Success", "Welcome, " + User);
            } else {
                reader.ReturnResultToArduino("RED");
                //USER NOT FOUND
                QMessageBox::warning(this, "Access Denied", "Card not recognized.");
            }

            reader.ReturnResultToArduino("DISABLE");
            this->close();
        }

    }
QString CardReaderDialog::SearchCodeCardInDB(QString cardData){
    qDebug() << cardData;
    cardData.replace("\r", "").replace("\n", "");
    QSqlQuery Querry ;
    Querry.prepare("SELECT NOM FROM MEDECIN WHERE CARD_CHAR = :cardData;");
    Querry.bindValue(":cardData",cardData);
    if (!Querry.exec()) {
            qDebug() << "Query failed:" << Querry.lastError().text();
            return "";
        }

    if (Querry.next()) {
            qDebug() << Querry.value("NOM").toString();
            return Querry.value("NOM").toString();
        } else {
        qDebug() << "no result";
            return "";
        }
}
