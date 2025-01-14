#include "knowledgeitemsdatabase.h"

KnowledgeItemsDataBase::KnowledgeItemsDataBase(QString dbType)
	: QSqlDatabase(QSqlDatabase::addDatabase(dbType))
{}

KnowledgeItemsDataBase::~KnowledgeItemsDataBase()
{}

// ��̬���������ڴ���KnowledgeItemsDataBase����
KnowledgeItemsDataBase* KnowledgeItemsDataBase::getKnowledgeItemsDataBase(QString dbname)
{
	// ����һ��KnowledgeItemsDataBase����
	KnowledgeItemsDataBase* knowledgeItemsDataBase = new KnowledgeItemsDataBase();
	// �������ݿ�����
	knowledgeItemsDataBase->setDatabaseName(dbname);
	// ����KnowledgeItemsDataBase����
	return knowledgeItemsDataBase;
}

// ����knowledgeItems��
bool KnowledgeItemsDataBase::createKnowledgeItemsTable()
{
	// ����SQL���
	// ����ΪknowledgeItems
	// �ֶΰ������͡����⡢�𰸡�����ʱ�䡢����ʱ��
	QString sql = "CREATE TABLE IF NOT EXISTS knowledgeItems("
		"id INTEGER PRIMARY KEY AUTOINCREMENT,"
		"type varchar(20) NOT NULL,"
		"question varchar(200) NOT NULL,"
		"answer varchar(200) NOT NULL,"
		"letestReviewTime varchar(20) NOT NULL,"
		"reviewCount varchar(20) NOT NULL"
		");";

	// ִ��SQL���
	return query.exec(sql);
}

// ����һ��֪ʶ��Ŀ
bool KnowledgeItemsDataBase::insertKnowledgeItem(KnowledgeItem item)
{
	// ����SQL���
	// ����ΪknowledgeItems
	// �ֶΰ������͡����⡢�𰸡�����ʱ�䡢����ʱ��
	QString sql = "INSERT INTO knowledgeItems(type,question,answer,letestReviewTime,reviewCount) VALUES(?,?,?,?,?)";

	// �󶨲���
	query.prepare(sql);
	query.bindValue(0, item.type);
	query.bindValue(1, item.question);
	query.bindValue(2, item.answer);
	query.bindValue(3, item.latestReviewTime);
	query.bindValue(4, item.reviewCount);

	// ִ��SQL���
	return query.exec();
}

QList<KnowledgeItem> KnowledgeItemsDataBase::getKnowledgeItems(QString type)
{
	// ���巵�ض���
	QList<KnowledgeItem> knowledgeItems;

	// ����SQL���
	QString sql;
	if (type == "")
	{
		sql = "SELECT * FROM knowledgeItems";
	}

	// ִ��SQL���
	if (!query.exec(sql))
	{
		qDebug() << "SQL��ѯ����ִ��ʧ�ܣ�" << query.lastError().text();
	}
	else
	{
		// ѭ����ȡ�����
		while (query.next())
		{
			// ��ȡһ������
			KnowledgeItem item;
			item.id = query.value("id").toInt();
			item.type = query.value("type").toString();
			item.question = query.value("question").toString();
			item.answer = query.value("answer").toString();
			item.latestReviewTime = query.value("letestReviewTime").toString();
			item.reviewCount = query.value("reviewCount").toString();
			// ��������ӵ����ض�����
			knowledgeItems.append(item);
		}
	}
	
	return knowledgeItems;
}

bool KnowledgeItemsDataBase::deleteKnowledgeItemById(int id)
{
	// ����SQL���
	QString sql = "DELETE FROM knowledgeItems WHERE id = ?";

	// �󶨲���
	query.prepare(sql);
	query.bindValue(0, id);
	
	// ִ��SQL���
	return query.exec(sql);
}

bool KnowledgeItemsDataBase::updateKnowledgeItemById(int id, KnowledgeItem item)
{
	// ����SQL���
	QString sql = "UPDATE knowledgeItems SET type = ?, question = ?, answer = ?, letestReviewTime = ?, reviewCount = ? WHERE id = ?";

	// �󶨲���
	query.prepare(sql);
	query.bindValue(0, item.type);
	query.bindValue(1, item.question);
	query.bindValue(2, item.answer);
	query.bindValue(3, item.latestReviewTime);
	query.bindValue(4, item.reviewCount);
	query.bindValue(5, id);

	// ִ��SQL���
	return query.exec(sql);
}
