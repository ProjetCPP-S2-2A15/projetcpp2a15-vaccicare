#include "dialogchoixtypeimportpdf.h"
#include "ui_dialogchoixtypeimportpdf.h"

DialogChoixTypeImportPDF::DialogChoixTypeImportPDF(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DialogChoixTypeImportPDF)
{
    ui->setupUi(this);

    connect(ui->ButtonGenerer,&QPushButton::clicked,this,&DialogChoixTypeImportPDF::GenererPDF);
    connect(ui->ButtonRetour,&QPushButton::clicked,this,&DialogChoixTypeImportPDF::ExitDialog);
    connect(ui->SelectPath,&QPushButton::clicked,this,&DialogChoixTypeImportPDF::SelectPath);

    SetUpUI();
}

DialogChoixTypeImportPDF::~DialogChoixTypeImportPDF()
{
    delete ui;
}

void DialogChoixTypeImportPDF::SetUpUI(){

    QDate today = QDate::currentDate();
    QDate oneMonthAgo = today.addMonths(-1);

    ui->dateEditStart->setDate(oneMonthAgo);
    ui->dateEditEnd->setDate(today);


    const QString DEFAULT_EXPORT_PATH = QStandardPaths::writableLocation(QStandardPaths::DocumentsLocation);
    ui->lineEditPath->setText(DEFAULT_EXPORT_PATH);
    filePath = DEFAULT_EXPORT_PATH;
}

void DialogChoixTypeImportPDF::GenererPDF(){
    // === CollectDataFromDialog ===
        QDate startDate = ui->dateEditStart->date();
        QDate endDate = ui->dateEditEnd->date();
        QString filePath = ui->lineEditPath->text().trimmed();

        // === ValidateData ===
        if (startDate > endDate) {
            QMessageBox::warning(this, "Invalid Date Range", "The start date must be before or equal to the end date.");
            return;
        }

        if (filePath.isEmpty()) {
            QMessageBox::warning(this, "Missing File Path", "Please provide a valid export path.");
            return;
        }

        QDir dir(filePath);
        if (!dir.exists()) {
            QMessageBox::warning(this, "Invalid Path", "The export path does not exist.");
            return;
        }

        if(ui->checkBoxMedecin->isChecked()){
            GenererPDFMedecin("testMedecin.pdf");
        }

        if(ui->checkBoxPatient->isChecked()){
            GenererPDFPatient("testPatient.pdf",ui->dateEditStart->date(),ui->dateEditEnd->date());
        }

        if(ui->checkBoxProjet->isChecked()){
            GenererPDFProjet("testProjet.pdf",ui->dateEditStart->date(),ui->dateEditEnd->date());
        }

        if(ui->checkBoxRessource->isChecked()){
            GenererPDFRessource("testRessource.pdf",ui->dateEditStart->date(),ui->dateEditEnd->date());
        }

        if(ui->checkBoxVaccin->isChecked()){
            GenererPDFVaccin("testVaccin.pdf",ui->dateEditStart->date(),ui->dateEditEnd->date());
        }

}

void DialogChoixTypeImportPDF::ExitDialog(){
    close();
}

void DialogChoixTypeImportPDF::SelectPath(){
    QString dir = QFileDialog::getExistingDirectory(this, tr("Select Export Folder"),
                                                        QStandardPaths::writableLocation(QStandardPaths::DocumentsLocation),
                                                        QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks);
        if (!dir.isEmpty()) {
            ui->lineEditPath->setText(dir);
        }
        filePath = dir;
}

