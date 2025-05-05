#ifndef EXPERIENCE_H
#define EXPERIENCE_H

#include <QString>
#include <vector>
#include <QSqlQuery>
#include <QVariant>

class Experience
{
public:
    Experience();
    Experience(QString Description,int ID_Experience, int ID_Projet, QString Nom);
    QString Description;
    int ID_Experience;
    int ID_Projet;
    QString Nom;

    static std::vector<Experience> LoadExperienceFromDb();
    static std::vector<Experience> LoadExperienceFromDb(int ID_Projet);
    static Experience LoadExperienceDescriptionFromDb(int ID_Experince);
    bool UpladteExperience();
    bool DeleteExperience();
    bool AddExperienceToDb(int ID_Projet);
    static int GetLastID();
};

#endif // EXPERIENCE_H
