#ifndef EXPORTDIALOG_H
#define EXPORTDIALOG_H

#include <QDialog>
#include "connection.h"
#include "projet.h"
#include "date.h"
#include <QMessageBox>
#include <QPdfWriter>
#include <QPainter>
#include <QFileDialog>
#include <QTextDocument>
#include <QStandardPaths>
#include <QPageSize>

namespace Ui {
class ExportDialog;
}

class ExportDialog : public QDialog
{
    Q_OBJECT

public:
    explicit ExportDialog(QWidget *parent = nullptr);
    ~ExportDialog();

private:
    void SetUpUI();
    Ui::ExportDialog *ui;
    void Annuler();
    void Valider();
    void SelectFolder();
    const int COL_ID_PROJET_WIDTH = 250;
    const int COL_NOM_WIDTH = 220;
    const int COL_DATE_DEBUT_WIDTH = 340;
    const int COL_ID_RESPONSABLE_WIDTH = 380;
    const int COL_BUDGET_WIDTH = 250;
    const int COL_DATE_FIN_WIDTH = 340;
    const int ROW_HEIGHT = 80;
};

#endif // EXPORTDIALOG_H
