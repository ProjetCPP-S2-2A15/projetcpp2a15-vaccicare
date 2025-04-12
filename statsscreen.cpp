#include "statsscreen.h"
#include "ui_statsscreen.h"


StatsScreen::StatsScreen(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::StatsScreen)
{
    ui->setupUi(this);
    connect(ui->ButtonCalculerStatistic,&QPushButton::clicked,this,&StatsScreen::CalculateStatistics);
    SetupUI();
}

StatsScreen::~StatsScreen()
{
    delete ui;
}

void StatsScreen::CalculateStatistics(){

    QSqlQuery Querry;
    Querry.prepare("SELECT ID_PROJET,DATE_DEBUT,DATE_FIN FROM PROJET WHERE NOM = :Nom");
    Querry.bindValue(":Nom",ui->comboBoxListeProjet->currentText());
    Querry.exec();
    Querry.next();
    int ID_PROJET = Querry.value("ID_PROJET").toInt();
    int Date_Debut = Querry.value("DATE_DEBUT").toInt();
    int Date_Fin = Querry.value("DATE_FIN").toInt();

    QSqlQuery SimpleStatisctics;
    SimpleStatisctics.prepare("SELECT "
                              "PR.NOM AS NOM_PROJET, "
                              "PR.DATE_DEBUT, "
                              "PR.DATE_FIN, "
                              "PR.BUDGET, "
                              "V.NOM AS NOM_VACCIN, "
                              "V.PRIX_UNITAIRE * COUNT(*) AS TOTAL_REVENUE, "
                              "COUNT(*) AS NMBRE_PATIENT "
                            "FROM PROJET PR "
                            "JOIN VACCIN V ON PR.ID_PROJET = V.ID_PROJET_SOURCE "
                            "JOIN PATIENT PA ON PR.ID_PROJET = PA.ID_PROJET_TESTER "
                            "WHERE PR.ID_PROJET = :Id_Projet "
                            "GROUP BY PR.NOM, PR.DATE_DEBUT, PR.DATE_FIN,PR.BUDGET,V.NOM,V.PRIX_UNITAIRE;");
    SimpleStatisctics.bindValue(":Id_Projet",ID_PROJET);
    SimpleStatisctics.exec();

    int Budget;
    int TotalRevenue;
    int NbrPatient;

    if (!SimpleStatisctics.next()) {
        QMessageBox::warning(this, "Error", "No Patient Has been Vaccinated yet ", QMessageBox::Ok);
        Budget = 0;
        TotalRevenue = 0;
        NbrPatient = 0;
    }else{
        Budget = SimpleStatisctics.value("BUDGET").toInt();
        TotalRevenue = SimpleStatisctics.value("TOTAL_REVENUE").toInt();
        NbrPatient = SimpleStatisctics.value("NMBRE_PATIENT").toInt();
    }

    QSqlQuery DoctorQuerry;
    DoctorQuerry.prepare("SELECT "
                         "EP.ID_CHERCHEUR, "
                         "M.NOM, "
                         "M.CHARGE_MENSUELLE AS WAGE "
                       "FROM EQUIPE_PROJET EP "
                       "JOIN MEDECIN M ON EP.ID_CHERCHEUR = M.ID_CHERCHEUR "
                       "WHERE ID_PROJET = :Id_Projet;");
    DoctorQuerry.bindValue(":Id_Projet",ID_PROJET);
    DoctorQuerry.exec();

    ui->tableWidget->setRowCount(0);
    ui->tableWidget->clearContents();

    double TotalCost = 0;
    int row = 0;
    while(DoctorQuerry.next()){
        QString Nom = DoctorQuerry.value("NOM").toString();
        int Wage = DoctorQuerry.value("WAGE").toInt();

        double WagePerDay = Wage / 30.0;
        int NumDaysWorked;
        if(Date_Fin == -1){
            int Today = Date::ConvertDateToInt(QDate::currentDate().toString("dd/MM/yyyy"));
            NumDaysWorked = Today - Date_Debut;
        }else{
            NumDaysWorked = Date_Fin - Date_Debut;
        }
        double Total = WagePerDay * NumDaysWorked;
        TotalCost += Total;
        ui->tableWidget->insertRow(row);

        ui->tableWidget->setItem(row, 0, new QTableWidgetItem(Nom));
        ui->tableWidget->setItem(row, 1, new QTableWidgetItem(QString::number(Wage)));
        ui->tableWidget->setItem(row, 2, new QTableWidgetItem(QString::number(Total)));
        row++;
    }

    TotalCost += Budget;

    double MonthlyRevenu;
    int TotalDays;
    if(Date_Fin == -1){
        int Today = Date::ConvertDateToInt(QDate::currentDate().toString("dd/MM/yyyy"));
        TotalDays = Today - Date_Debut;
    }else{
        TotalDays = Date_Fin - Date_Debut;
    }
    double NumMonths = TotalDays / 30.0;
    if(NumMonths <= 0){
        MonthlyRevenu = TotalRevenue;
    } else {
        MonthlyRevenu = TotalRevenue / NumMonths;
    }

    ui->LineEditBudget->setText(QString::number(Budget));
    ui->LineEditCoutTotal->setText(QString::number(TotalCost));
    ui->LineEditNmbrePatient->setText(QString::number(NbrPatient));
    ui->LineEditRetourMensuelle->setText(QString::number(MonthlyRevenu));
    ui->LineEditRetourTotal->setText(QString::number(TotalRevenue));

    //Set Up Charts
}

