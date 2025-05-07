#ifndef STATSSCREEN_H
#define STATSSCREEN_H

#include <QDialog>
#include "connection.h"
#include "projet.h"
#include "Design.h"
#include <QMessageBox>
#include <QtCharts/QChart>
#include <QtCharts/QChartView>
#include <QtCharts/QBarSeries>
#include <QtCharts/QBarSet>
#include <QtCharts/QBarCategoryAxis>
#include <QPainter>
#include <QtCharts/QLineSeries>
#include <QtCharts/QCategoryAxis>
#include <QtCharts/QValueAxis>
#include <QSqlQuery>
#include <QSqlError>
#include <QDebug>

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
    void SetupUI();
    void CalculateStatistics();
    QVector<QPair<int, double>> GetChartDate(int Date_Debut,int Date_Fin,int Id_Projet);
    void SetupDesign();
    Ui::StatsScreen *ui;
    double RetourTotal;
    double RetourMensuelle;
    int NbrPatient;
    int Budget;
    int CoutTotal;
    QChartView *chartView;
};

#endif // STATSSCREEN_H
