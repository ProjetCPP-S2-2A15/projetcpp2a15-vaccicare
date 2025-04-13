#ifndef STATVACCIN_H
#define STATVACCIN_H

#include <QDialog>
#include <QtCharts/QChartView>
#include <QtCharts/QPieSeries>
#include <QtCharts/QChart>

class StatVaccin : public QDialog
{
    Q_OBJECT

public:
    explicit StatVaccin(QWidget *parent = nullptr);
    ~StatVaccin();

private:
    void afficherStatParType();
};

#endif // STATVACCIN_H
