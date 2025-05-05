#ifndef DIALOGSTATMEDECIN_H
#define DIALOGSTATMEDECIN_H

#include <QDialog>

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
