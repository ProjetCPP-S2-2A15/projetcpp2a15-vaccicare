#ifndef ADDRESSOURCE_H
#define ADDRESSOURCE_H

#include "qlistview.h"
#include "qtablewidget.h"
#include <QDialog>
#include <QListView>
#include <QStringList>
#include "resources.h"

namespace Ui {
class AddRessource;
}

class AddRessource : public QDialog
{
    Q_OBJECT

public:
    explicit AddRessource(QWidget *parent = nullptr);
    ~AddRessource();
    void displayResourcesInTableWidget(QTableWidget* tableWidget, const QStringList& resources);
    QStringList fetchResourcesFromDB();
    QStringList getCheckedResources(QTableWidget* tableWidget);
    void notif();
    void ExitApp();

private slots:
    //void notif(const QString &nomRessource);

private:
    Ui::AddRessource *ui;
    bool initializeDatabase();


};

#endif // ADDRESSOURCE_H
