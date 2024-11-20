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
	int bonusType;         // 0: 单次加成 1: 持续加成
	bool isRandomMove;       // 是否随机移动鼠标
	bool isSendMessage;      // 是否输入消息

protected:
	void run() override;

public slots:
	void addBonusWeapon(SingleWeaponThread* bonusWeapon);
	void setBonusType(int bonusType);
	void setIsRandomMove(bool isRandomMove);
	void setIsSendMessage(bool isSendMessage);
	void randomMove();
	void sendMessage();
	// 转动视线
	void rollGaze();
	// 获取当前的加成类型
	int getBonusType();
};
