#include "weaponbonusthread.h"

WeaponBonusThread::WeaponBonusThread(QObject *parent)
	: QThread(parent)
{}

WeaponBonusThread::~WeaponBonusThread()
{}

void WeaponBonusThread::run()
{
	// 首先等待1秒钟，确保快捷键弹开
	QThread::msleep(1000);
	LeftDown();
	if (bonusType == 0) {
		for (int i = 0; i < bonusWeaponList.size(); i++)
		{
			bonusWeaponList[i]->start();
			bonusWeaponList[i]->wait();
			QThread::msleep(500);
		}
	}
	
}

void WeaponBonusThread::setBonusType(int bonusType)
{
	this->bonusType = bonusType;
}

void WeaponBonusThread::addBonusWeapon(QThread * bonusWeapon)
{
	bonusWeaponList.append(bonusWeapon);
}