//
bool DialogChoixTypeImportPDF::GenererPDFProjet(QString fileName,QDate startDate,QDate endDate){
    // Generate full file path (add .pdf extension if not already there)
        if (!fileName.endsWith(".pdf", Qt::CaseInsensitive))
            fileName += ".pdf";

        QString fullFilePath = QDir(filePath).filePath(fileName);

        //Collect Project List From DATABASE
        int NumOrdreDebut = Date::ConvertDateToInt(startDate);
        int NumOrdreEnd = Date::ConvertDateToInt(endDate);

        std::vector<Projet> ListProjet = Projet::ReadProjectListFromDB(true,"","");

        if(ListProjet.size() == 0){
            QMessageBox::warning(this, "Database Error", "There was an error while fetching data from the database");
            return false;
        }
        std::vector<Projet> filteredList;

        for(size_t index=0;index < ListProjet.size();index++){
            if(ListProjet[index].Data.DateDebut <= NumOrdreEnd && ListProjet[index].Data.DateDebut >= NumOrdreDebut){
                filteredList.push_back(ListProjet[index]);
            }
        }

        if(filteredList.size() == 0){
            QMessageBox::warning(this, "Empty File", "No Data for the Selected Period");
            return false;
        }

        // === Export TO PDF ===
        QPdfWriter writer(fullFilePath);
        writer.setPageSize(QPageSize::A4);
        writer.setResolution(300);

        QPainter painter(&writer);
        if (!painter.isActive()) {
            QMessageBox::critical(this, "Export Error", "Could not open PDF file for writing.");
            return false;
        }

        // Use custom column widths based on your margin layout
        int colWidths[] = {COL_ID_PROJET_WIDTH, COL_NOM_WIDTH, COL_DATE_DEBUT_WIDTH, COL_ID_RESPONSABLE_WIDTH, COL_BUDGET_WIDTH, COL_DATE_FIN_WIDTH};
        QString headers[] = {"ID_PROJET", "NOM", "DATE_DEBUT", "RESPONSABLE", "BUDGET", "DATE_FIN"};
        const int numCols = sizeof(colWidths) / sizeof(int);

        QFont font("Times", 10);
        QFont boldFont("Times", 10, QFont::Bold);
        painter.setFont(boldFont);

        int startX = 50;
        int startY = 100;
        int rowHeight = ROW_HEIGHT;

        int y = startY;

        // Draw header
        int x = startX;
        for (int i = 0; i < numCols; ++i) {
            painter.drawRect(x, y, colWidths[i], rowHeight);
            painter.drawText(x + 10, y + 40, headers[i]);  // Vertical padding
            x += colWidths[i];
        }

        y += rowHeight;
        painter.setFont(font);

        // Draw each row
        for (const auto& proj : filteredList) {
            const auto& data = proj.Data;

            QString dateDebutStr = Date::ConvertIntToDate(data.DateDebut).toString("dd/MM/yyyy");
            QString dateFinStr = (data.DateFin == -1)
                ? "Not set"
                : Date::ConvertIntToDate(data.DateFin).toString("dd/MM/yyyy");

            QString rowData[] = {
                QString::number(data.ID_Projet),
                data.Nom,
                dateDebutStr,
                QString::number(data.Id_Responsable),
                QString::number(data.Budget),
                dateFinStr
            };

            int x = startX;
            for (int col = 0; col < numCols; ++col) {
                painter.drawRect(x, y, colWidths[col], rowHeight);
                painter.drawText(x + 10, y + 40, rowData[col]);
                x += colWidths[col];
            }

            y += rowHeight;

            // Page break check
            if (y + rowHeight > writer.height() - 100) {
                writer.newPage();
                y = startY;

                // Repeat header on new page
                painter.setFont(boldFont);
                x = startX;
                for (int i = 0; i < numCols; ++i) {
                    painter.drawRect(x, y, colWidths[i], rowHeight);
                    painter.drawText(x + 10, y + 40, headers[i]);
                    x += colWidths[i];
                }

                y += rowHeight;
                painter.setFont(font);
            }
        }

        painter.end();
        return true;
}
//
bool DialogChoixTypeImportPDF::GenererPDFMedecin(QString fileName){
        QAbstractItemModel* model = Medecin::GetDataByDate();

        if (!fileName.endsWith(".pdf", Qt::CaseInsensitive)) {
            fileName += ".pdf";
        }

        QString fullFilePath = QDir(filePath).filePath(fileName);
        QPdfWriter pdfWriter(fullFilePath);
        QPageLayout layout(QPageSize(QPageSize::A4), QPageLayout::Landscape, QMarginsF(20, 20, 20, 20), QPageLayout::Millimeter);
        pdfWriter.setPageLayout(layout);

        QPainter painter(&pdfWriter);
        painter.setRenderHint(QPainter::Antialiasing);

        const int pageWidth = pdfWriter.width();
        const int pageHeight = pdfWriter.height();
        const int margin = 20 * 72 / 25.4;
        const int rowHeight = 30;
        const int columnCount = model->columnCount();
        const int rowCount = model->rowCount();

        QFont font("Arial", 7);
        painter.setFont(font);
        QFontMetrics fontMetrics(font, &pdfWriter);

        QVector<int> columnWidths(columnCount, 0);
        int totalContentWidth = 0;
        for (int col = 0; col < columnCount; ++col) {
            QString headerText = model->headerData(col, Qt::Horizontal).toString();
            int headerWidth = fontMetrics.boundingRect(headerText).width() + 20;

            int maxCellWidth = headerWidth;
            for (int row = 0; row < rowCount; ++row) {
                QString cellText = model->data(model->index(row, col)).toString();
                int cellWidth = fontMetrics.boundingRect(cellText).width() + 20;
                maxCellWidth = qMax(maxCellWidth, cellWidth);
            }
            columnWidths[col] = maxCellWidth;
            totalContentWidth += maxCellWidth;
        }

        int availableWidth = pageWidth - 2 * margin;
        if (totalContentWidth > availableWidth) {
            double scaleFactor = static_cast<double>(availableWidth) / totalContentWidth;
            for (int col = 0; col < columnCount; ++col) {
                columnWidths[col] = static_cast<int>(columnWidths[col] * scaleFactor);
            }
            totalContentWidth = availableWidth;
        }

        QFont titleFont("Arial", 12, QFont::Bold);
        painter.setFont(titleFont);
        painter.setPen(Qt::black);
        QString title = "Liste des Médecins";
        int titleHeight = fontMetrics.height();
        painter.drawText(margin, margin, title);

        painter.setFont(font);

        int y = margin + titleHeight + 20;

        painter.setPen(Qt::black);
        painter.setBrush(QBrush(Qt::lightGray));
        int x = margin;
        for (int col = 0; col < columnCount; ++col) {
            painter.drawRect(x, y, columnWidths[col], rowHeight);
            QString headerText = model->headerData(col, Qt::Horizontal).toString();
            QString displayText = fontMetrics.elidedText(headerText, Qt::ElideRight, columnWidths[col] - 20);
            int textY = y + (rowHeight - fontMetrics.height()) / 2 + fontMetrics.ascent();
            painter.drawText(x + 10, textY, displayText);
            x += columnWidths[col];
        }
        y += rowHeight;

        painter.setBrush(Qt::NoBrush);
        for (int row = 0; row < rowCount; ++row) {
            if (y + rowHeight > pageHeight - margin) {
                pdfWriter.newPage();
                y = margin;
                painter.setBrush(QBrush(Qt::lightGray));
                x = margin;
                for (int col = 0; col < columnCount; ++col) {
                    painter.drawRect(x, y, columnWidths[col], rowHeight);
                    QString headerText = model->headerData(col, Qt::Horizontal).toString();
                    QString displayText = fontMetrics.elidedText(headerText, Qt::ElideRight, columnWidths[col] - 20);
                    int textY = y + (rowHeight - fontMetrics.height()) / 2 + fontMetrics.ascent();
                    painter.drawText(x + 10, textY, displayText);
                    x += columnWidths[col];
                }
                y += rowHeight;
                painter.setBrush(Qt::NoBrush);
            }

            x = margin;
            for (int col = 0; col < columnCount; ++col) {
                painter.drawRect(x, y, columnWidths[col], rowHeight);
                QString cellText = model->data(model->index(row, col)).toString();
                QString displayText = fontMetrics.elidedText(cellText, Qt::ElideRight, columnWidths[col] - 20);
                int textY = y + (rowHeight - fontMetrics.height()) / 2 + fontMetrics.ascent();
                painter.drawText(x + 10, textY, displayText);
                x += columnWidths[col];
            }
            y += rowHeight;
        }

        // Ajout d'une note de bas de page
        painter.setFont(font);
        QString footerText = "Généré par l'application de gestion des médecins - " + QDateTime::currentDateTime().toString("dd/MM/yyyy HH:mm");
        QString footerDisplayText = fontMetrics.elidedText(footerText, Qt::ElideRight, pageWidth - 2 * margin);
        painter.drawText(margin, pageHeight - margin, footerDisplayText);

        painter.end();

        return true;
}

