#include "excel.h"

void importFromXlsx(QTableWidget* tableWidget)
{
    QString fileName = QFileDialog::getOpenFileName(nullptr, "Open XLSX", "", "Excel Files (*.xlsx)");
    if (fileName.isEmpty()) {
        return;
    }

    importFromXlsx(tableWidget, fileName);
}

void importFromXlsx(QTableWidget* tableWidget, QString fileName)
{
    if (!QFile::exists(fileName)) {
        return;
    }

    QXlsx::Document xlsx(fileName);

    // ������б������
    tableWidget->clearContents();

    // ��ȡ Excel �ļ���������
    int rowCount = xlsx.dimension().rowCount();
    int colCount = xlsx.dimension().columnCount();

    // ���ñ���С
    tableWidget->setRowCount(rowCount);
    tableWidget->setColumnCount(colCount);

    // ���� Excel �ļ���ÿ����Ԫ�񣬲����õ� QTableWidget ��
    for (int row = 1; row <= rowCount; ++row) {
        for (int col = 1; col <= colCount; ++col) {
            QVariant value = xlsx.read(row, col);
            if (value.isValid()) {
                QTableWidgetItem* newItem = new QTableWidgetItem(value.toString());
                tableWidget->setItem(row - 1, col - 1, newItem);
            }
        }
    }
}

void exportToXlsx(QTableWidget* tableWidget)
{
    QString fileName = QFileDialog::getSaveFileName(nullptr, "Save as XLSX", "", "Excel Files (*.xlsx)");
    if (fileName.isEmpty()) {
        return;
    }

    exportToXlsx(tableWidget, fileName);
}

void exportToXlsx(QTableWidget* tableWidget, QString fileName)
{
    check_file_path(fileName);
    if(tableWidget->rowCount() == 0 || tableWidget->columnCount() == 0)
        return;

    QXlsx::Document xlsx;

    // ��������е����ݲ�д�� XLSX
    for (int row = 0; row < tableWidget->rowCount(); ++row) {
        for (int col = 0; col < tableWidget->columnCount(); ++col) {
            QTableWidgetItem* item = tableWidget->item(row, col);
            if (item) {
                xlsx.write(row + 1, col + 1, item->text());
            }
        }
    }

    // �����ļ�
    if (!xlsx.saveAs(fileName)) {
        QMessageBox::warning(nullptr, "Export Failed", "Failed to export the file.");
    }
}
