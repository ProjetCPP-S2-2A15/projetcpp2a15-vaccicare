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
    const int X_MARGIN_NOM = 260;
    const int X_MARGIN_DATE_DEBUT = 480;
    const int X_MARGIN_ID_RESPONSABLE = 820;
    const int X_MARGIN_BUDGET = 1200;
    const int X_MARGIN_DATE_FIN = 1450;
};

#endif // EXPORTDIALOG_H
