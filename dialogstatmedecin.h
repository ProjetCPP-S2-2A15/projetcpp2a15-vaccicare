#ifndef DIALOGSTATMEDECIN_H
#define DIALOGSTATMEDECIN_H

#include <QDialog>
#include <QSqlQuery>
#include <QMessageBox>
#include <QVBoxLayout>
#include <QPainter>
#include <QtCharts/QChart>
#include <QtCharts/QChartView>
#include <QtCharts/QPieSeries>

using namespace QtCharts;

class DialogStatMedecin : public QDialog
{
    Q_OBJECT

public:
    explicit DialogStatMedecin(QWidget *parent = nullptr);
    ~DialogStatMedecin();

private:
    void afficherStatParStatut(); // Méthode pour charger et afficher le graphique
};

#endif // DIALOGSTATMEDECIN_H