bool DialogChoixTypeImportPDF::GenererPDFPatient(QString fileName,QDate startDate,QDate endDate){
    // Generate full file path (add .pdf extension if not already there)
        if (!fileName.endsWith(".pdf", Qt::CaseInsensitive))
            fileName += ".pdf";

        QString fullFilePath = QDir(filePath).filePath(fileName);

        //Collect Project List From DATABASE
        int NumOrdreDebut = Date::ConvertDateToInt(startDate);
        int NumOrdreEnd = Date::ConvertDateToInt(endDate);

        std::vector<Projet> ListProjet = Projet::ReadProjectListFromDB(true,"","");

        if(ListProjet.size() == 0){
            QMessageBox::warning(this, "Database Error", "There was an error while fetching data from the database");
            return false;
        }
        std::vector<Projet> filteredList;

        for(size_t index=0;index < ListProjet.size();index++){
            if(ListProjet[index].Data.DateDebut <= NumOrdreEnd && ListProjet[index].Data.DateDebut >= NumOrdreDebut){
                filteredList.push_back(ListProjet[index]);
            }
        }

        if(filteredList.size() == 0){
            QMessageBox::warning(this, "Empty File", "No Data for the Selected Period");
            return false;
        }

        // === Export TO PDF ===
        QPdfWriter writer(fullFilePath);
        writer.setPageSize(QPageSize::A4);
        writer.setResolution(300);

        QPainter painter(&writer);
        if (!painter.isActive()) {
            QMessageBox::critical(this, "Export Error", "Could not open PDF file for writing.");
            return false;
        }

        // Use custom column widths based on your margin layout
        int colWidths[] = {COL_ID_PROJET_WIDTH, COL_NOM_WIDTH, COL_DATE_DEBUT_WIDTH, COL_ID_RESPONSABLE_WIDTH, COL_BUDGET_WIDTH, COL_DATE_FIN_WIDTH};
        QString headers[] = {"ID_PROJET", "NOM", "DATE_DEBUT", "RESPONSABLE", "BUDGET", "DATE_FIN"};
        const int numCols = sizeof(colWidths) / sizeof(int);

        QFont font("Times", 10);
        QFont boldFont("Times", 10, QFont::Bold);
        painter.setFont(boldFont);

        int startX = 50;
        int startY = 100;
        int rowHeight = ROW_HEIGHT;

        int y = startY;

        // Draw header
        int x = startX;
        for (int i = 0; i < numCols; ++i) {
            painter.drawRect(x, y, colWidths[i], rowHeight);
            painter.drawText(x + 10, y + 40, headers[i]);  // Vertical padding
            x += colWidths[i];
        }

        y += rowHeight;
        painter.setFont(font);

        // Draw each row
        for (const auto& proj : filteredList) {
            const auto& data = proj.Data;

            QString dateDebutStr = Date::ConvertIntToDate(data.DateDebut).toString("dd/MM/yyyy");
            QString dateFinStr = (data.DateFin == -1)
                ? "Not set"
                : Date::ConvertIntToDate(data.DateFin).toString("dd/MM/yyyy");

            QString rowData[] = {
                QString::number(data.ID_Projet),
                data.Nom,
                dateDebutStr,
                QString::number(data.Id_Responsable),
                QString::number(data.Budget),
                dateFinStr
            };

            int x = startX;
            for (int col = 0; col < numCols; ++col) {
                painter.drawRect(x, y, colWidths[col], rowHeight);
                painter.drawText(x + 10, y + 40, rowData[col]);
                x += colWidths[col];
            }

            y += rowHeight;

            // Page break check
            if (y + rowHeight > writer.height() - 100) {
                writer.newPage();
                y = startY;

                // Repeat header on new page
                painter.setFont(boldFont);
                x = startX;
                for (int i = 0; i < numCols; ++i) {
                    painter.drawRect(x, y, colWidths[i], rowHeight);
                    painter.drawText(x + 10, y + 40, headers[i]);
                    x += colWidths[i];
                }

                y += rowHeight;
                painter.setFont(font);
            }
        }

        painter.end();
        return true;
}

