#include "projet.h"

Projet::Projet()
{

}

std::vector<Projet> Projet::ReadProjectListFromDB(bool AFFICHER_CLOTURER,QString NomMedecin,QString NomProjet){
    std::vector<Projet> ProjectList;
    QSqlQuery Querry;
    QString STATUT_VALUES = "('EN COURS','ARRETER')";
    if (AFFICHER_CLOTURER) {
        STATUT_VALUES = "('EN COURS','ARRETER','CLOTURER')";
    }

    QString QuerryDetail = "SELECT P.ID_PROJET, P.NOM, P.DATE_DEBUT, P.RESPONSABLE "
                           "FROM Projet P "
                           "JOIN MEDECIN M ON P.RESPONSABLE = M.ID_CHERCHEUR "
                           "WHERE P.ID_PROJET > 0 "
                           "AND P.STATUT IN " + STATUT_VALUES + " "
                           "AND LOWER(M.NOM) LIKE '%" + NomMedecin.toLower() + "%'"
                           "AND LOWER(P.NOM) LIKE '%" + NomProjet.toLower() + "%'";

    Querry.prepare(QuerryDetail);
    if(!Querry.exec()){
        return ProjectList;
    }
    Projet Temp;
    while(Querry.next()){
        Temp.Data.ID_Projet = Querry.value("ID_PROJET").toInt();
        Temp.Data.Nom = Querry.value("NOM").toString();
        Temp.Data.DateDebut = Querry.value("DATE_DEBUT").toInt();
        Temp.Data.Id_Responsable = Querry.value("RESPONSABLE").toInt();

        ProjectList.push_back(Temp);
    }

    return ProjectList;
}

int Projet::GetLastProjectID(){
    QSqlQuery Querry;
    Querry.prepare("Select MAX(ID_PROJET) FROM PROJET;");
    Querry.exec();

    if (Querry.next()) {  // Move to the first row
        QVariant result = Querry.value(0);
        if (result.isNull()) {
            return 0;  // Return 0 if the table is empty
        }
        return result.toInt();
    }

    return 0;  // If no rows exist, return 0
}

Projet::ProjectData Projet::LoadProjetData(int ID){
    Projet::ProjectData Data;
    QSqlQuery Querry;

    Querry.prepare("Select * from Projet where ID_PROJET = :id;");
    Querry.bindValue(":id",ID);
    Querry.exec();
    Querry.next();

    Data.ID_Projet = Querry.value("ID_PROJET").toInt();
    Data.Nom = Querry.value("NOM").toString();
    Data.DateDebut = Querry.value("DATE_DEBUT").toInt();
    Data.DateFin = Querry.value("DATE_FIN").toInt();
    Data.Id_Responsable = Querry.value("RESPONSABLE").toInt();
    Data.Status = ConvertStringToEnum(Querry.value("STATUT").toString());
    Data.Budget = Querry.value("BUDGET").toInt();

    return Data;
}

Projet::Statut_Projet Projet::ConvertStringToEnum(QString Statut){
    if(Statut == "ARRETER"){
        return Statut_Projet::ARRETER;
    }
    if(Statut == "CLOTURER"){
        return Statut_Projet::CLOTURER;
    }
    if(Statut == "EN COURS"){
        return Statut_Projet::EN_COURS;
    }

    return Statut_Projet::NONE;

}

QString Projet::ConvertEnumToString(Projet::Statut_Projet Statut){
    if(Statut == Statut_Projet::ARRETER){
        return "ARRETER";
    }
    if(Statut == Statut_Projet::CLOTURER){
        return "CLOTURER";
    }
    if(Statut == Statut_Projet::EN_COURS){
        return "EN COURS";
    }

    return "";

}

bool Projet::AddProjectToDB(){
    QSqlQuery Querry;
    Querry.prepare("INSERT INTO PROJET(ID_PROJET,NOM,DATE_DEBUT,DATE_FIN,STATUT,BUDGET,RESPONSABLE) "
                   "VALUES(:id,:nom,:date_debut,:date_fin,:statut,:budget,:responsable);");

    Querry.bindValue(":id",Data.ID_Projet);
    Querry.bindValue(":nom",Data.Nom);
    Querry.bindValue(":date_debut",Data.DateDebut);
    Querry.bindValue(":date_fin",Data.DateFin);
    Querry.bindValue(":statut",Projet::ConvertEnumToString(Data.Status));
    Querry.bindValue(":budget",Data.Budget);
    Querry.bindValue(":responsable",Data.Id_Responsable);

    if(!Querry.exec()){
            return   false;
    }

    if(!AddTeamToDb(Data.Team,Data.ID_Projet)){
        return false;
    }

    return true;
}

