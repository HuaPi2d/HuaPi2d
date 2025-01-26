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
	// 按下 2 键
	keyPress(0x32);
	// 等待 200ms
	QThread::msleep(100);
	// 按下 E 键
	keyPress(0x45);
}
