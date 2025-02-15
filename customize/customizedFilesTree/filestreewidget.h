#pragma once

#include <QTreeWidget>
#include <QApplication>

#include "universal/filepath/filepath.h"

class FilesTreeWidget  : public QTreeWidget
{
	Q_OBJECT

public:
	FilesTreeWidget(QWidget *parent);
	~FilesTreeWidget();

public slots:
	void setRootPath();
	void addTopLevelDir(const QString& dirPath);

private:
	QString rootPath;
};
