#include "mainwindow.h"
#include "connection.h"
#include "resources.h"
#include <QMessageBox>
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;

    // db connection
    Connection c;
    bool test = c.createconnect(); // establish it
    if (test) {
        w.show();  // display main window
        QMessageBox::information(nullptr, QObject::tr("Database Connection"),
                                 QObject::tr("Connection successful.\nClick Cancel to exit."),
                                 QMessageBox::Cancel);


        Resources res1(1, "Microscope", 2, 10, 1, 20240312, 3, 1500);

        // try adding
        if (res1.ajouter()) {
            qDebug() << "Addition successful.";
        } else {
            qDebug() << "Error adding resource.";
        }

        // changing its name and quantity
        res1 = Resources(1, "Microscope Pro", 2, 15, 1, 20240312, 3, 2000);
        if (res1.modifier()) {
            qDebug() << "Modification successful.";
        } else {
            qDebug() << "Error modifying resource.";
        }

        // display in db
        qDebug() << "Listing resources:";
        Resources::afficher();

        // delete with id 1
        if (Resources::supprimer(1)) {
            qDebug() << "Deletion successful.";
        } else {
            qDebug() << "Error deleting resource.";
        }

    } else {
        // bd fails to connect show an error message
        QMessageBox::critical(nullptr, QObject::tr("Database Connection"),
                              QObject::tr("Connection failed.\nClick Cancel to exit."),
                              QMessageBox::Cancel);
    }

    return a.exec(); // application event loop
}
