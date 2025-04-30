#ifndef RESSOURCE_H
#define RESSOURCE_H

#include "qsqlquerymodel.h"
#include <QString>
#include <QSqlQuery>
#include <QSqlQueryModel>

class Ressource
{
public:
    Ressource();
    Ressource(int, QString, int, int, int, int, int, int);
    bool fetchRessourceById(int id, QString &nom, QString &type, QString &fournisseur, int &quantite, int &cout_acquisition, int &disponibilite, QString &statut);


    // Getters
    int getId();
    QString getNom();
    int getIdTypeRessource();
    int getQuantite();
    int getDisponibilite();
    int getDateAcquisition();
    int getIdFournisseur();
    int getCoutAcquisition();

    // Setters
    void setId(int);
    void setNom(QString);
    void setIdTypeRessource(int);
    void setQuantite(int);
    void setDisponibilite(int);
    void setDateAcquisition(int);
    void setIdFournisseur(int);
    void setCoutAcquisition(int);


    // CRUD operations
    bool ajouter();
    QSqlQueryModel* afficher();
    bool supprimer(int);
    bool modifier(int id, QString nom, int type, int fournisseur, int quantite, int cout_acquisition, int disponibilite, int date_acquisition);
    QSqlQueryModel* rechercherParNom(const QString& nom);
    QSqlQueryModel* trierPar(const QString& critere);

private:
    int id_ressource, id_type_ressource, quantite, disponibilite, date_acquisition, id_fournisseur, cout_acquisition;
    QString nom;
};

#endif // RESSOURCE_H
