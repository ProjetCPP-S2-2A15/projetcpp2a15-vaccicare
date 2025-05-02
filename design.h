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

void StyleButton(QPushButton* button, const QString& type = "default");
void StyleLineEdit(QLineEdit* lineEdit);
void StyleLabel(QLabel* label);
void StyleTableView(QTableView* tableView);
void StyleTableWidget(QTableWidget* tableWidget);
void StyleCheckBox(QCheckBox* checkBox);
void StyleComboBox(QComboBox* comboBox);
void StyleDateEdit(QDateEdit* dateEdit);

#endif // DESIGN_H