void StatsScreen::SetupUI(){

    //Get list of all the Projects
    QSqlQuery Querry;
    Querry.prepare("SELECT NOM FROM PROJET WHERE ID_PROJET >0");
    Querry.exec();
    while(Querry.next()){
        ui->comboBoxListeProjet->addItem(Querry.value(0).toString());
    }

    ui->LineEditBudget->setReadOnly(true);
    ui->LineEditCoutTotal->setReadOnly(true);
    ui->LineEditNmbrePatient->setReadOnly(true);
    ui->LineEditRetourMensuelle->setReadOnly(true);
    ui->LineEditRetourTotal->setReadOnly(true);

    //Fill UI with Null(0) Values
    ui->LineEditBudget->setText("0");
    ui->LineEditCoutTotal->setText("0");
    ui->LineEditNmbrePatient->setText("0");
    ui->LineEditRetourMensuelle->setText("0");
    ui->LineEditRetourTotal->setText("0");

    //setup Doctor Table
    ui->tableWidget->setColumnCount(3);
    // Set Column Headers
    QStringList headers;
    headers << "Doctor Name" << "Wage" << "Total";
    ui->tableWidget->setHorizontalHeaderLabels(headers);
    // Set the column widths
    ui->tableWidget->setColumnWidth(0, 100);
    ui->tableWidget->setColumnWidth(1, 90);
    ui->tableWidget->setColumnWidth(2, 90);

    ui->tableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);  // Make it non-editable

    // Set the table size (for example, you can resize it to fill the available space)
    ui->tableWidget->resize(280, 390);

    ////////////
    // Set up an empty chart to be added into your vertical layout (assumed to be named "chartLayout")
        // Create a new chart
        QtCharts::QChart *chart = new QtCharts::QChart();
        chart->setTitle("Statistics Overview");
        // Optionally, you can disable animation if you want a static chart
        chart->setAnimationOptions(QtCharts::QChart::NoAnimation);

        // Create an empty bar series (or another type of series if needed)
        QtCharts::QBarSeries *series = new QtCharts::QBarSeries();
        // No data is added now; this is an empty series.
        chart->addSeries(series);

        // Setup a category axis with dummy categories (replace later with real data if needed)
        QStringList categories;
        categories << "Category 1" << "Category 2"; // Placeholder, update as needed.
        QtCharts::QBarCategoryAxis *axisX = new QtCharts::QBarCategoryAxis();
        axisX->append(categories);
        chart->addAxis(axisX, Qt::AlignBottom);
        series->attachAxis(axisX);

        // Optionally, add a default value axis (Y-axis) if needed
        // Example: chart->createDefaultAxes();

        // Create a chart view with antialiasing for smoother visuals
        QtCharts::QChartView *chartView = new QtCharts::QChartView(chart);
        chartView->setRenderHint(QPainter::Antialiasing);

        // Add the chart view to your vertical layout
        ui->verticalLayout->addWidget(chartView);
}
