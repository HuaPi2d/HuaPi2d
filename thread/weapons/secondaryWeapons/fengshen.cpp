#include "fengshen.h"

FengShen::FengShen(QObject *parent)
	: SingleWeaponThread(parent)
{
	name = "FengShen";
	times = 33;
}

FengShen::~FengShen()
{}

void FengShen::run()
{
	qDebug() << name;
	// ���� 2 ��
	keyPress(0x32);
	// �ȴ� 200ms
	QThread::msleep(100);
	// ���� E ��
	keyPress(0x45);
}
