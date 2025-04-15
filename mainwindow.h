#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QStandardItemModel>

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
    void on_pushButton_clicked();
    void on_pushButton_5_clicked();
    void on_pushButton_6_clicked(); // Nouveau slot pour le bouton Générer
    void on_pushButton3_ajouter_clicked();
    void on_pushButton_2_supprimer_clicked();
    void on_pushButton_modifier_clicked();
    void on_lineEdit_rechercher_textChanged(const QString &text);
    void on_comboBox_tri_currentTextChanged(const QString &text);
    void on_pushButton_pdf_clicked();
    void updateResourceFilter();

private:
    Ui::MainWindow *ui;
    int attempts = 3;
    void refreshTableView();
    QStandardItemModel *fullResourceModel; // Modèle contenant toutes les ressources
};

#endif // MAINWINDOW_H