bool DialogChoixTypeImportPDF::GenererPDFRessource(QString fileName,QDate startDate,QDate endDate){
    // Generate full file path (add .pdf extension if not already there)
        if (!fileName.endsWith(".pdf", Qt::CaseInsensitive))
            fileName += ".pdf";

        QString fullFilePath = QDir(filePath).filePath(fileName);

        //Collect Project List From DATABASE
        int NumOrdreDebut = Date::ConvertDateToInt(startDate);
        int NumOrdreEnd = Date::ConvertDateToInt(endDate);

        std::vector<Projet> ListProjet = Projet::ReadProjectListFromDB(true,"","");

        if(ListProjet.size() == 0){
            QMessageBox::warning(this, "Database Error", "There was an error while fetching data from the database");
            return false;
        }
        std::vector<Projet> filteredList;

        for(size_t index=0;index < ListProjet.size();index++){
            if(ListProjet[index].Data.DateDebut <= NumOrdreEnd && ListProjet[index].Data.DateDebut >= NumOrdreDebut){
                filteredList.push_back(ListProjet[index]);
            }
        }

        if(filteredList.size() == 0){
            QMessageBox::warning(this, "Empty File", "No Data for the Selected Period");
            return false;
        }

        // === Export TO PDF ===
        QPdfWriter writer(fullFilePath);
        writer.setPageSize(QPageSize::A4);
        writer.setResolution(300);

        QPainter painter(&writer);
        if (!painter.isActive()) {
            QMessageBox::critical(this, "Export Error", "Could not open PDF file for writing.");
            return false;
        }

        // Use custom column widths based on your margin layout
        int colWidths[] = {COL_ID_PROJET_WIDTH, COL_NOM_WIDTH, COL_DATE_DEBUT_WIDTH, COL_ID_RESPONSABLE_WIDTH, COL_BUDGET_WIDTH, COL_DATE_FIN_WIDTH};
        QString headers[] = {"ID_PROJET", "NOM", "DATE_DEBUT", "RESPONSABLE", "BUDGET", "DATE_FIN"};
        const int numCols = sizeof(colWidths) / sizeof(int);

        QFont font("Times", 10);
        QFont boldFont("Times", 10, QFont::Bold);
        painter.setFont(boldFont);

        int startX = 50;
        int startY = 100;
        int rowHeight = ROW_HEIGHT;

        int y = startY;

        // Draw header
        int x = startX;
        for (int i = 0; i < numCols; ++i) {
            painter.drawRect(x, y, colWidths[i], rowHeight);
            painter.drawText(x + 10, y + 40, headers[i]);  // Vertical padding
            x += colWidths[i];
        }

        y += rowHeight;
        painter.setFont(font);

        // Draw each row
        for (const auto& proj : filteredList) {
            const auto& data = proj.Data;

            QString dateDebutStr = Date::ConvertIntToDate(data.DateDebut).toString("dd/MM/yyyy");
            QString dateFinStr = (data.DateFin == -1)
                ? "Not set"
                : Date::ConvertIntToDate(data.DateFin).toString("dd/MM/yyyy");

            QString rowData[] = {
                QString::number(data.ID_Projet),
                data.Nom,
                dateDebutStr,
                QString::number(data.Id_Responsable),
                QString::number(data.Budget),
                dateFinStr
            };

            int x = startX;
            for (int col = 0; col < numCols; ++col) {
                painter.drawRect(x, y, colWidths[col], rowHeight);
                painter.drawText(x + 10, y + 40, rowData[col]);
                x += colWidths[col];
            }

            y += rowHeight;

            // Page break check
            if (y + rowHeight > writer.height() - 100) {
                writer.newPage();
                y = startY;

                // Repeat header on new page
                painter.setFont(boldFont);
                x = startX;
                for (int i = 0; i < numCols; ++i) {
                    painter.drawRect(x, y, colWidths[i], rowHeight);
                    painter.drawText(x + 10, y + 40, headers[i]);
                    x += colWidths[i];
                }

                y += rowHeight;
                painter.setFont(font);
            }
        }

        painter.end();
        return true;
}

