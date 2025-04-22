#ifndef PDFSCANNER_H
#define PDFSCANNER_H

#include <QObject>
#include <QString>
#include <QFileDialog>
#include <QProcess>
#include <QFile>
#include <QTextStream>
#include <QDebug>
#include <QCoreApplication>

class PDFScanner : public QObject
{
    Q_OBJECT

public:
    explicit PDFScanner(QObject *parent = nullptr);

    QString SelectPDF();    // Returns the path to the selected PDF
    bool ValidatePDF(const QString &pdfPath);  // Validates if the PDF can be scanned
    bool ScanPDF(const QString &pdfPath);  // Scans the PDF and returns true/false if successful

private:
    QString lastSelectedPDF;
    const QString OUTPUT_TXT_PATH = QCoreApplication::applicationDirPath() + "/scanned_output.txt";
};

#endif // PDFSCANNER_H
