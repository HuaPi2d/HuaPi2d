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

	// ������
	bool createKnowledgeItemsTable();

	// ��������
	bool insertKnowledgeItem(KnowledgeItem item);

	// ����˳���ѯ����
	// bool getKnowledgeItemByIndex(int index, KnowledgeItem& item);

	// �������Ͳ�ѯ����
	QList<KnowledgeItem> getKnowledgeItems(QString type = "");

	// ͨ�� ID ɾ������
	bool deleteKnowledgeItemById(int id);

	// ͨ�� ID ��������
	bool updateKnowledgeItemById(int id, KnowledgeItem item);

private:
	QSqlQuery query;
};
