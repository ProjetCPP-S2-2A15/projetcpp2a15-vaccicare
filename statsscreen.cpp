#include "statsscreen.h"
#include "ui_statsscreen.h"
#include <QtCharts/QChartView>
#include <QtCharts/QLineSeries>
#include <QtCharts/QChart>

StatsScreen::StatsScreen(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::StatsScreen)
{
    ui->setupUi(this);

    // Create a simple line chart
       QLineSeries *series = new QLineSeries();
       series->append(0, 6);
       series->append(1, 4);
       series->append(2, 7);
       series->append(3, 3);
       series->append(4, 8);

       QChart *chart = new QChart();
       chart->addSeries(series);
       chart->createDefaultAxes();
       chart->setTitle("Simple Line Chart");

       // Create a QChartView to display the chart
       chartView = new QChartView(chart);
       chartView->setRenderHint(QPainter::Antialiasing);

       // Set the QChartView as the central widget
       ui->verticalLayout->addWidget(chartView);
}

StatsScreen::~StatsScreen()
{
    delete ui;
}
