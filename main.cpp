#include "mainwindow.h"
#include "connection.h"
#include <QMessageBox>
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
//   Connection Cnx;
//   bool Connected_To_DB;
//   Connected_To_DB = Cnx.createconnect();
//   if(!Connected_To_DB){
//       // Show a warning message box
//       QMessageBox::warning(w.centralWidget(),"Database Connection Failed",
//                   "Failed to connect to the database. The application will now exit.",
//                   QMessageBox::Ok);

//       a.closeAllWindows();
//   }
    w.show();
    return a.exec();
}
