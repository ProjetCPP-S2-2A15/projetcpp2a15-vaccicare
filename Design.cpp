#include "design.h"
#include "QStandardItemModel"
#include "QStandardItem"


void StyleButtonGreen(QPushButton* button) {
    button->setStyleSheet(
        "QPushButton {"
        "  background-color: #7ed957;"         // Green for Add
        "  color: white;"
        "  border-radius: 8px;"
        "  padding: 6px 12px;"
        "  font-weight: bold;"
        "}"
        "QPushButton:hover {"
        "  background-color: #9beb79;"
        "}"
        "QPushButton:pressed {"
        "  background-color: #7ed957;"
        "}"
        );
}

void StyleButtonBlue(QPushButton* button) {
    button->setStyleSheet(
        "QPushButton {"
        "  background-color: #8fbaf3;"         // Blue for Modify
        "  color: white;"
        "  border-radius: 8px;"
        "  padding: 6px 12px;"
        "  font-weight: bold;"
        "}"
        "QPushButton:hover {"
        "  background-color: #bdf1f6;"
        "}"
        "QPushButton:pressed {"
        "  background-color: #8fbaf3;"
        "}"
        );
}

void StyleButtonRed(QPushButton* button) {
    button->setStyleSheet(
        "QPushButton {"
        "  background-color: #d50000;"         // Red for Delete
        "  color: white;"
        "  border-radius: 8px;"
        "  padding: 6px 12px;"
        "  font-weight: bold;"
        "}"
        "QPushButton:hover {"
        "  background-color: #f52e2e;"
        "}"
        "QPushButton:pressed {"
        "  background-color: #d50000;"
        "}"
        );
}

void StyleButtonDarkred(QPushButton* button) {
    button->setStyleSheet(
        "QPushButton {"
        "  background-color: #ac0000;"         // Dark red for EXPORTpdf
        "  color: white;"
        "  border-radius: 8px;"
        "  padding: 6px 12px;"
        "  font-weight: bold;"
        "}"
        "QPushButton:hover {"
        "  background-color: #d50000;"
        "}"
        "QPushButton:pressed {"
        "  background-color: #ac0000;"
        "}"
        );
}

void StyleButtonDarkblue(QPushButton* button) {
    button->setStyleSheet(
        "QPushButton {"
        "  background-color: #0245a3;"         // Dark blue for OTHERbtn
        "  color: white;"
        "  border-radius: 8px;"
        "  padding: 6px 12px;"
        "  font-weight: bold;"
        "}"
        "QPushButton:hover {"
        "  background-color: #8fbaf3;"
        "}"
        "QPushButton:pressed {"
        "  background-color: #0245a3;"
        "}"
        );
}


void StyleLineEdit(QLineEdit* lineEdit) {
    lineEdit->setStyleSheet(
        "QLineEdit {"
        "  border: 1.5px solid #0245a3;"
        "  border-radius: 4px;"
        "  padding: 4px;"
        "  background-color: #ffffff;"
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
        "  gridline-color: #b2e0f5;"
        "  selection-background-color: #00cc99;"
        "  selection-color: white;"
        "}"
        "QHeaderView::section {"
        "  background-color: #8fbaf3;"
        "  padding: 6px;"
        "  border: 1px solid #bdf1f6;"
        "  font-weight: bold;"
        "  color: #003f73;"
        "}"
        );

    tableView->setAlternatingRowColors(false);

    // Optional: set row color via model if it's QStandardItemModel
    auto model = qobject_cast<QStandardItemModel*>(tableView->model());
    if (model) {
        for (int row = 0; row < model->rowCount(); ++row) {
            QColor rowColor = (row % 2 == 0) ? QColor("#bdf1f6") : QColor("#ebf1ff");
            for (int col = 0; col < model->columnCount(); ++col) {
                QStandardItem* item = model->item(row, col);
                if (item)
                    item->setBackground(rowColor);
            }
        }
    }
}

