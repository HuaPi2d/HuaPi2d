#pragma once

#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include <QList>

typedef struct OneKnowledgeItem
{
	int id;
	QString type;
	QString question;
	QString answer;
	QString latestReviewTime;
	QString reviewCount;
} KnowledgeItem;

class KnowledgeItemsDataBase : public QSqlDatabase
{
public:
	KnowledgeItemsDataBase(QString dbType = "QSQLITE");
	~KnowledgeItemsDataBase();

	static KnowledgeItemsDataBase* getKnowledgeItemsDataBase(QString dbname = "knowledgeItems.db");

	// 创建表
	bool createKnowledgeItemsTable();

	// 插入数据
	bool insertKnowledgeItem(KnowledgeItem item);

	// 按照顺序查询数据
	// bool getKnowledgeItemByIndex(int index, KnowledgeItem& item);

	// 按照类型查询数据
	QList<KnowledgeItem> getKnowledgeItems(QString type = "");

	// 通过 ID 删除数据
	bool deleteKnowledgeItemById(int id);

	// 通过 ID 更新数据
	bool updateKnowledgeItemById(int id, KnowledgeItem item);

private:
	QSqlQuery query;
};
