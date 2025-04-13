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
}

ExportDialog::~ExportDialog()
{
    delete ui;
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

    int y = 100;  // Start y-position
    int xMargin = 50;
    int lineHeight = 70;

    QFont font("Times", 10);
    QFont boldFont("Times", 10, QFont::Bold);
    painter.setFont(boldFont);

    // Header row
    painter.drawText(xMargin, y, "ID_PROJET");
    painter.drawText(xMargin + X_MARGIN_NOM, y, "NOM");
    painter.drawText(xMargin + X_MARGIN_DATE_DEBUT, y, "DATE_DEBUT");
    painter.drawText(xMargin + X_MARGIN_ID_RESPONSABLE, y, "RESPONSABLE");
    painter.drawText(xMargin + X_MARGIN_BUDGET, y, "BUDGET");
    painter.drawText(xMargin + X_MARGIN_DATE_FIN, y, "DATE_FIN");

    y += lineHeight;
    painter.setFont(font);

    for (const auto& proj : filteredList) {
        const auto& data = proj.Data;

        QString dateDebutStr = Date::ConvertIntToDate(data.DateDebut).toString("dd/MM/yyyy");
        QString dateFinStr = (data.DateFin == -1)
                ? "Not set"
                : Date::ConvertIntToDate(data.DateFin).toString("dd/MM/yyyy");

        painter.drawText(xMargin, y, QString::number(data.ID_Projet));
        painter.drawText(xMargin + X_MARGIN_NOM, y, data.Nom);
        painter.drawText(xMargin + X_MARGIN_DATE_DEBUT, y, dateDebutStr);
        painter.drawText(xMargin + X_MARGIN_ID_RESPONSABLE, y, QString::number(data.Id_Responsable));
        painter.drawText(xMargin + X_MARGIN_BUDGET, y, QString::number(data.Budget));
        painter.drawText(xMargin + X_MARGIN_DATE_FIN, y, dateFinStr);

        y += lineHeight;

        if (y > writer.height() - 100) {
            writer.newPage();
            y = 100;
        }
    }

    painter.end();

    QMessageBox::information(this, "Export Successful", "PDF exported successfully!");
    accept();  // Close the ExportDialog
}

