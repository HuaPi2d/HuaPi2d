#pragma once

#include <QObject>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include <QVariant>
#include <QVector>
#include <QMap>
#include <QSqlRecord>

#include "universal/filepath/filepath.h"


// 本地数据库类
class LocalDatabase  : public QObject
{
	Q_OBJECT

public:
	LocalDatabase(const QString& dbName, QObject* parent);
	~LocalDatabase();

	bool createTable(const QString& tableName, const QMap<QString, QString>& columns, const QString& uniqueKey = "");
	bool insertData(const QString& tableName, const QMap<QString, QVariant>& data);
	QVector<QMap<QString, QVariant>> queryData(const QString& tableName, const QString& column, const QVariant& value);
	QVector<QVariant> extractColumn(const QVector<QMap<QString, QVariant>>& data, const QString& columnName);
	bool deleteDataByProperty(const QString& tableName, const QString& columnName, const QVariant& value);
	QVector<QMap<QString, QVariant>> getAllData(const QString& tableName);

private:
	QSqlDatabase db;
	bool checkError(const QSqlQuery& query);
};
