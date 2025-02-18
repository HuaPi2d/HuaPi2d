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

    // 清空现有表格数据
    tableWidget->clearContents();

    // 获取 Excel 文件的行列数
    int rowCount = xlsx.dimension().rowCount();
    int colCount = xlsx.dimension().columnCount();

    // 设置表格大小
    tableWidget->setRowCount(rowCount);
    tableWidget->setColumnCount(colCount);

    // 遍历 Excel 文件的每个单元格，并设置到 QTableWidget 中
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

    // 遍历表格中的数据并写入 XLSX
    for (int row = 0; row < tableWidget->rowCount(); ++row) {
        for (int col = 0; col < tableWidget->columnCount(); ++col) {
            QTableWidgetItem* item = tableWidget->item(row, col);
            if (item) {
                xlsx.write(row + 1, col + 1, item->text());
            }
        }
    }

    // 保存文件
    if (!xlsx.saveAs(fileName)) {
        QMessageBox::warning(nullptr, "Export Failed", "Failed to export the file.");
    }
}
