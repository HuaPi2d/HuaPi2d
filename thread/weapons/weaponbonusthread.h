#pragma once

#include <QObject>
#include <QThread>
#include "universal/script/simulate/mouseKeyboradControl.h"

class WeaponBonusThread  : public QThread
{
	Q_OBJECT

public:
	WeaponBonusThread(QObject *parent);
	~WeaponBonusThread();

private:
	QVector<QThread*> bonusWeaponList;
	int bonusType;         // 0: ���μӳ� 1: �����ӳ�

protected:
	void run() override;

public slots:
	void addBonusWeapon(QThread* bonusWeapon);
	void setBonusType(int bonusType);
};
