#ifndef RESSOURCE_H
#define RESSOURCE_H

#include <QString>
#include <QSqlQuery>
#include <QSqlError>
#include <QDebug>
#include <QVariant>

QT_BEGIN_NAMESPACE
namespace Ui {
class Resources;
}
QT_END_NAMESPACE

class Resources {
private:
    int id_ressource;
    QString nom;
    int id_type_ressource;
    int quantite;
    int disponibilite;
    int date_acquisition;
    int id_fournisseur;
    int cout_acquisition;

public:
    // Constructor
    Resources(int id, QString n, int type, int q, int dispo, int date, int fournisseur, int cout);

    bool ajouter();
    static bool supprimer(int id);
    bool modifier();
    static void afficher();
};

#endif // RESSOURCE_H
