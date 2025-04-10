#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#include "medecin.h"
#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
public slots:
    void on_pushButton3_ajouter_clicked();
    void on_pushButton_modifier_clicked();
    void on_pushButton_2_supprimer_clicked();
    void on_lineEdit_rechercher_textChanged(const QString &text);
    void on_comboBox_tri_currentTextChanged(const QString &text);
    void on_pushButton_pdf_clicked();
private:
    Ui::MainWindow *ui;
    Medecin medecin;
    void refreshTableView();
};
#endif // MAINWINDOW_H
