#include "StatTvaccin.h"

StatVaccin::StatVaccin(QWidget *parent)
    : QDialog(parent)
{
    setWindowTitle("Statistiques - Types de Vaccins");
    resize(600, 400);
    afficherStatParType();
}

StatVaccin::~StatVaccin() {}

void StatVaccin::afficherStatParType()
{
    QSqlQuery query;
    QPieSeries *series = new QPieSeries();

    if (query.exec("SELECT t.TYPE_VACCIN, COUNT(v.ID_VACCIN) "
                   "FROM TYPE_VACCIN t "
                   "LEFT JOIN VACCIN v ON v.ID_TYPE_VACCIN = t.ID_TYPE_VACCIN "
                   "GROUP BY t.TYPE_VACCIN"))
    {
        while (query.next()) {
            QString type = query.value(0).toString();
            int count = query.value(1).toInt();
            series->append(type + " (" + QString::number(count) + ")", count);
        }
    }
    else {
        QMessageBox::critical(this, "Erreur", "Impossible de récupérer les données.");
        return;
    }

    QChart *chart = new QChart();
    chart->addSeries(series);
    chart->setTitle("Répartition des Vaccins par Type");
    chart->legend()->setVisible(true);
    chart->legend()->setAlignment(Qt::AlignRight);

    QChartView *chartView = new QChartView(chart);
    chartView->setRenderHint(QPainter::Antialiasing);

    QVBoxLayout *layout = new QVBoxLayout(this);
    layout->addWidget(chartView);
    setLayout(layout);
}
