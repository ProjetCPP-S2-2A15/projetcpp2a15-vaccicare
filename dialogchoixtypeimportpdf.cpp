#include "dialogchoixtypeimportpdf.h"
#include "ui_dialogchoixtypeimportpdf.h"
#include "ui_dialogchoixtypeimportpdf.h"
#include "Design.h"

DialogChoixTypeImportPDF::DialogChoixTypeImportPDF(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DialogChoixTypeImportPDF)
{
    ui->setupUi(this);

    connect(ui->ButtonGenerer,&QPushButton::clicked,this,&DialogChoixTypeImportPDF::GenererPDF);
    connect(ui->ButtonRetour,&QPushButton::clicked,this,&DialogChoixTypeImportPDF::ExitDialog);
    connect(ui->SelectPath,&QPushButton::clicked,this,&DialogChoixTypeImportPDF::SelectPath);

    SetUpUI();
    setupDesign();

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
        GenererPDFPatient("testPatient.pdf");
    }

    if(ui->checkBoxProjet->isChecked()){
        GenererPDFProjet("testProjet.pdf",ui->dateEditStart->date(),ui->dateEditEnd->date());
    }

    if(ui->checkBoxRessource->isChecked()){
        GenererPDFRessource("testRessource.pdf");
    }

    if(ui->checkBoxVaccin->isChecked()){
        GenererPDFVaccin("testVaccin.pdf");
    }
    close();
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

bool DialogChoixTypeImportPDF::GenererPDFMedecin(QString fileName){
    QAbstractItemModel* model = Medecin::GetDataByDate();

    QString strStream;
    QTextStream out(&strStream);

    int rowCount = model->rowCount();
    int columnCount = model->columnCount();
    out << "<html>\n"
           "<head>\n"
           "<meta Content=\"Text/html; charset=Windows-1251\">\n"
        << QString("<title>%1</title>\n").arg("Export PDF")
        << "</head>\n"
           "<body bgcolor=#ffffff link=#5000A0>\n"
           "<center><h1>Liste des Recherches</h1><br><br>\n"
           "<table border=1 cellspacing=0 cellpadding=2>\n";

    // Table headers
    out << "<thead><tr bgcolor=#f0f0f0><th>Numéro</th>";
    for (int column = 0; column < columnCount; ++column) {
        out << QString("<th>%1</th>").arg(
                   model->headerData(column, Qt::Horizontal).toString());
    }
    out << "</tr></thead>\n";

    // Table data
    for (int row = 0; row < rowCount; ++row) {
        out << "<tr><td>" << row + 1 << "</td>";
        for (int column = 0; column < columnCount; ++column) {
            QString data = model
                    ->data(model->index(row, column))
                    .toString()
                    .simplified();
            out << QString("<td>%1</td>").arg(data.isEmpty() ? "&nbsp;" : data);
        }
        out << "</tr>\n";
    }

    out << "</table></center>\n</body>\n</html>\n";

    QString fullFilePath = QDir(filePath).filePath(fileName);
    QPrinter printer(QPrinter::HighResolution);
    printer.setOutputFormat(QPrinter::PdfFormat);
    printer.setPageSize(QPageSize(QPageSize::A4));
    printer.setOutputFileName(fullFilePath);

    QTextDocument doc;
    doc.setHtml(strStream);
    doc.print(&printer);
    return true;
}

bool DialogChoixTypeImportPDF::GenererPDFPatient(QString fileName){
    QString strStream;
    QTextStream out(&strStream);

    QAbstractItemModel* model = Patient::GetDataForPDF();
    int rowCount = model->rowCount();
    int columnCount = model->columnCount();
    out << "<html>\n"
           "<head>\n"
           "<meta Content=\"Text/html; charset=Windows-1251\">\n"
        << QString("<title>%1</title>\n").arg("Export PDF")
        << "</head>\n"
           "<body bgcolor=#ffffff link=#5000A0>\n"
           "<center><h1>Liste des Recherches</h1><br><br>\n"
           "<table border=1 cellspacing=0 cellpadding=2>\n";

    // Table headers
    out << "<thead><tr bgcolor=#f0f0f0><th>Numéro</th>";
    for (int column = 0; column < columnCount; ++column) {
        out << QString("<th>%1</th>").arg(
                   model->headerData(column, Qt::Horizontal).toString());
    }
    out << "</tr></thead>\n";

    // Table data
    for (int row = 0; row < rowCount; ++row) {
        out << "<tr><td>" << row + 1 << "</td>";
        for (int column = 0; column < columnCount; ++column) {
            QString data = model
                    ->data(model->index(row, column))
                    .toString()
                    .simplified();
            out << QString("<td>%1</td>").arg(data.isEmpty() ? "&nbsp;" : data);
        }
        out << "</tr>\n";
    }

    out << "</table></center>\n</body>\n</html>\n";

    QString fullFilePath = QDir(filePath).filePath(fileName);
    QPrinter printer(QPrinter::HighResolution);
    printer.setOutputFormat(QPrinter::PdfFormat);
    printer.setPageSize(QPageSize(QPageSize::A4));
    printer.setOutputFileName(fullFilePath);

    QTextDocument doc;
    doc.setHtml(strStream);
    doc.print(&printer);
    return true;

}

