#include "huihe.h"

HuiHe::HuiHe(QObject *parent)
	: SingleWeaponThread(parent)
{
	name = "HuiHe";
	times = 25;
}

HuiHe::~HuiHe()
{}

void HuiHe::run()
{
	qDebug() << name;
	// ���� 5 ��
	keyPress(0x35);
	// �ȴ� 200ms
	QThread::msleep(200);
	// ���� E ��
	keyPress(0x45);
	// �ȴ� 100ms
	QThread::msleep(100);
}
