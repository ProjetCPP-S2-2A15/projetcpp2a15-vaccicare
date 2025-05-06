#include "dialoglistepatient.h"
#include "dialogformpatient.h"
#include "patient.h"
#include "ui_dialoglistepatient.h"
#include <QMessageBox>
#include <QSqlError>
#include <QRegularExpressionValidator>
#include <QSqlQuery>
#include <QTextStream>
#include <QFileDialog>
#include <QTextDocument>
#include <QSystemTrayIcon>
#include <QMessageBox>
#include <QRegularExpression>
#include <QtPrintSupport/QPrinter>
#include <QtPrintSupport/QPrintDialog>
#include <QTextDocument>
#include <QFileDialog>
#include <QFileInfo>
#include <QTextStream>
#include <QtCharts>
#include <QtCharts/QChartView>
#include <QtCharts/QChart>
#include <QtCharts/QPieSeries>
#include <QPainter>
#include <QSqlQuery>
#include <QSqlError>
#include <QtCharts/QPieSlice>
//#include <QSsl>
//#include <QSslSocket> // Add this for SSL support
#include <QMessageBox>
#include "Design.h"
#include <QScopedPointer>
//#include "smtp.h"

Dialoglistepatient::Dialoglistepatient(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::Dialoglistepatient)
{
    ui->setupUi(this);
    connect(ui->ButtonAjouter,&QPushButton::clicked,this,&Dialoglistepatient::on_buttonAjouter_clicked);
    connect(ui->ButtonModifier,&QPushButton::clicked,this,&Dialoglistepatient::on_buttonModifier_clicked);
    connect(ui->ButtonModifier,&QPushButton::clicked,this,&Dialoglistepatient::on_buttonSupprimer_clicked);
    //connect(ui->ButtonPDF,&QPushButton::clicked,this,&Dialoglistepatient::on_buttonPDF_clicked);
    connect(ui->ButtonRetourner,&QPushButton::clicked,this,&Dialoglistepatient::ExitApp);

    setupDesign();
}

Dialoglistepatient::~Dialoglistepatient()
{
    delete ui;
}

void Dialoglistepatient::setupDesign() {

    StyleButtonGreen(ui->ButtonAjouter);
    StyleButtonRed(ui->ButtonSupprimer);
    StyleButtonBlue(ui->ButtonModifier);
    StyleButtonDarkred(ui->ButtonRetourner);

    StyleTextEdit(ui->recherche);
    StyleComboBox(ui->recherche_par);
    StyleComboBox(ui->tri);

    StyleTableView(ui->tableView);

}


//void Dialoglistepatient::on_Pat_Button_ExportPDF_clicked()
//{
//    QString strStream;
//    QTextStream out(&strStream);

//    const int rowCount = ui->tableView->model()->rowCount();
//    const int columnCount = ui->tableView->model()->columnCount();

//    out << "<html>\n"
//           "<head>\n"
//           "<meta Content=\"Text/html; charset=Windows-1251\">\n"
//        << QString("<title>%1</title>\n").arg("Export PDF")
//        << "</head>\n"
//           "<body bgcolor=#ffffff link=#5000A0>\n"
//           "<center><h1>Liste des Recherches</h1><br><br>\n"
//           "<table border=1 cellspacing=0 cellpadding=2>\n";

//    // Table headers
//    out << "<thead><tr bgcolor=#f0f0f0><th>Numéro</th>";
//    for (int column = 0; column < columnCount; ++column) {
//        if (!ui->tableView->isColumnHidden(column)) {
//            out << QString("<th>%1</th>").arg(
//                ui->tableView->model()->headerData(column, Qt::Horizontal).toString());
//        }
//    }
//    out << "</tr></thead>\n";

//    // Table data
//    for (int row = 0; row < rowCount; ++row) {
//        out << "<tr><td>" << row + 1 << "</td>";
//        for (int column = 0; column < columnCount; ++column) {
//            if (!ui->tableView->isColumnHidden(column)) {
//                QString data = ui->tableView->model()
//                ->data(ui->tableView->model()->index(row, column))
//                    .toString()
//                    .simplified();
//                out << QString("<td>%1</td>").arg(data.isEmpty() ? "&nbsp;" : data);
//            }
//        }
//        out << "</tr>\n";
//    }

//    out << "</table></center>\n</body>\n</html>\n";

//    QString fileName = QFileDialog::getSaveFileName(this, "Sauvegarder en PDF", "", "*.pdf");
//    if (QFontInfo(fileName).suffix().isEmpty())
//        fileName.append(".pdf");

//    QPainter printer((QPainter::HighResolution));
//    printer.setOutputFormat(QPainter::PdfFormat);
//    printer.setPageSize(QPageSize(QPageSize::A4));
//    printer.setOutputFileName(fileName);

//    QTextDocument doc;
//    doc.setHtml(strStream);
//    doc.print(&printer);
//}


void Dialoglistepatient::on_buttonAjouter_clicked()
{
    Dialogformpatient *dialog = new Dialogformpatient(this);
    dialog->exec();
    Patient result = dialog->GetResult();
    if(result.getID_PATIENT() != -1){
        result.Ajouter(); // Ajoute directement à la base
    }
}

void Dialoglistepatient::on_buttonModifier_clicked()
{
    Dialogformpatient *dialog = new Dialogformpatient(this);
    dialog->exec();
    Patient result = dialog->GetResult();
    if(result.getID_PATIENT() != -1){
        result.Modifier(); // Met à jour le patient dans la base
    }
}

void Dialoglistepatient::on_buttonSupprimer_clicked()
{
    Dialogformpatient *dialog = new Dialogformpatient(this);
    dialog->exec();
    delete dialog;
}

void Dialoglistepatient::ExitApp(){
    close();
}
