#ifndef STATVACCIN_H
#define STATVACCIN_H

#include <QDialog>
#include <QtCharts/QChartView>
#include <QtCharts/QPieSeries>
#include <QtCharts/QChart>
#include <QSqlQuery>
#include <QMessageBox>
#include <QVBoxLayout>
#include <QPainter>

using namespace QtCharts;

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
