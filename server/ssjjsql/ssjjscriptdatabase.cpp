#include "ssjjscriptdatabase.h"

SSJJScriptDataBase::SSJJScriptDataBase()
    : QSqlDatabase{ QSqlDatabase::addDatabase("QSQLITE")} // 调用 QSqlDatabase 的构造函数，添加一个 SQLite 数据库
{

}

SSJJScriptDataBase::~SSJJScriptDataBase()
{
}

SSJJScriptDataBase SSJJScriptDataBase::getSSJJScriptDatabase(QString databaseName)
{
    // 声明一个 SSJJScriptDataBase 对象
    SSJJScriptDataBase ssjjScriptDataBase;
    // 设置数据库名称
    ssjjScriptDataBase.setDatabaseName(databaseName);
    // 返回 SSJJScriptDataBase 对象
    return ssjjScriptDataBase;
}

QStringList SSJJScriptDataBase::SSJJScriptDataBaseDriver()
{
    return QSqlDatabase::drivers(); // 调用 QSqlDatabase 的静态函数
}
