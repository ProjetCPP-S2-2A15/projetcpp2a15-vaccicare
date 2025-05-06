#ifndef DIALOGCHOIXTYPEIMPORTPDF_H
#define DIALOGCHOIXTYPEIMPORTPDF_H

#include <QDialog>
#include "date.h"
#include "projet.h"
#include "medecin.h"
#include "resources.h"
#include "vaccin.h"
#include "patient.h"

#include <QMessageBox>
#include <QPdfWriter>
#include <QPainter>
#include <QPrinter>
#include <QFileDialog>
#include <QTextDocument>
#include <QStandardPaths>
#include <QPageSize>
#include <QSqlQueryModel>

namespace Ui {
class DialogChoixTypeImportPDF;
}

class DialogChoixTypeImportPDF : public QDialog
{
    Q_OBJECT

public:
    explicit DialogChoixTypeImportPDF(QWidget *parent = nullptr);
    ~DialogChoixTypeImportPDF();
    int GetResult() const {return result;};
private:
    const int COL_ID_PROJET_WIDTH = 250;
    const int COL_NOM_WIDTH = 220;
    const int COL_DATE_DEBUT_WIDTH = 340;
    const int COL_ID_RESPONSABLE_WIDTH = 380;
    const int COL_BUDGET_WIDTH = 250;
    const int COL_DATE_FIN_WIDTH = 340;
    const int ROW_HEIGHT = 80;
    QString filePath;
    Ui::DialogChoixTypeImportPDF *ui;
    int result;
    void GenererPDF();
    void ExitDialog();
    void SelectPath();
    void SetUpUI();
    void setupDesign();

    bool GenererPDFProjet(QString fileName,QDate startDate,QDate endDate);
    bool GenererPDFPatient(QString fileName);
    bool GenererPDFMedecin(QString fileName);
    bool GenererPDFRessource(QString fileName,QDate startDate,QDate endDate);
    bool GenererPDFVaccin(QString fileName,QDate startDate,QDate endDate);
};
#endif // DIALOGCHOIXTYPEIMPORTPDF_H
