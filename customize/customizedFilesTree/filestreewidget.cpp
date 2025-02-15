#include "filestreewidget.h"

FilesTreeWidget::FilesTreeWidget(QWidget *parent)
	: QTreeWidget(parent)
{
}

FilesTreeWidget::~FilesTreeWidget()
{}

void FilesTreeWidget::setRootPath()
{
	rootPath = qApp->applicationDirPath(); 
	this->addTopLevelDir(rootPath);
}

void FilesTreeWidget::addTopLevelDir(const QString& dirPath)
{
	QTreeWidgetItem* item = new QTreeWidgetItem(this);
	item->setText(0, dirPath);
	populateTree(dirPath, item);
	this->addTopLevelItem(item);
}
