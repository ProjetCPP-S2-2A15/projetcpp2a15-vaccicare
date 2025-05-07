#include "dialogstatmedecin.h"


DialogStatMedecin::DialogStatMedecin(QWidget *parent)
    : QDialog(parent)
{
    setWindowTitle("Statistiques - Répartition des Médecins par Statut");
    resize(600, 400);
    afficherStatParStatut();
}

DialogStatMedecin::~DialogStatMedecin() {}

void DialogStatMedecin::afficherStatParStatut()
{
    QSqlQuery query;
    QPieSeries *series = new QPieSeries();

    // Requête pour compter les médecins par statut
    if (query.exec("SELECT STATUT, COUNT(*) FROM MEDECIN GROUP BY STATUT"))
    {
        while (query.next()) {
            QString statut = query.value(0).toString();
            int count = query.value(1).toInt();
            series->append(statut + " (" + QString::number(count) + ")", count);
        }
    }
    else {
        QMessageBox::critical(this, "Erreur", "Impossible de récupérer les données.");
        return;
    }

    QChart *chart = new QChart();
    chart->addSeries(series);
    chart->setTitle("Répartition des Médecins par Statut");
    chart->legend()->setVisible(true);
    chart->legend()->setAlignment(Qt::AlignRight);

    QChartView *chartView = new QChartView(chart);
    chartView->setRenderHint(QPainter::Antialiasing);

    QVBoxLayout *layout = new QVBoxLayout(this);
    layout->addWidget(chartView);
    setLayout(layout);
}
