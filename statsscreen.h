#ifndef STATSSCREEN_H
#define STATSSCREEN_H

#include <QDialog>
#include <QtCharts/QChartView>
#include <QtCharts/QLineSeries>
#include <QtCharts/QChart>

QT_CHARTS_USE_NAMESPACE

namespace Ui {
class StatsScreen;
}

class StatsScreen : public QDialog
{
    Q_OBJECT

public:
    explicit StatsScreen(QWidget *parent = nullptr);
    ~StatsScreen();

private:
    Ui::StatsScreen *ui;
    QChartView *chartView;
};

#endif // STATSSCREEN_H
