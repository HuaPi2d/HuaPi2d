#include "knowledgeitemsdatabase.h"

KnowledgeItemsDataBase::KnowledgeItemsDataBase(QString dbType)
	: QSqlDatabase(QSqlDatabase::addDatabase(dbType))
{}

KnowledgeItemsDataBase::~KnowledgeItemsDataBase()
{}

// 静态方法，用于创建KnowledgeItemsDataBase对象
KnowledgeItemsDataBase* KnowledgeItemsDataBase::getKnowledgeItemsDataBase(QString dbname)
{
	// 声明一个KnowledgeItemsDataBase对象
	KnowledgeItemsDataBase* knowledgeItemsDataBase = new KnowledgeItemsDataBase();
	// 设置数据库名称
	knowledgeItemsDataBase->setDatabaseName(dbname);
	// 返回KnowledgeItemsDataBase对象
	return knowledgeItemsDataBase;
}

// 创建knowledgeItems表
bool KnowledgeItemsDataBase::createKnowledgeItemsTable()
{
	// 定义SQL语句
	// 表名为knowledgeItems
	// 字段包括类型、问题、答案、创建时间、更新时间
	QString sql = "CREATE TABLE IF NOT EXISTS knowledgeItems("
		"id INTEGER PRIMARY KEY AUTOINCREMENT,"
		"type varchar(20) NOT NULL,"
		"question varchar(200) NOT NULL,"
		"answer varchar(200) NOT NULL,"
		"letestReviewTime varchar(20) NOT NULL,"
		"reviewCount varchar(20) NOT NULL"
		");";

	// 执行SQL语句
	return query.exec(sql);
}

// 插入一条知识条目
bool KnowledgeItemsDataBase::insertKnowledgeItem(KnowledgeItem item)
{
	// 定义SQL语句
	// 表名为knowledgeItems
	// 字段包括类型、问题、答案、创建时间、更新时间
	QString sql = "INSERT INTO knowledgeItems(type,question,answer,letestReviewTime,reviewCount) VALUES(?,?,?,?,?)";

	// 绑定参数
	query.prepare(sql);
	query.bindValue(0, item.type);
	query.bindValue(1, item.question);
	query.bindValue(2, item.answer);
	query.bindValue(3, item.latestReviewTime);
	query.bindValue(4, item.reviewCount);

	// 执行SQL语句
	return query.exec();
}

QList<KnowledgeItem> KnowledgeItemsDataBase::getKnowledgeItems(QString type)
{
	// 定义返回对象
	QList<KnowledgeItem> knowledgeItems;

	// 定义SQL语句
	QString sql;
	if (type == "")
	{
		sql = "SELECT * FROM knowledgeItems";
	}

	// 执行SQL语句
	if (!query.exec(sql))
	{
		qDebug() << "SQL查询数据执行失败：" << query.lastError().text();
	}
	else
	{
		// 循环读取结果集
		while (query.next())
		{
			// 读取一条数据
			KnowledgeItem item;
			item.id = query.value("id").toInt();
			item.type = query.value("type").toString();
			item.question = query.value("question").toString();
			item.answer = query.value("answer").toString();
			item.latestReviewTime = query.value("letestReviewTime").toString();
			item.reviewCount = query.value("reviewCount").toString();
			// 将数据添加到返回对象中
			knowledgeItems.append(item);
		}
	}
	
	return knowledgeItems;
}

bool KnowledgeItemsDataBase::deleteKnowledgeItemById(int id)
{
	// 定义SQL语句
	QString sql = "DELETE FROM knowledgeItems WHERE id = ?";

	// 绑定参数
	query.prepare(sql);
	query.bindValue(0, id);
	
	// 执行SQL语句
	return query.exec(sql);
}

bool KnowledgeItemsDataBase::updateKnowledgeItemById(int id, KnowledgeItem item)
{
	// 定义SQL语句
	QString sql = "UPDATE knowledgeItems SET type = ?, question = ?, answer = ?, letestReviewTime = ?, reviewCount = ? WHERE id = ?";

	// 绑定参数
	query.prepare(sql);
	query.bindValue(0, item.type);
	query.bindValue(1, item.question);
	query.bindValue(2, item.answer);
	query.bindValue(3, item.latestReviewTime);
	query.bindValue(4, item.reviewCount);
	query.bindValue(5, id);

	// 执行SQL语句
	return query.exec(sql);
}
