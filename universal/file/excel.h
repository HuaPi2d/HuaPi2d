#pragma once

#include "xlsxdocument.h"
#include <QTableWidget>
#include <QFileDialog>
#include <QMessageBox>

#include "universal/filepath/filepath.h"

// ��ȡxlsx�ļ�
void importFromXlsx(QTableWidget* tableWidget);
void importFromXlsx(QTableWidget* tableWidget, QString fileName);

// ����xlsx�ļ�
void exportToXlsx(QTableWidget* tableWidget);
void exportToXlsx(QTableWidget* tableWidget, QString fileName);