bool DialogChoixTypeImportPDF::GenererPDFRessource(QString fileName){
    QAbstractItemModel* model = Resources::GetDataForPDF();

    QString strStream;
    QTextStream out(&strStream);

    int rowCount = model->rowCount();
    int columnCount = model->columnCount();
    out << "<html>\n"
           "<head>\n"
           "<meta Content=\"Text/html; charset=Windows-1251\">\n"
        << QString("<title>%1</title>\n").arg("Export PDF")
        << "</head>\n"
           "<body bgcolor=#ffffff link=#5000A0>\n"
           "<center><h1>Liste des Recherches</h1><br><br>\n"
           "<table border=1 cellspacing=0 cellpadding=2>\n";

    // Table headers
    out << "<thead><tr bgcolor=#f0f0f0><th>Numéro</th>";
    for (int column = 0; column < columnCount; ++column) {
        out << QString("<th>%1</th>").arg(
                   model->headerData(column, Qt::Horizontal).toString());
    }
    out << "</tr></thead>\n";

    // Table data
    for (int row = 0; row < rowCount; ++row) {
        out << "<tr><td>" << row + 1 << "</td>";
        for (int column = 0; column < columnCount; ++column) {
            QString data = model
                    ->data(model->index(row, column))
                    .toString()
                    .simplified();
            out << QString("<td>%1</td>").arg(data.isEmpty() ? "&nbsp;" : data);
        }
        out << "</tr>\n";
    }

    out << "</table></center>\n</body>\n</html>\n";

    QString fullFilePath = QDir(filePath).filePath(fileName);
    QPrinter printer(QPrinter::HighResolution);
    printer.setOutputFormat(QPrinter::PdfFormat);
    printer.setPageSize(QPageSize(QPageSize::A4));
    printer.setOutputFileName(fullFilePath);

    QTextDocument doc;
    doc.setHtml(strStream);
    doc.print(&printer);
    return true;
}

bool DialogChoixTypeImportPDF::GenererPDFVaccin(QString fileName){
    QAbstractItemModel* model = Vaccin::GetDataForPDF();

    QString strStream;
    QTextStream out(&strStream);

    int rowCount = model->rowCount();
    int columnCount = model->columnCount();
    out << "<html>\n"
           "<head>\n"
           "<meta Content=\"Text/html; charset=Windows-1251\">\n"
        << QString("<title>%1</title>\n").arg("Export PDF")
        << "</head>\n"
           "<body bgcolor=#ffffff link=#5000A0>\n"
           "<center><h1>Liste des Recherches</h1><br><br>\n"
           "<table border=1 cellspacing=0 cellpadding=2>\n";

    // Table headers
    out << "<thead><tr bgcolor=#f0f0f0><th>Numéro</th>";
    for (int column = 0; column < columnCount; ++column) {
        out << QString("<th>%1</th>").arg(
                   model->headerData(column, Qt::Horizontal).toString());
    }
    out << "</tr></thead>\n";

    // Table data
    for (int row = 0; row < rowCount; ++row) {
        out << "<tr><td>" << row + 1 << "</td>";
        for (int column = 0; column < columnCount; ++column) {
            QString data = model
                    ->data(model->index(row, column))
                    .toString()
                    .simplified();
            out << QString("<td>%1</td>").arg(data.isEmpty() ? "&nbsp;" : data);
        }
        out << "</tr>\n";
    }

    out << "</table></center>\n</body>\n</html>\n";

    QString fullFilePath = QDir(filePath).filePath(fileName);
    QPrinter printer(QPrinter::HighResolution);
    printer.setOutputFormat(QPrinter::PdfFormat);
    printer.setPageSize(QPageSize(QPageSize::A4));
    printer.setOutputFileName(fullFilePath);


    QTextDocument doc;
    doc.setHtml(strStream);
    doc.print(&printer);
    return true;
}

void DialogChoixTypeImportPDF::setupDesign() {

    StyleLineEdit(ui->lineEditPath);

    StyleButtonGreen(ui->ButtonGenerer);
    StyleButtonRed(ui->ButtonRetour);
    StyleButtonDarkblue(ui->SelectPath);

    StyleCheckBox(ui->checkBoxMedecin);
    StyleCheckBox(ui->checkBoxPatient);
    StyleCheckBox(ui->checkBoxProjet);
    StyleCheckBox(ui->checkBoxVaccin);
    StyleCheckBox(ui->checkBoxRessource);

    StyleDateEdit(ui->dateEditStart);
    StyleDateEdit(ui->dateEditEnd);

}

