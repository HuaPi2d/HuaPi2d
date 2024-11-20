#pragma once

#include <QObject>
#include <QThread>
#include <QRandomGenerator>
#include "universal/script/simulate/mouseKeyboradControl.h"
#include "thread/weapons/singleweaponthread.h"
#include "universal/script/image/snapShoot.h"

class WeaponBonusThread  : public QThread
{
	Q_OBJECT

public:
	WeaponBonusThread(QObject *parent);
	~WeaponBonusThread();

private:
	QVector<SingleWeaponThread*> bonusWeaponList;
	int bonusType;         // 0: ���μӳ� 1: �����ӳ�
	bool isRandomMove;       // �Ƿ�����ƶ����
	bool isSendMessage;      // �Ƿ�������Ϣ

protected:
	void run() override;

public slots:
	void addBonusWeapon(SingleWeaponThread* bonusWeapon);
	void setBonusType(int bonusType);
	void setIsRandomMove(bool isRandomMove);
	void setIsSendMessage(bool isSendMessage);
	void randomMove();
	void sendMessage();
	// ת������
	void rollGaze();
	// ��ȡ��ǰ�ļӳ�����
	int getBonusType();
};
