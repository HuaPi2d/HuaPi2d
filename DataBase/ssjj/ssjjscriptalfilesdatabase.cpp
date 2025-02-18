#include "ssjjscriptalfilesdatabase.h"

SSJJScriptalFilesDatabase::SSJJScriptalFilesDatabase(const QString& dbName, QObject* parent)
	: LocalDatabase(dbName, parent)
{
	// 定义表名称
	zx_tableName = "zx_ssjj_scriptal_files";
	ld_tableName = "ld_ssjj_scriptal_files";

	// 定义表结构
	zx_columns["id"] = "INTEGER PRIMARY KEY AUTOINCREMENT";
	zx_columns["chapter"] = "TEXT";
	zx_columns["level"] = "TEXT";
	zx_columns["difficulty"] = "TEXT";
	zx_columns["fullfilepath"] = "TEXT";

	ld_columns["id"] = "INTEGER PRIMARY KEY AUTOINCREMENT";
	ld_columns["fullfilepath"] = "TEXT";

	// 创建表
	createTable(zx_tableName, zx_columns, "UNIQUE(fullfilepath)");
	createTable(ld_tableName, ld_columns, "UNIQUE(fullfilepath)");

	// 读取文件夹下的数据至数据库
	zx_defaultFileSavePath = QDir::currentPath() + "/scriptal/zx";
	ld_defaultFileSavePath = QDir::currentPath() + "/scriptal/ld";
	deleteEmptyFilesFromDatabase();
	readFilesIntoDatabase(QDir(zx_defaultFileSavePath));
	readFilesIntoDatabase(QDir(ld_defaultFileSavePath));
}

SSJJScriptalFilesDatabase::~SSJJScriptalFilesDatabase()
{}

QString SSJJScriptalFilesDatabase::getTableName(QString type) const
{
	if (type == "zx")
		return zx_tableName;
	else if (type == "ld")
		return ld_tableName;
	else
		return QString();
}

void SSJJScriptalFilesDatabase::readFilesIntoDatabase(QDir dir)
{
	// 读取文件夹下后缀为 scp, lscp, zscp 的文件
	QStringList filters;
	filters << "*.scp" << "*.lscp" << "*.zscp";
	dir.setNameFilters(filters);
	dir.setFilter(QDir::Files | QDir::NoSymLinks);
	dir.setSorting(QDir::Name);

	// 遍历文件
	QFileInfoList fileInfoList = dir.entryInfoList();
	for (QFileInfo fileInfo : fileInfoList)
	{
		readFileIntoDatabase(fileInfo);
	}
}

void SSJJScriptalFilesDatabase::readFileIntoDatabase(QFileInfo fileInfo)
{
	if (fileInfo.suffix() == "zscp") {
		QMap<QString, QString> attributes = readFileAttributesMap(fileInfo.absoluteFilePath());
		QMap<QString, QVariant> values;
		values["chapter"] = attributes["chapter"];
		values["level"] = attributes["level"];
		values["difficulty"] = attributes["difficulty"];
		values["fullfilepath"] = fileInfo.absoluteFilePath();
		insertData(zx_tableName, values);
	}
	else if (fileInfo.suffix() == "lscp") {
		QMap<QString, QVariant> values;
		values["fullfilepath"] = fileInfo.absoluteFilePath();
		insertData(ld_tableName, values);
	}
}

QStringList SSJJScriptalFilesDatabase::getFilesFromDatabase(QString level)
{
	QVector<QMap<QString, QVariant>> data = queryData(zx_tableName, "level", level);
	QVector<QVariant> fileList = extractColumn(data, "fullfilepath");
	QStringList filePaths;
	for (QVariant filePath : fileList)
		filePaths << filePath.toString();
	return filePaths;
}

QStringList SSJJScriptalFilesDatabase::getAllFilesPath(QString type)
{
	if (type == "zx") {
		QVector<QMap<QString, QVariant>> data = getAllData(zx_tableName);
		QVector<QVariant> fileList = extractColumn(data, "fullfilepath");
		QStringList filePaths;
		for (QVariant filePath : fileList)
			filePaths << filePath.toString();
		return filePaths;
	}
	else if (type == "ld") {
		QVector<QMap<QString, QVariant>> data = getAllData(ld_tableName);
		QVector<QVariant> fileList = extractColumn(data, "fullfilepath");
		QStringList filePaths;
		for (QVariant filePath : fileList)
			filePaths << filePath.toString();
		return filePaths;
	}
	else
		return QStringList();
}

void SSJJScriptalFilesDatabase::deleteFileFromDatabase(QString filePath)
{
	QFileInfo fileInfo(filePath);
	if (fileInfo.suffix() == "zscp")
		deleteDataByProperty(zx_tableName, "fullfilepath", filePath);
	else if (fileInfo.suffix() == "lscp")
		deleteDataByProperty(ld_tableName, "fullfilepath", filePath);
}

void SSJJScriptalFilesDatabase::updateZXFileInDatabase(QString oldFilePath, QString newfilePath)
{
	deleteDataByProperty(zx_tableName, "fullfilepath", oldFilePath);
	readFileIntoDatabase(QFileInfo(newfilePath));
}

void SSJJScriptalFilesDatabase::deleteEmptyFilesFromDatabase()
{
	QVector<QMap<QString, QVariant>> data = getAllData(zx_tableName);
	for (QMap<QString, QVariant> row : data) {
		QString filePath = row["fullfilepath"].toString();
		if (!QFile::exists(filePath)) {
			deleteDataByProperty(zx_tableName, "fullfilepath", row["fullfilepath"]);
		}
		
	}
	data = getAllData(ld_tableName);
	for (QMap<QString, QVariant> row : data) {
		QString filePath = row["fullfilepath"].toString();
		if (!QFile::exists(filePath)) {
			deleteDataByProperty(ld_tableName, "fullfilepath", row["fullfilepath"]);
		}
	}
}
