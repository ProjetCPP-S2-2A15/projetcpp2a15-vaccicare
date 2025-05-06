#ifndef DESIGN_H
#define DESIGN_H

#include <QPushButton>
#include <QLineEdit>
#include <QLabel>
#include <QTextEdit>
#include <QTableView>
#include <QTableWidget>
#include <QCheckBox>
#include <QComboBox>
#include <QDateEdit>
#include <QDialogButtonBox>

void StyleButtonGreen(QPushButton* button);
void StyleButtonBlue(QPushButton* button);
void StyleButtonRed(QPushButton* button);
void StyleButtonDarkred(QPushButton* button);
void StyleButtonDarkblue(QPushButton* button);


void StyleLineEdit(QLineEdit* lineEdit);
void StyleLabel(QLabel* label);
void StyleTextEdit(QTextEdit *textEdit);
void StyleTableView(QTableView* tableView);
void StyleTableWidget(QTableWidget* tableWidget);
void StyleCheckBox(QCheckBox* checkBox);
void StyleComboBox(QComboBox* comboBox);
void StyleDateEdit(QDateEdit* dateEdit);
void StyleDialogButtonBox(QDialogButtonBox *buttonBox);


#endif // DESIGN_H
