#include "experience.h"

Experience::Experience()
{

}

Experience::Experience(QString Description, int ID_Experience, int ID_Projet)
    : Description(Description), ID_Experience(ID_Experience), ID_Projet(ID_Projet) {}


std::vector<Experience> Experience::LoadExperienceFromDb() {
    std::vector<Experience> result;
    QSqlQuery query("SELECT * FROM EXPERIENCE");

    while (query.next()) {
        result.emplace_back(
            query.value("DESCRIPTION").toString(),
            query.value("ID_EXPERIENCE").toInt(),
            query.value("ID_PROJET").toInt()
        );
    }
    return result;
}

std::vector<Experience> Experience::LoadExperienceFromDb(int ID_Projet) {
    std::vector<Experience> result;
    QSqlQuery query;
    query.prepare("SELECT * FROM EXPERIENCE WHERE ID_PROJET = :id");
    query.bindValue(":id", ID_Projet);
    query.exec();

    while (query.next()) {
        result.emplace_back(
            query.value("DESCRIPTION").toString(),
            query.value("ID_EXPERIENCE").toInt(),
            query.value("ID_PROJET").toInt()
        );
    }
    return result;
}

bool Experience::AddExperienceToDb(int ID_Projet) {
    QSqlQuery query;
    query.prepare("INSERT INTO EXPERIENCE (ID_EXPERIENCE, ID_PROJET, DESCRIPTION) "
                  "VALUES (:id_experience, :id_projet, :description)");
    query.bindValue(":id_experience", ID_Experience);
    query.bindValue(":id_projet", ID_Projet);
    query.bindValue(":description", Description);
    return query.exec();
}

Experience Experience::LoadExperienceDescriptionFromDb(int ID_Experience) {
    QSqlQuery query;
    query.prepare("SELECT * FROM EXPERIENCE WHERE ID_EXPERIENCE = :id");
    query.bindValue(":id", ID_Experience);
    query.exec();
    query.next();

    return Experience(
        query.value("DESCRIPTION").toString(),
        query.value("ID_EXPERIENCE").toInt(),
        query.value("ID_PROJET").toInt()
    );
}

bool Experience::UpladteExperience() {
    QSqlQuery query;
    query.prepare("UPDATE EXPERIENCE SET DESCRIPTION = :desc WHERE ID_EXPERIENCE = :id");
    query.bindValue(":desc", Description);
    query.bindValue(":id", ID_Experience);
    return query.exec();
}

bool Experience::DeleteExperience() {
    QSqlQuery query;
    query.prepare("DELETE FROM EXPERIENCE WHERE ID_EXPERIENCE = :id");
    query.bindValue(":id", ID_Experience);
    return query.exec();
}

int Experience::GetLastID() {
    QSqlQuery query;
    query.prepare("SELECT MAX(ID_EXPERIENCE) FROM EXPERIENCE");

    if (query.exec() && query.next()) {
        return query.value(0).toInt();
    }

    return 0;
}

