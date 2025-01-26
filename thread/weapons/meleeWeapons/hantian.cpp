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
	// 按下 3 键
	keyPress(0x33);
	// 等待 200ms
	QThread::msleep(100);
	// 按下 E 键
	keyPress(0x45);
}