bool Projet::ModifyProjectDataInDB(){
    QSqlQuery Querry;
    Querry.prepare("UPDATE PROJET SET NOM= :nom,DATE_DEBUT= :date_debut,DATE_FIN= :date_fin,STATUT= :statut,"
                   "BUDGET= :budget,RESPONSABLE= :responsable WHERE ID_PROJET= :id;");

    Querry.bindValue(":id",Data.ID_Projet);
    Querry.bindValue(":nom",Data.Nom);
    Querry.bindValue(":date_debut",Data.DateDebut);
    Querry.bindValue(":date_fin",Data.DateFin);
    Querry.bindValue(":statut",Projet::ConvertEnumToString(Data.Status));
    Querry.bindValue(":budget",Data.Budget);
    Querry.bindValue(":responsable",Data.Id_Responsable);

    if(!AddTeamToDb(Data.Team,Data.ID_Projet)){
        return false;
    }
    return Querry.exec();
}

bool Projet::AddTeamToDb(std::vector<int> Team,int Id_Projet){
    QSqlQuery DeleteQuerry;
    DeleteQuerry.prepare("DELETE FROM EQUIPE_PROJET WHERE ID_PROJET = :id_projet");
    DeleteQuerry.bindValue(":id_projet",Id_Projet);
    DeleteQuerry.exec();

    // Build the INSERT query dynamically
    if (Team.empty()) return true; // No team to insert is not a failure

    for (int id : Team) {
            QSqlQuery InsertQuery;
            InsertQuery.prepare("INSERT INTO EQUIPE_PROJET (ID_CHERCHEUR, ID_PROJET) VALUES (:id_chercheur, :id_projet)");
            InsertQuery.bindValue(":id_chercheur", id);
            InsertQuery.bindValue(":id_projet", Id_Projet);
            if (!InsertQuery.exec()) {
                return false;
            }
        }
        return true;
}

bool Projet::CloseProject(int ID){
    QSqlQuery Querry;
    Querry.prepare("UPDATE PROJET SET DATE_FIN= :date_fin,STATUT= :statut"
                   " WHERE ID_PROJET= :id;");

    QDate Today = QDate::currentDate();
    int DateFin = Date::ConvertDateToInt(Today.toString("dd/MM/yyyy"));
    Querry.bindValue(":id",ID);
    Querry.bindValue(":date_fin",DateFin);
    Querry.bindValue(":statut",Projet::ConvertEnumToString(Statut_Projet::CLOTURER));

    return Querry.exec();
}

bool Projet::DeleteProjectFromDb(int ID){
    QSqlQuery Querry;
    Querry.prepare("DELETE FROM PROJET WHERE ID_PROJET = :id");

    Querry.bindValue(":id",ID);

    return Querry.exec();
}

int Projet::GetIdResponsable(QString Name) {
    QSqlQuery Querry;
    Querry.prepare("SELECT ID_CHERCHEUR FROM MEDECIN WHERE NOM = :nom;");
    Querry.bindValue(":nom", Name);  // Fixed missing colon in bindValue key
    if (!Querry.exec()) {
        //qDebug() << "Error executing query: " << Querry.lastError().text();
        return -1;  // Return an invalid ID in case of failure
    }
    if (Querry.next()) {
        return Querry.value("ID_CHERCHEUR").toInt();
    }
    return -1;  // Return -1 if no record is found
}

QString Projet::GetChercheurFromID(int ID) {
    QSqlQuery Querry;
    Querry.prepare("SELECT NOM FROM MEDECIN WHERE ID_CHERCHEUR = :id;");
    Querry.bindValue(":id", ID);  // Fixed missing colon in bindValue key
    if (!Querry.exec()) {
        //qDebug() << "Error executing query: " << Querry.lastError().text();
        return QString();  // Return empty string in case of failure
    }
    if (Querry.next()) {
        return Querry.value("NOM").toString();
    }
    return QString();  // Return empty string if no record is found
}