void StyleTextEdit(QTextEdit *textEdit) {
    textEdit->setStyleSheet(
        "QTextEdit {"
        " background-color: #f0f0f0;"
        " color: #000000;"
        " border: 1px solid #ccc;"
        " border-radius: 6px;"
        " padding: 6px;"
        " font-size: 14px;"
        "}"
        );
}

void StyleTableWidget(QTableWidget* tableWidget) {
    tableWidget->setStyleSheet(
        "QTableWidget {"
        "  border: 1px solid #8fbaf3;"
        "  border-radius: 6px;"
        "  gridline-color: #b2e0f5;"
        "  selection-background-color: #00cc99;"
        "  selection-color: black;"
        "}"
        "QHeaderView::section {"
        "  background-color: #8fbaf3;"
        "  padding: 6px;"
        "  border: 1px solid #bdf1f6;"
        "  font-weight: bold;"
        "  color: #003f73;"
        "}"
        "QTableWidget::item:selected {"
        "  background-color: #00cc99;"
        "  color: black;"
        "}"
        );

    tableWidget->setAlternatingRowColors(false);

    // Manual alternation
    for (int row = 0; row < tableWidget->rowCount(); ++row) {
        QColor rowColor = (row % 2 == 0) ? QColor("#bdf1f6") : QColor("#ebf1ff");
        for (int col = 0; col < tableWidget->columnCount(); ++col) {
            QTableWidgetItem* item = tableWidget->item(row, col);
            if (item)
                item->setBackground(rowColor);
        }
    }
}


void StyleCheckBox(QCheckBox* checkBox) {
    checkBox->setStyleSheet(
        "QCheckBox {"
        "  spacing: 6px;"
        "  color: #ac0000;"
        "  font-size: 14px;"
        "}"
        "QCheckBox::indicator {"
        "  width: 16px;"
        "  height: 16px;"
        "}"
        "QCheckBox::indicator:unchecked {"
        "  border: 1px solid #ac0000;"
        "  background-color: #ffffff;"
        "}"
        "QCheckBox::indicator:checked {"
        "  border: 1px solid #7ed957;"
        "  background-color: #ffffff;"
        "}"
        );
}

void StyleComboBox(QComboBox* comboBox) {
    comboBox->setStyleSheet(
        "QComboBox {"
        "  border: 1.5px solid #0245a3;"
        "  border-radius: 4px;"
        "  padding: 4px 8px;"
        "  background-color: #ffffff;"
        "}"
        "QComboBox:hover {"
        "  border: 1px solid #8fbaf3;"
        "}"
        "QComboBox QAbstractItemView {"
        "  border: 1px solid #8fbaf3;"
        "  selection-background-color: #ffffff;"
        "  selection-color: black;"
        "}"
        );
}

void StyleDateEdit(QDateEdit* dateEdit) {
    dateEdit->setStyleSheet(
        "QDateEdit {"
        "  border: 1.5px solid #0245a3;"
        "  border-radius: 4px;"
        "  padding: 4px;"
        "  background-color: #ffffff;"
        "}"
        "QDateEdit::drop-down {"
        "  subcontrol-origin: padding;"
        "  subcontrol-position: top right;"
        "  width: 20px;"
        "  border-left: 1px solid #0245a3;"
        "  background-color: #ffffff;"
        "}"
        );
}

void StyleDialogButtonBox(QDialogButtonBox *buttonBox)
{
    buttonBox->setStyleSheet(
        "QDialogButtonBox QPushButton {"
        " background-color: #0245a3;"
        " color: white;"
        " border: none;"
        " border-radius: 6px;"
        " padding: 6px 12px;"
        " font-size: 14px;"
        "}"
        "QDialogButtonBox QPushButton:hover {"
        " background-color: #8fbaf3;"
        "}"
        "QDialogButtonBox QPushButton:pressed {"
        " background-color: #0245a3;"
        "}"
        );
}
