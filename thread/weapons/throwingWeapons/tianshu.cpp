#include "tianshu.h"

TianShu::TianShu(QObject *parent)
	: SingleWeaponThread(parent)
{
	name = "TianShu";
	times = 14;
}

TianShu::~TianShu()
{}

void TianShu::run()
{
	qDebug() << name;
	// 按下 4 键
	// keyPress(0x34);
	// 按下 F 键
	keyPress(0x46);
	// 等待 100ms
	// QThread::msleep(100);
}
