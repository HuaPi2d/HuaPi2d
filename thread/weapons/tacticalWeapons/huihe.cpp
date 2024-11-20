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
	// 按下 5 键
	keyPress(0x35);
	// 等待 200ms
	QThread::msleep(200);
	// 按下 E 键
	keyPress(0x45);
	// 等待 100ms
	QThread::msleep(100);
}
