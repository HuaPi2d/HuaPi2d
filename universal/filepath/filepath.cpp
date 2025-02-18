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
	QFileInfoList file_list = directory.entryInfoList();
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

QString getAbsolutePath(QString& file_path)
{
	QFileInfo file_info(file_path);
	QFile file(file_info.absoluteFilePath());
	if (file.exists())
	{
		return file_info.absoluteFilePath();
	}
	else
	{
		if(QFile::exists("x64/Release/" + file_path))
			return "x64/Release/" + file_path;
		else
			return file_path;
	}
}

QString getRelativePath(const QString& file_path)
{
	QString current_app_path = qApp->applicationDirPath().replace("/x64/Release", "");
	QString new_file_path = file_path;
	if (new_file_path.contains(current_app_path))
	{
		return new_file_path.replace(current_app_path + "/", "");
	}
	else {
		return new_file_path;
	}
}
