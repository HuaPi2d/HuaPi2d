#include "weaponbonusthread.h"

WeaponBonusThread::WeaponBonusThread(QObject *parent)
	: QThread(parent)
{}

WeaponBonusThread::~WeaponBonusThread()
{}

void WeaponBonusThread::run()
{
	// ���ȵȴ�1���ӣ�ȷ����ݼ�����
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

			/* �ж��Ƿ�����ƶ� */
			if (isRandomMove)
			{
				randomMove();
			}
			else 
			{
				QThread::msleep(1000);
			}

			/* ÿ 120 ��ѭ������һ����Ϣ */
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
	// ���� W A S D �Ϳռ���
	QList<BYTE> keys = { 0x57, 0x41, 0x53, 0x44, 0x00, 0x00, 0x00, 0x00 }; // W, A, S, D ���ĸ��ռ���

	// ���ѡ����������
	BYTE key1 = keys[QRandomGenerator::global()->bounded(keys.size())];
	BYTE key2 = keys[QRandomGenerator::global()->bounded(keys.size())];

	// ���ת���ӽ�
	rollGaze();

	// ���·ǿռ�
	if (key1 != 0x00) keyDown(key1);
	if (key2 != 0x00) keyDown(key2);

	// ģ�ⰴ������ʱ�� 1 ��
	QThread::msleep(1000);

	// �ͷŰ��µļ�
	if (key1 != 0x00) keyRelease(key1);
	if (key2 != 0x00) keyRelease(key2);
}

void WeaponBonusThread::sendMessage()
{
	// ����Enter��
	keyPress(VK_RETURN);

	// ����1��
	keyPress(0x31);

	// ����Enter��
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
