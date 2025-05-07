#ifndef RESSOURCE_H
#define RESSOURCE_H

#include "qdatetime.h"
#include <QString>
#include <QSqlQuery>
#include <QSqlError>
#include <QDebug>
#include <QVariant>
#include <QSqlQueryModel>

QT_BEGIN_NAMESPACE
namespace Ui {
class Resources;
}
QT_END_NAMESPACE

class Resources {
private:

public:
    // Constructor
    Resources(int id, QString n, int type, int q, int dispo, int date, int fournisseur, int cout);
    Resources();

    bool ajouter();
    static bool supprimer(int id);
    bool modifier();
    static QSqlQueryModel* Afficher();
    int id_ressource;
    QString nom;
    int id_type_ressource;
    int quantite;
    int disponibilite;
    int date_acquisition;
    int id_fournisseur;
    int cout_acquisition;
    static QAbstractItemModel* GetDataForPDF();
    static int getLastId();
};

#endif // RESSOURCE_H
