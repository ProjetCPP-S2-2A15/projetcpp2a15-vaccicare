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

QVector<QPair<int, double>> StatsScreen::GetChartDate(int Date_Debut,int Date_Fin,int Id_Projet){
    QVector<QPair<int, double>> DataPoints;

    // Build the SQL query.
    QSqlQuery query;
    query.prepare("SELECT CEIL(DATE_VACCIN / 30) AS MonthIndex, "
                  "       COUNT(*) * V.PRIX_UNITAIRE AS TotalRevenue "
                  "FROM PATIENT "
                  "JOIN VACCIN V ON PATIENT.ID_PROJET_TESTER = V.ID_PROJET_SOURCE "
                  "WHERE ID_PROJET_TESTER = :Id_Projet "
                  "  AND DATE_VACCIN >= :Date_Min "
                  "GROUP BY CEIL(DATE_VACCIN / 30) ,V.PRIX_UNITAIRE  "
                  "ORDER BY MonthIndex");

    query.bindValue(":Date_Min", Date_Debut);
    //query.bindValue(":Date_Fin", Date_Fin);
    query.bindValue(":Id_Projet", Id_Projet);
    query.exec();
    while (query.next()) {
        int month = query.value("MonthIndex").toInt();
        double count = query.value("TotalRevenue").toDouble();
        DataPoints.append(QPair<int, double>(month, count));
    }

    for(size_t index = 0; index < DataPoints.size(); index++){
        DataPoints[index].first = DataPoints[0].first + 1;
    }

    return DataPoints;
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
            int Today = Date::GetTodayDate();
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
        int Today = Date::GetTodayDate();
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

    QVector<QPair<int, double>> monthlyRevenueData = GetChartDate(Date_Debut,Date_Fin,ID_PROJET);

    if (monthlyRevenueData.isEmpty()) {
        QMessageBox::warning(this, "No Data", "No revenue data available for this project.");
        for(int index = 1; index<6 ;index ++){
            monthlyRevenueData.append(QPair<int, double>(index, 0));
        }
    }

    // Normalize data: ensure at least 5 months are represented
    QMap<int, double> normalizedData;
    for (const auto &pair : monthlyRevenueData) {
        normalizedData[pair.first] = pair.second;
    }

    int minMonth = monthlyRevenueData.first().first;
    int maxMonth = monthlyRevenueData.last().first;

    if (monthlyRevenueData.size() < 5) {
        maxMonth = minMonth + 4; // Ensure we have 5 months
    }

    // Fill in missing months with 0
    QVector<QPair<int, double>> finalData;
    for (int month = minMonth; month <= maxMonth; ++month) {
        double revenue = normalizedData.value(month, 0.0); // 0.0 if not found
        finalData.append({month, revenue});
    }

    // Create a new line series
    QtCharts::QLineSeries *series = new QtCharts::QLineSeries();
    for (const auto &pair : monthlyRevenueData) {
        int month = pair.first;
        double revenue = pair.second;
        series->append(month, revenue);
    }
    series->setName("Revenue per Month");

    // Create a new chart and add the series
    QtCharts::QChart *chart = new QtCharts::QChart();
    chart->addSeries(series);
    chart->setTitle("Monthly Revenue");
    chart->legend()->setVisible(true);
    chart->legend()->setAlignment(Qt::AlignBottom);

    // Create a category axis for the x-axis representing month numbers or labels
    // Alternatively, if you want to use numeric values, you could use QValueAxis.
    // Here we create a category axis so we can label the months.
    QtCharts::QCategoryAxis *axisX = new QtCharts::QCategoryAxis();
    axisX->setTitleText("Month");
    // Assume month numbers (1, 2, ...) – you can customize labels if needed.
    for (const auto &pair : finalData) {
        int month = pair.first;
        // Each tick is placed at the corresponding month value.
        axisX->append(QString("Month %1").arg(month), month);
    }
    // Set the range from first to last month based on your data:
    axisX->setRange(finalData.first().first, finalData.last().first);

    // Create a value axis for the y-axis representing revenue
    QtCharts::QValueAxis *axisY = new QtCharts::QValueAxis();
    axisY->setTitleText("Revenue");
    // You can set range if needed. For example, from 0 up to a maximum revenue, or use autoscaling.
    axisY->setLabelFormat("%i");

    // Attach the axes to the chart and the series
    chart->addAxis(axisX, Qt::AlignBottom);
    chart->addAxis(axisY, Qt::AlignLeft);
    series->attachAxis(axisX);
    series->attachAxis(axisY);

    // Create a chart view with antialiasing for smoother visuals
    QtCharts::QChartView *chartView = new QtCharts::QChartView(chart);
    chartView->setRenderHint(QPainter::Antialiasing);

    QLayoutItem *child;
    while ((child = ui->verticalLayout->takeAt(0)) != nullptr) {
        if(child->widget()){
            // Remove the widget from its parent so that deletion is clean.
            child->widget()->setParent(nullptr);
            child->widget()->deleteLater();  // schedule deletion of the widget
        }
        delete child;
    }
    ui->verticalLayout->addWidget(chartView);
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
    // Create a new chart and set its title
    QtCharts::QChart *chart = new QtCharts::QChart();
    chart->setTitle("Monthly Revenue");
    chart->setAnimationOptions(QtCharts::QChart::NoAnimation);

    // Create an empty line series (for revenue per month)
    QtCharts::QLineSeries *series = new QtCharts::QLineSeries();
    // Data points can be added later as: series->append(month, revenue);
    chart->addSeries(series);

    // Set up the X-axis for time (months)
    // We'll use a category axis so you can label each month.
    QtCharts::QCategoryAxis *axisX = new QtCharts::QCategoryAxis();
    axisX->setTitleText("Month");
    // For the empty chart, we add a few placeholder categories.
    // You can update these with real month numbers or names later.
    QStringList categories;
    categories << "Month 1" << "Month 2" << "Month 3" << "Month 4" << "Month 5" << "Month 6";
    int pos = 1;
    for (const QString &cat : categories) {
        axisX->append(cat, pos); // associates the label with the month number
        pos++;
    }
    axisX->setRange(1, categories.size());

    // Set up the Y-axis for revenue
    QtCharts::QValueAxis *axisY = new QtCharts::QValueAxis();
    axisY->setTitleText("Revenue");
    axisY->setLabelFormat("%.2f"); // format numbers with 2 decimal places

    // Add the axes to the chart and attach them to the series
    chart->addAxis(axisX, Qt::AlignBottom);
    chart->addAxis(axisY, Qt::AlignLeft);
    series->attachAxis(axisX);
    series->attachAxis(axisY);

    // Create a chart view to display the chart with antialiasing for smooth rendering
    QtCharts::QChartView *chartView = new QtCharts::QChartView(chart);
    chartView->setRenderHint(QPainter::Antialiasing);

    // Clear any existing widgets in the vertical layout and add this chart view
    QLayoutItem *child;
    while ((child = ui->verticalLayout->takeAt(0)) != nullptr) {
        if (child->widget())
            delete child->widget();
        delete child;
    }
    ui->verticalLayout->addWidget(chartView);
}
