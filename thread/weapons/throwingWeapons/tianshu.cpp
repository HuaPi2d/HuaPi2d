#include "tianshu.h"

TianShu::TianShu(QObject *parent)
	: SingleWeaponThread(parent)
{
	name = "TianShu";
	times = 10;
}

TianShu::~TianShu()
{}

void TianShu::run()
{
	qDebug() << name;
	// °´ÏÂ 4 ¼ü
	keyPress(0x34);
	// µÈ´ý 100ms
	QThread::msleep(100);
}
