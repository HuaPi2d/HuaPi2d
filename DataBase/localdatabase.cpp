#include "localdatabase.h"

LocalDatabase::LocalDatabase(const QString& dbName, QObject* parent)
	: QObject(parent)
{
    QString newDbName = dbName;
    newDbName = "databases/" + newDbName;
    check_file_path(newDbName);
    db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName(newDbName);

    if (!db.open()) {
        qCritical() << "Failed to open database:" << db.lastError().text();
    }
    else {
        qDebug() << "Database opened successfully!";
    }
}

LocalDatabase::~LocalDatabase()
{}

bool LocalDatabase::createTable(const QString & tableName, const QMap<QString, QString>&columns, const QString &uniqueKey)
{
    QStringList columnDefs;
    for (auto it = columns.begin(); it != columns.end(); ++it) {
        columnDefs.append(QString("%1 %2").arg(it.key(), it.value()));
    }

    if(uniqueKey != "")
        columnDefs.append(uniqueKey);

    QString sql = QString("CREATE TABLE IF NOT EXISTS %1 (%2)").arg(tableName, columnDefs.join(", "));
    QSqlQuery query;
    if (!query.exec(sql)) {
        return checkError(query);
    }

    return true;
}

bool LocalDatabase::insertData(const QString& tableName, const QMap<QString, QVariant>& data)
{
    QStringList columns = data.keys();
    QStringList placeholders;
    for (const QString& col : columns) {
        placeholders.append(":" + col);
    }

    QString sql = QString("INSERT INTO %1 (%2) VALUES (%3)")
        .arg(tableName, columns.join(", "), placeholders.join(", "));
    QSqlQuery query;
    query.prepare(sql);

    for (auto it = data.begin(); it != data.end(); ++it) {
        query.bindValue(":" + it.key(), it.value());
    }

    if (!query.exec()) {
        return checkError(query);
    }

    return true;
}

QVector<QMap<QString, QVariant>> LocalDatabase::queryData(const QString& tableName, const QString& column, const QVariant& value)
{
    QVector<QMap<QString, QVariant>> results;

    QString sql = QString("SELECT * FROM %1 WHERE %2 = :value").arg(tableName, column);
    QSqlQuery query;
    query.prepare(sql);
    query.bindValue(":value", value);

    if (!query.exec()) {
        checkError(query);
        return results;
    }

    while (query.next()) {
        QMap<QString, QVariant> row;
        for (int i = 0; i < query.record().count(); ++i) {
            row[query.record().fieldName(i)] = query.value(i);
        }
        results.append(row);
    }

    return results;
}

QVector<QVariant> LocalDatabase::extractColumn(const QVector<QMap<QString, QVariant>>& data, const QString& columnName)
{
    QVector<QVariant> columnValues;

    for (const auto& row : data) {
        if (row.contains(columnName)) {
            columnValues.append(row[columnName]);
        }
        else {
            qWarning() << "Column" << columnName << "not found in row.";
        }
    }

    return columnValues;
}

bool LocalDatabase::deleteDataByProperty(const QString& tableName, const QString& columnName, const QVariant& value)
{
    if (tableName.isEmpty() || columnName.isEmpty()) {
        qWarning() << "Table name or column name cannot be empty.";
        return false;
    }

    // Prepare the DELETE SQL query
    QString queryString = QString("DELETE FROM %1 WHERE %2 = :value").arg(tableName).arg(columnName);

    // Bind query to database connection
    QSqlQuery query(db);
    if (!query.prepare(queryString)) {
        qWarning() << "Failed to prepare SQL query:" << query.lastError().text();
        return false;
    }

    // Bind the value for the condition
    query.bindValue(":value", value);

    // Execute the query
    if (!query.exec()) {
        qWarning() << "Failed to delete data:" << query.lastError().text();
        return false;
    }

    return true;
}

QVector<QMap<QString, QVariant>> LocalDatabase::getAllData(const QString& tableName)
{
    QVector<QMap<QString, QVariant>> data;

    if (tableName.isEmpty()) {
        qWarning() << "Table name cannot be empty.";
        return data;
    }

    // Prepare the SELECT query to fetch all data
    QString queryString = QString("SELECT * FROM %1").arg(tableName);
    QSqlQuery query(db);

    if (!query.exec(queryString)) {
        qWarning() << "Failed to fetch data from table:" << tableName << query.lastError().text();
        return data;
    }

    // Process the results
    while (query.next()) {
        QMap<QString, QVariant> row;
        for (int i = 0; i < query.record().count(); ++i) {
            QString columnName = query.record().fieldName(i);
            QVariant value = query.value(i);
            row.insert(columnName, value);
        }
        data.append(row);
    }

    return data;
}

bool LocalDatabase::checkError(const QSqlQuery& query)
{
    if (query.lastError().isValid()) {
        qCritical() << "SQL error:" << query.lastError().text();
        return true;
    }
    return false;
}
