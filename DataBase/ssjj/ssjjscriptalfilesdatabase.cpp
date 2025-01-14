#include "ssjjscriptalfilesdatabase.h"

SSJJScriptalFilesDatabase::SSJJScriptalFilesDatabase(const QString& dbName, QObject* parent)
	: LocalDatabase(dbName, parent)
{
	// ���������
	zx_tableName = "zx_ssjj_scriptal_files";

	// �����ṹ
	zx_columns["id"] = "INTEGER PRIMARY KEY AUTOINCREMENT";
	zx_columns["chapter"] = "TEXT";
	zx_columns["level"] = "TEXT";
	zx_columns["difficulty"] = "TEXT";
	zx_columns["fullfilepath"] = "TEXT";

	// ������
	createTable(zx_tableName, zx_columns, "UNIQUE(fullfilepath)");

	// ��ȡ�ļ����µ����������ݿ�
	zx_defaultFileSavePath = QDir::currentPath() + "/scriptal/zx";
	deleteEmptyFilesFromDatabase();
	readFilesIntoDatabase(QDir(zx_defaultFileSavePath));
}

SSJJScriptalFilesDatabase::~SSJJScriptalFilesDatabase()
{}

QString SSJJScriptalFilesDatabase::getTableName(QString type) const
{
	if (type == "zx")
		return zx_tableName;
	else
		return QString();
}

void SSJJScriptalFilesDatabase::readFilesIntoDatabase(QDir dir)
{
	// ��ȡ�ļ����º�׺Ϊ scp, lscp, zscp ���ļ�
	QStringList filters;
	filters << "*.scp" << "*.lscp" << "*.zscp";
	dir.setNameFilters(filters);
	dir.setFilter(QDir::Files | QDir::NoSymLinks);
	dir.setSorting(QDir::Name);

	// �����ļ�
	QFileInfoList fileInfoList = dir.entryInfoList();
	for (QFileInfo fileInfo : fileInfoList)
	{
		readFileIntoDatabase(fileInfo);
	}
}

void SSJJScriptalFilesDatabase::readFileIntoDatabase(QFileInfo fileInfo)
{
	if (fileInfo.suffix() == "zscp") {
		QList<FileAttribute> attributes = readFileAttributes(fileInfo.absoluteFilePath());
		QMap<QString, QVariant> values;
		for (FileAttribute attribute : attributes) {
			if (attribute.name == "chapter")
				values["chapter"] = attribute.value;
			else if (attribute.name == "level")
				values["level"] = attribute.value;
			else if (attribute.name == "difficulty")
				values["difficulty"] = attribute.value;
		}
		values["fullfilepath"] = fileInfo.absoluteFilePath();
		insertData(zx_tableName, values);
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
	else
		return QStringList();
}

void SSJJScriptalFilesDatabase::deleteEmptyFilesFromDatabase()
{
	QVector<QMap<QString, QVariant>> data = getAllData(zx_tableName);
	for (QMap<QString, QVariant> row : data) {
		QString filePath = row["fullfilepath"].toString();
		if (!QFile::exists(filePath))
			deleteDataByProperty(zx_tableName, "fullfilepath", row["fullfilepath"]);
	}
}
