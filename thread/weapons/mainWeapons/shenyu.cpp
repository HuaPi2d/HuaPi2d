#include "shenyu.h"

ShenYu::ShenYu(QObject *parent)
	: SingleWeaponThread(parent)
{
	name = "ShenYu";
	times = 3;
}

ShenYu::~ShenYu()
{}

void ShenYu::run()
{
	qDebug() << name;
	// 按下 1 键
	keyPress(0x31);
	// 等待 200ms
	QThread::msleep(200);
	// 按下 E 键
	keyPress(0x45);
}
