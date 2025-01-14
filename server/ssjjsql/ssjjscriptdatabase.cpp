#include "ssjjscriptdatabase.h"

SSJJScriptDataBase::SSJJScriptDataBase()
    : QSqlDatabase{ QSqlDatabase::addDatabase("QSQLITE")} // ���� QSqlDatabase �Ĺ��캯�������һ�� SQLite ���ݿ�
{

}

SSJJScriptDataBase::~SSJJScriptDataBase()
{
}

SSJJScriptDataBase SSJJScriptDataBase::getSSJJScriptDatabase(QString databaseName)
{
    // ����һ�� SSJJScriptDataBase ����
    SSJJScriptDataBase ssjjScriptDataBase;
    // �������ݿ�����
    ssjjScriptDataBase.setDatabaseName(databaseName);
    // ���� SSJJScriptDataBase ����
    return ssjjScriptDataBase;
}

QStringList SSJJScriptDataBase::SSJJScriptDataBaseDriver()
{
    return QSqlDatabase::drivers(); // ���� QSqlDatabase �ľ�̬����
}
