#pragma once

#include "xlsxdocument.h"
#include <QTableWidget>
#include <QFileDialog>
#include <QMessageBox>

#include "universal/filepath/filepath.h"

// 读取xlsx文件
void importFromXlsx(QTableWidget* tableWidget);
void importFromXlsx(QTableWidget* tableWidget, QString fileName);

// 导出xlsx文件
void exportToXlsx(QTableWidget* tableWidget);
void exportToXlsx(QTableWidget* tableWidget, QString fileName);
