#include "liangyi.h"

LiangYi::LiangYi(QObject *parent)
	: SingleWeaponThread(parent)
{
	name = "LiangYi";
	times = 45;
}

LiangYi::~LiangYi()
{}

void LiangYi::run()
{
	qDebug() << name;
	// ���� 4 ��
	keyPress(0x34);
	// �ȴ� 200ms
	QThread::msleep(200);
	// ���� E ��
	keyPress(0x45);
	// �ȴ� 200ms
	QThread::msleep(100);
	// �л�Ϊ������
	keyPress(0x31);
}
