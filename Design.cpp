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
