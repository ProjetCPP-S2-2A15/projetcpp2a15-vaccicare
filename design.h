#ifndef DESIGN_H
#define DESIGN_H

#include <QPushButton>
#include <QLineEdit>
#include <QLabel>
#include <QTableView>
#include <QTableWidget>
#include <QCheckBox>
#include <QComboBox>
#include <QDateEdit>

// Style functions declarations
void StyleButtonAjouter(QPushButton* button);
void StyleButtonModifier(QPushButton* button);
void StyleButtonSupprimer(QPushButton* button);
void StyleButtonPDF(QPushButton* button);
void StyleButtonOther(QPushButton* button);


void StyleLineEdit(QLineEdit* lineEdit);
void StyleLabel(QLabel* label);
void StyleTableView(QTableView* tableView);
void StyleTableWidget(QTableWidget* tableWidget);
void StyleCheckBox(QCheckBox* checkBox);
void StyleComboBox(QComboBox* comboBox);
void StyleDateEdit(QDateEdit* dateEdit);

#endif // DESIGN_H
