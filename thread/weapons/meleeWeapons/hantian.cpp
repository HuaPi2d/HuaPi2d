#include "hantian.h"

HanTian::HanTian(QObject *parent)
	: SingleWeaponThread(parent)
{
	name = "HanTian";
	times = 20;
}

HanTian::~HanTian()
{}

void HanTian::run()
{
	qDebug() << name;
	// ���� 3 ��
	keyPress(0x33);
	// �ȴ� 200ms
	QThread::msleep(100);
	// ���� E ��
	keyPress(0x45);
}
