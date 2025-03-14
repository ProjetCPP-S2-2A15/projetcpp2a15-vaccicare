#ifndef MEDECIN_H
#define MEDECIN_H

#include <QMainWindow>
#include <QString>
#include <QSqlQueryModel>
#include <QMessageBox>
#include <QDebug>
#include <QSqlQuery>
#include <QSqlError>
#include <memory>
#include "connection.h"

namespace Ui {
class medecin;
}

class medecin : public QMainWindow
{
    Q_OBJECT

public:
    explicit medecin(QWidget *parent = nullptr);
    explicit medecin(int id_cherch, const QString& nom, const QString& specialite, const QString& grade,
                     const QString& email, int telephone, const QString& disponibilite,
                     int experience, const QString& statut, const QString& login,
                     const QString& motDePasse, QWidget *parent = nullptr);
    ~medecin();

    std::unique_ptr<QSqlQueryModel> afficher(); // Use smart pointer for memory management

private slots:
    void on_ajouter_clicked();
    void on_afficher_clicked();
    void on_supprimer_clicked();
    void on_modifier_clicked();

private:
    Connection Cnx;
    Ui::medecin *ui;
    int id_cherch;  // ID of the doctor
    QString nom;
    QString specialite;
    QString grade;
    QString email;
    int telephone;
    QString disponibilite;
    int experience;
    QString statut;
    QString login;
    QString motDePasse;

    bool ajouter();       // Add a doctor to the database
    bool supprimer(int id); // Delete a doctor from the database
    bool modifier(int id);  // Update a doctor's information in the database
};

#endif // MEDECIN_H
