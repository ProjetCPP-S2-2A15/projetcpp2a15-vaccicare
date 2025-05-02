#ifndef PROJET_H
#define PROJET_H

#include <vector>
#include <iostream>
#include <string>
#include <QSqlQuery>
#include <QString>
#include <QVariant>
#include "date.h"

class Projet
{
public:
    enum Statut_Projet{
      NONE = -1,
      ARRETER =1,
      EN_COURS = 0,
      CLOTURER =2
    };

    typedef struct{
        int ID_Projet;
        QString Nom;
        int DateDebut;
        int DateFin;
        Statut_Projet Status;
        int Budget;
        int Id_Responsable;
        std::vector<int>Team;
    }ProjectData;


    ProjectData Data;
    Projet();
    static std::vector<Projet> ReadProjectListFromDB(bool AFFICHER_CLOTURER,QString NomMedecin,QString NomProjet);
    static std::vector<Projet> ReadProjectListFromDB(int IdMedecin);
    static int GetLastProjectID();
    static Projet::Statut_Projet ConvertStringToEnum(QString Statut);
    static QString ConvertEnumToString(Projet::Statut_Projet Statut);
    ProjectData LoadProjetData(int ID);
    bool AddProjectToDB();
    bool ModifyProjectDataInDB();
    bool AddTeamToDb(std::vector<int> Team,int Id_Projet);
    static bool CloseProject(int ID);
    static bool DeleteProjectFromDb(int ID);
    static int GetIdResponsable(QString Name);
    static QString GetChercheurFromID(int ID);



private:

};

#endif // PROJET_H