bool DialogChoixTypeImportPDF::GenererPDFVaccin(QString fileName,QDate startDate,QDate endDate){
    // Generate full file path (add .pdf extension if not already there)
        if (!fileName.endsWith(".pdf", Qt::CaseInsensitive))
            fileName += ".pdf";

        QString fullFilePath = QDir(filePath).filePath(fileName);

        //Collect Project List From DATABASE
        int NumOrdreDebut = Date::ConvertDateToInt(startDate);
        int NumOrdreEnd = Date::ConvertDateToInt(endDate);

        std::vector<Projet> ListProjet = Projet::ReadProjectListFromDB(true,"","");

        if(ListProjet.size() == 0){
            QMessageBox::warning(this, "Database Error", "There was an error while fetching data from the database");
            return false;
        }
        std::vector<Projet> filteredList;

        for(size_t index=0;index < ListProjet.size();index++){
            if(ListProjet[index].Data.DateDebut <= NumOrdreEnd && ListProjet[index].Data.DateDebut >= NumOrdreDebut){
                filteredList.push_back(ListProjet[index]);
            }
        }

        if(filteredList.size() == 0){
            QMessageBox::warning(this, "Empty File", "No Data for the Selected Period");
            return false;
        }

        // === Export TO PDF ===
        QPdfWriter writer(fullFilePath);
        writer.setPageSize(QPageSize::A4);
        writer.setResolution(300);

        QPainter painter(&writer);
        if (!painter.isActive()) {
            QMessageBox::critical(this, "Export Error", "Could not open PDF file for writing.");
            return false;
        }

        // Use custom column widths based on your margin layout
        int colWidths[] = {COL_ID_PROJET_WIDTH, COL_NOM_WIDTH, COL_DATE_DEBUT_WIDTH, COL_ID_RESPONSABLE_WIDTH, COL_BUDGET_WIDTH, COL_DATE_FIN_WIDTH};
        QString headers[] = {"ID_PROJET", "NOM", "DATE_DEBUT", "RESPONSABLE", "BUDGET", "DATE_FIN"};
        const int numCols = sizeof(colWidths) / sizeof(int);

        QFont font("Times", 10);
        QFont boldFont("Times", 10, QFont::Bold);
        painter.setFont(boldFont);

        int startX = 50;
        int startY = 100;
        int rowHeight = ROW_HEIGHT;

        int y = startY;

        // Draw header
        int x = startX;
        for (int i = 0; i < numCols; ++i) {
            painter.drawRect(x, y, colWidths[i], rowHeight);
            painter.drawText(x + 10, y + 40, headers[i]);  // Vertical padding
            x += colWidths[i];
        }

        y += rowHeight;
        painter.setFont(font);

        // Draw each row
        for (const auto& proj : filteredList) {
            const auto& data = proj.Data;

            QString dateDebutStr = Date::ConvertIntToDate(data.DateDebut).toString("dd/MM/yyyy");
            QString dateFinStr = (data.DateFin == -1)
                ? "Not set"
                : Date::ConvertIntToDate(data.DateFin).toString("dd/MM/yyyy");

            QString rowData[] = {
                QString::number(data.ID_Projet),
                data.Nom,
                dateDebutStr,
                QString::number(data.Id_Responsable),
                QString::number(data.Budget),
                dateFinStr
            };

            int x = startX;
            for (int col = 0; col < numCols; ++col) {
                painter.drawRect(x, y, colWidths[col], rowHeight);
                painter.drawText(x + 10, y + 40, rowData[col]);
                x += colWidths[col];
            }

            y += rowHeight;

            // Page break check
            if (y + rowHeight > writer.height() - 100) {
                writer.newPage();
                y = startY;

                // Repeat header on new page
                painter.setFont(boldFont);
                x = startX;
                for (int i = 0; i < numCols; ++i) {
                    painter.drawRect(x, y, colWidths[i], rowHeight);
                    painter.drawText(x + 10, y + 40, headers[i]);
                    x += colWidths[i];
                }

                y += rowHeight;
                painter.setFont(font);
            }
        }

        painter.end();
        return true;
}
