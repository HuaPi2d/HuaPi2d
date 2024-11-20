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
			QThread::msleep(100);
		}
	}
	else if (bonusType == 1) {
		int i = 0;
		while (true) {
			for (int j = 0; j < bonusWeaponList.size(); j++)
			{
				if (bonusWeaponList[j]->times != -1)
				{
					if (i % bonusWeaponList[j]->times == 0)
					{
						bonusWeaponList[j]->start();
						bonusWeaponList[j]->wait();
					}
				}
				
				QThread::msleep(100);
				keyPress(0x31);
			}

			/* 判断是否随机移动 */
			if (isRandomMove)
			{
				randomMove();
			}
			else 
			{
				QThread::msleep(1000);
			}

			/* 每 120 次循环发送一次消息 */
			if(i % 120 == 0 && isSendMessage)
			{
				sendMessage();
			}

			i++;
		}
	}
	
}

void WeaponBonusThread::setBonusType(int bonusType)
{
	this->bonusType = bonusType;
}

void WeaponBonusThread::setIsRandomMove(bool isRandomMove)
{
	this->isRandomMove = isRandomMove;
}

void WeaponBonusThread::setIsSendMessage(bool isSendMessage)
{
	this->isSendMessage = isSendMessage;
}

void WeaponBonusThread::randomMove()
{
	// 定义 W A S D 和空键码
	QList<BYTE> keys = { 0x57, 0x41, 0x53, 0x44, 0x00, 0x00, 0x00, 0x00 }; // W, A, S, D 和四个空键码

	// 随机选择两个键码
	BYTE key1 = keys[QRandomGenerator::global()->bounded(keys.size())];
	BYTE key2 = keys[QRandomGenerator::global()->bounded(keys.size())];

	// 随机转动视角
	rollGaze();

	// 按下非空键
	if (key1 != 0x00) keyDown(key1);
	if (key2 != 0x00) keyDown(key2);

	// 模拟按键持续时间 1 秒
	QThread::msleep(1000);

	// 释放按下的键
	if (key1 != 0x00) keyRelease(key1);
	if (key2 != 0x00) keyRelease(key2);
}

void WeaponBonusThread::sendMessage()
{
	// 按下Enter键
	keyPress(VK_RETURN);

	// 按下1键
	keyPress(0x31);

	// 按下Enter键
	keyPress(VK_RETURN);
}

void WeaponBonusThread::rollGaze()
{
	cv::Point rec = getScreenResolution();
	cv::Point pos(QRandomGenerator::global()->bounded(1, rec.x), rec.y / 2);
	MoveMouse(pos.x, pos.y);
}

int WeaponBonusThread::getBonusType()
{
	return bonusType;
}

void WeaponBonusThread::addBonusWeapon(SingleWeaponThread* bonusWeapon)
{
	bonusWeaponList.append(bonusWeapon);
}
