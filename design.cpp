#include "design.h"


void StyleButton(QPushButton* button) {
    button->setStyleSheet(
        "QPushButton {"
        "  background-color: #0245a3;"
        "  color: white;"
        "  border-radius: 8px;"
        "  padding: 6px 12px;"
        "}"
        "QPushButton:hover {"
        "  background-color: #01398a;"
        "}"
        );
}

void StyleLineEdit(QLineEdit* lineEdit) {
    lineEdit->setStyleSheet(
        "QLineEdit {"
        "  border: 1px solid #8fbaf3;"
        "  border-radius: 4px;"
        "  padding: 4px;"
        "}"
        );
}

void StyleLabel(QLabel* label) {
    label->setStyleSheet(
        "QLabel {"
        "  font-size: 14px;"
        "  color: #0245a3;"
        "  font-weight: bold;"
        "}"
        );
}

void StyleTableView(QTableView* tableView) {
    tableView->setStyleSheet(
        "QTableView {"
        "  border: 1px solid #8fbaf3;"
        "  border-radius: 6px;"
        "  gridline-color: #d0e5fb;"
        "  background-color: #ffffff;"
        "  selection-background-color: #7ed957;"
        "  selection-color: white;"
        "}"
        "QHeaderView::section {"
        "  background-color: #8fbaf3;"
        "  padding: 4px;"
        "  border: 1px solid #bdf1f6;"
        "  font-weight: bold;"
        "}"
        );
    tableView->setAlternatingRowColors(true);
}

void StyleTableWidget(QTableWidget* tableWidget) {
    tableWidget->setStyleSheet(
        "QTableWidget {"
        "  border: 1px solid #8fbaf3;"
        "  border-radius: 6px;"
        "  gridline-color: #d0e5fb;"
        "  background-color: #ffffff;"
        "  selection-background-color: #7ed957;"
        "  selection-color: black;"
        "}"
        "QHeaderView::section {"
        "  background-color: #8fbaf3;"
        "  padding: 4px;"
        "  border: 1px solid #bdf1f6;"
        "  font-weight: bold;"
        "}"
        "QTableWidget::item:selected {"
        "  background-color: #7ed957;"
        "  color: black;"
        "}"
        );
    tableWidget->setAlternatingRowColors(true);
}


void StyleCheckBox(QCheckBox* checkBox) {
    checkBox->setStyleSheet(
        "QCheckBox {"
        "  spacing: 6px;"
        "  color: #0245a3;"
        "  font-size: 14px;"
        "}"
        "QCheckBox::indicator {"
        "  width: 16px;"
        "  height: 16px;"
        "}"
        "QCheckBox::indicator:unchecked {"
        "  border: 1px solid #8fbaf3;"
        "  background-color: #ffffff;"
        "}"
        "QCheckBox::indicator:checked {"
        "  border: 1px solid #0245a3;"
        "  background-color: #7ed957;"
        "}"
        );
}

void StyleComboBox(QComboBox* comboBox) {
    comboBox->setStyleSheet(
        "QComboBox {"
        "  border: 1px solid #8fbaf3;"
        "  border-radius: 4px;"
        "  padding: 4px 8px;"
        "  background-color: #ffffff;"
        "}"
        "QComboBox:hover {"
        "  border: 1px solid #0245a3;"
        "}"
        "QComboBox QAbstractItemView {"
        "  border: 1px solid #8fbaf3;"
        "  selection-background-color: #7ed957;"
        "  selection-color: black;"
        "}"
        );
}

void StyleDateEdit(QDateEdit* dateEdit) {
    dateEdit->setStyleSheet(
        "QDateEdit {"
        "  border: 1px solid #8fbaf3;"
        "  border-radius: 4px;"
        "  padding: 4px;"
        "  background-color: #ffffff;"
        "}"
        "QDateEdit::drop-down {"
        "  subcontrol-origin: padding;"
        "  subcontrol-position: top right;"
        "  width: 20px;"
        "  border-left: 1px solid #8fbaf3;"
        "  background-color: #bdf1f6;"
        "}"

        );
}


