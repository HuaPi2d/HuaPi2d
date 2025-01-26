#pragma once
#include <QSqlDatabase>
#include <QObject>
#include <QStringList>
#include <QSqlError>

class SSJJScriptDataBase : public QSqlDatabase // 改为继承 QObject
{
public:
    explicit SSJJScriptDataBase();
    ~SSJJScriptDataBase();

    static SSJJScriptDataBase getSSJJScriptDatabase(QString databaseName = "SSJJScriptDatabase.db");

    static QStringList SSJJScriptDataBaseDriver();
};

