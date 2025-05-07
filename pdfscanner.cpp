#include "pdfscanner.h"

PDFScanner::PDFScanner(QObject *parent) : QObject(parent) {}

QString PDFScanner::SelectPDF()
{
    // Open a file dialog to select a PDF file
    QString filePath = QFileDialog::getOpenFileName(nullptr, "Select PDF File", "", "PDF Files (*.pdf)");
    if (!filePath.isEmpty()) {
        lastSelectedPDF = filePath;
        return filePath;
    }
    return "";
}

bool PDFScanner::ValidatePDF(const QString &pdfPath)
{
    // For simplicity, we'll check if the PDF exists and if it is smaller than a threshold size (to avoid image-heavy PDFs).
    QFile file(pdfPath);
    if (!file.exists()) {
        qDebug() << "❌ File does not exist!";
        return false;
    }

    // Optionally, check if the file size is too large (image-heavy PDFs may be large)
    if (file.size() > 10 * 1024 * 1024) {  // 10MB limit, you can adjust
        qDebug() << "❌ File is too large, may contain too many images!";
        return false;
    }

    // You can further check for image-heavy PDFs by parsing the document, but for now, this is a basic check.
    return true;
}

bool PDFScanner::ScanPDF(const QString &pdfPath)
{
    if (!ValidatePDF(pdfPath)) {
        return false;
    }

    QString txtPath = OUTPUT_TXT_PATH;
    //QString pdftotextPath = QDir::cleanPath(QDir::currentPath() + "/debug/popler/bin/pdftotext.exe");
    QString pdftotextPath = QDir::currentPath() + "/../popler/bin/pdftotext.exe";
    QProcess process;
    QStringList arguments;
    arguments << pdfPath << "-";  // "-" outputs to stdout
    qDebug() << "Running:" << pdftotextPath;
    qDebug() << "Arguments:" << arguments;

    process.start(pdftotextPath, arguments);

    if (!process.waitForFinished()) {
        qDebug() << "❌ pdftotext did not finish properly.";
        qDebug() << "Error:" << process.errorString();
        return false;
    }

    QByteArray output = process.readAllStandardOutput();

    if (output.isEmpty()) {
        qDebug() << "❌ pdftotext returned no output.";
        qDebug() << "StdErr:" << process.readAllStandardError();
        return false;
    }

    QFile txtFile(txtPath);
    if (txtFile.open(QIODevice::WriteOnly | QIODevice::Text)) {
        QTextStream out(&txtFile);
        out << output;
        txtFile.close();
        qDebug() << "✅ Text extracted and saved to:" << txtPath;
        return true;
    } else {
        qDebug() << "❌ Could not write to:" << txtPath;
        return false;
    }
}

QString PDFScanner::GetOutputPath(){

    return QCoreApplication::applicationDirPath() + "/scanned_output.txt";
}
