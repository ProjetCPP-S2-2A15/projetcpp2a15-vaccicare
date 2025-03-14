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
        //int Id_Membre_Equipe[5];
    }ProjectData;


    ProjectData Data;
    Projet();
    static std::vector<Projet> ReadProjectListFromDB();
    static int GetLastProjectID();
    static Projet::Statut_Projet ConvertStringToEnum(QString Statut);
    static QString ConvertEnumToString(Projet::Statut_Projet Statut);
    ProjectData LoadProjetData(int ID);
    bool AddProjectToDB();
    bool ModifyProjectDataInDB();
    static bool CloseProject(int ID);
    static bool DeleteProjectFromDb(int ID);
    static int GetIdResponsable(QString Name);
    static QString GetChercheurFromID(int ID);



private:

};

#endif // PROJET_H
