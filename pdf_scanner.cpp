#include "pdf_scanner.h"

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

    QString txtPath = OUTPUT_TXT_PATH;  // Save the output text file in the same directory as the .exe

    // Run pdftotext using QProcess
    QProcess process;
    QString program = "pdftotext";
    QStringList arguments;
    arguments << pdfPath << txtPath;

    process.start(program, arguments);
    process.waitForFinished();

    // Check if the .txt file was created successfully
    if (QFile::exists(txtPath)) {
        qDebug() << "✅ Text file created at:" << txtPath;

        // Optionally, you can read and verify the contents of the .txt file here.
        QFile txtFile(txtPath);
        if (txtFile.open(QIODevice::ReadOnly | QIODevice::Text)) {
            QTextStream in(&txtFile);
            QString content = in.readAll();
            qDebug() << "📄 Extracted text:\n" << content.left(500);  // Display first 500 chars
            txtFile.close();
        }
        return true;
    } else {
        // If the file doesn't exist, create it manually
        QFile txtFile(txtPath);
        if (txtFile.open(QIODevice::WriteOnly | QIODevice::Text)) {
            QTextStream out(&txtFile);
            out << "";  // Write an empty string to create the file
            txtFile.close();
            qDebug() << "❌ Text file not found, created an empty one at:" << txtPath;
            return false;
        } else {
            qDebug() << "❌ Failed to create empty text file at:" << txtPath;
            return false;
        }
    }
}
