#include "design.h"

void StyleButton(QPushButton* button) {
    button->setStyleSheet(
        "QPushButton {"
        "  background-color: #3498db;"
        "  color: white;"
        "  border-radius: 8px;"
        "  padding: 6px 12px;"
        "}"
        "QPushButton:hover {"
        "  background-color: #2980b9;"
        "}"
    );
}

void StyleLineEdit(QLineEdit* lineEdit) {
    lineEdit->setStyleSheet(
        "QLineEdit {"
        "  border: 1px solid #bdc3c7;"
        "  border-radius: 4px;"
        "  padding: 4px;"
        "}"
    );
}

void StyleLabel(QLabel* label) {
    label->setStyleSheet(
        "QLabel {"
        "  font-size: 14px;"
        "  color: #2c3e50;"
        "}"
    );
}

void StyleTableView(QTableView* tableView) {
    tableView->setStyleSheet(
        "QTableView {"
        "  border: 1px solid #bdc3c7;"
        "  border-radius: 6px;"
        "  gridline-color: #ecf0f1;"
        "  background-color: #ffffff;"
        "  selection-background-color: #3498db;"
        "  selection-color: white;"
        "}"
        "QHeaderView::section {"
        "  background-color: #ecf0f1;"
        "  padding: 4px;"
        "  border: 1px solid #bdc3c7;"
        "  font-weight: bold;"
        "}"
    );
    tableView->setAlternatingRowColors(true);
}

void StyleCheckBox(QCheckBox* checkBox) {
    checkBox->setStyleSheet(
        "QCheckBox {"
        "  spacing: 6px;"
        "  color: #2c3e50;"
        "  font-size: 14px;"
        "}"
        "QCheckBox::indicator {"
        "  width: 16px;"
        "  height: 16px;"
        "}"
        "QCheckBox::indicator:unchecked {"
        "  border: 1px solid #bdc3c7;"
        "  background-color: #ffffff;"
        "}"
        "QCheckBox::indicator:checked {"
        "  border: 1px solid #3498db;"
        "  background-color: #3498db;"
        "}"
    );
}

void StyleComboBox(QComboBox* comboBox) {
    comboBox->setStyleSheet(
        "QComboBox {"
        "  border: 1px solid #bdc3c7;"
        "  border-radius: 4px;"
        "  padding: 4px 8px;"
        "  background-color: #ffffff;"
        "}"
        "QComboBox:hover {"
        "  border: 1px solid #3498db;"
        "}"
        "QComboBox QAbstractItemView {"
        "  border: 1px solid #bdc3c7;"
        "  selection-background-color: #3498db;"
        "  selection-color: white;"
        "}"
    );
}

void StyleDateEdit(QDateEdit* dateEdit) {
    dateEdit->setStyleSheet(
        "QDateEdit {"
        "  border: 1px solid #bdc3c7;"
        "  border-radius: 4px;"
        "  padding: 4px;"
        "  background-color: #ffffff;"
        "}"
        "QDateEdit::drop-down {"
        "  subcontrol-origin: padding;"
        "  subcontrol-position: top right;"
        "  width: 20px;"
        "  border-left: 1px solid #bdc3c7;"
        "}"
        "QDateEdit::down-arrow {"
        "  image: url(:/icons/arrow-down.png);"  // Optional: add your own icon here
        "}"
    );
}
