#include "exportdialog.h"
#include "ui_exportdialog.h"

ExportDialog::ExportDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ExportDialog)
{
    ui->setupUi(this);
    connect(ui->PushButtonAnnuler,&QPushButton::clicked,this,&ExportDialog::Annuler);
    connect(ui->PushButtonValider,&QPushButton::clicked,this,&ExportDialog::Valider);
    connect(ui->PushButtonSelectFolder,&QPushButton::clicked,this,&ExportDialog::SelectFolder);

    SetUpUI();
    SetupDesign();
}

ExportDialog::~ExportDialog()
{
    delete ui;
}

void ExportDialog::SetupDesign(){
    //Design For Buttons
    StyleButton(ui->PushButtonValider);
    StyleButton(ui->PushButtonAnnuler);

    //Design For LineEdits
    StyleLineEdit(ui->lineEditPath);
    StyleLineEdit(ui->lineEditFileName);

    //Design For DateEdit
    StyleDateEdit(ui->dateEditEnd);
    StyleDateEdit(ui->dateEditStart);

    //Design For Labels
    StyleLabel(ui->label);
    StyleLabel(ui->label_2);
    StyleLabel(ui->label_3);
    StyleLabel(ui->label_4);

}

void ExportDialog::SelectFolder(){
    QString dir = QFileDialog::getExistingDirectory(this, tr("Select Export Folder"),
                                                    QStandardPaths::writableLocation(QStandardPaths::DocumentsLocation),
                                                    QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks);
    if (!dir.isEmpty()) {
        ui->lineEditPath->setText(dir);
    }
}

void ExportDialog::Annuler(){
    this->reject();  // Close the dialog and return QDialog::Rejected
}

void ExportDialog::SetUpUI(){

    QDate today = QDate::currentDate();
    QDate oneMonthAgo = today.addMonths(-1);

    ui->dateEditStart->setDate(oneMonthAgo);
    ui->dateEditEnd->setDate(today);

    QString fileName = "ListeProjet_" + today.toString("yyyyMMdd");
    ui->lineEditFileName->setText(fileName);

    const QString DEFAULT_EXPORT_PATH = QStandardPaths::writableLocation(QStandardPaths::DocumentsLocation);
    ui->lineEditPath->setText(DEFAULT_EXPORT_PATH);
}

void ExportDialog::Valider(){
    // === CollectDataFromDialog ===
    QDate startDate = ui->dateEditStart->date();
    QDate endDate = ui->dateEditEnd->date();
    QString fileName = ui->lineEditFileName->text().trimmed();
    QString filePath = ui->lineEditPath->text().trimmed();

    // === ValidateData ===
    if (startDate > endDate) {
        QMessageBox::warning(this, "Invalid Date Range", "The start date must be before or equal to the end date.");
        return;
    }

    if (fileName.isEmpty()) {
        QMessageBox::warning(this, "Missing File Name", "Please provide a file name.");
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
        return;
    }
    std::vector<Projet> filteredList;

    for(size_t index=0;index < ListProjet.size();index++){
        if(ListProjet[index].Data.DateDebut <= NumOrdreEnd && ListProjet[index].Data.DateDebut >= NumOrdreDebut){
            filteredList.push_back(ListProjet[index]);
        }
    }

    if(filteredList.size() == 0){
        QMessageBox::warning(this, "Empty File", "No Data for the Selected Period");
        return;
    }

    // === Export TO PDF ===
    QPdfWriter writer(fullFilePath);
    writer.setPageSize(QPagedPaintDevice::A4);
    writer.setResolution(300);

    QPainter painter(&writer);
    if (!painter.isActive()) {
        QMessageBox::critical(this, "Export Error", "Could not open PDF file for writing.");
        return;
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
    QMessageBox::information(this, "Export Successful", "PDF exported successfully!");
    accept();  // Close the ExportDialog
}

