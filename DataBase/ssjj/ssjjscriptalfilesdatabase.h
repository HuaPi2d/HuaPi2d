#pragma once

#include <QString>
#include <QDir>
#include <QDirIterator>

#include "universal/file/fileattributes.h"
#include "DataBase/localdatabase.h"

class SSJJScriptalFilesDatabase  : public LocalDatabase
{
	Q_OBJECT

public:
	SSJJScriptalFilesDatabase(const QString& dbName, QObject* parent);
	~SSJJScriptalFilesDatabase();

	QString getTableName(QString type) const;
	// ���ļ���ȡ�����ݿ�
	void readFilesIntoDatabase(QDir dir);
	void readFileIntoDatabase(QFileInfo fileInfo);
	// ���ݹؿ���Ϣ��ȡ�ű��ļ�
	QStringList getFilesFromDatabase(QString level);
	QStringList getAllFilesPath(QString type);


private:
	QString zx_tableName;
	QMap<QString, QString> zx_columns;
	QString zx_defaultFileSavePath;

	void deleteEmptyFilesFromDatabase();
};
