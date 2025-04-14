#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "medecin.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_pushButton_clicked(); // Slot pour le bouton LOGIN
    void on_pushButton3_ajouter_clicked();
    void on_pushButton_2_supprimer_clicked();
    void on_pushButton_modifier_clicked();
    void on_lineEdit_rechercher_textChanged(const QString &text);
    void on_comboBox_tri_currentTextChanged(const QString &text);
    void on_pushButton_pdf_clicked();
    void refreshTableView();

private:
    Ui::MainWindow *ui;
    int attempts = 3; // Ajout de la variable pour les tentatives
};

#endif // MAINWINDOW_H
