#include "filepath.h"

QString tips_file_path(QString file_path)
{
	if (QFile::exists(file_path))
	{
		return file_path;
	}
	else {
		return "resources/" + file_path;
	}
}

void check_file_path(QString file_path)
{
	QFileInfo file_info(file_path);
	check_file_path(file_info);
}

void check_file_path(QFileInfo file_info)
{
	// 判断文件路径是否存在
	if (!file_info.absoluteDir().exists())
		file_info.absoluteDir().mkpath(".");
}

void populateTree(const QString dir, QTreeWidgetItem* parent)
{
	QDir directory(dir);
	if (!directory.exists())
		return;
	qDebug() << "populateTree: " << directory.absolutePath();
	QFileInfoList file_list = directory.entryInfoList();
	qDebug() << "file_list: " << file_list;
	for (int i = 0; i < file_list.size(); i++)
	{
		QFileInfo file_info = file_list.at(i);
		if (file_info.fileName() == "." || file_info.fileName() == "..") {
			continue;
		}
		else if (file_info.isDir())
		{
			QTreeWidgetItem* item = new QTreeWidgetItem(parent, QStringList(file_info.fileName()));
			populateTree(file_info.absoluteFilePath(), item);
		}
		else
		{
			QTreeWidgetItem* item = new QTreeWidgetItem(parent, QStringList(file_info.fileName()));
		}
	}
}
