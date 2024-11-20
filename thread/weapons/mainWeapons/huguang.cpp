#include "huguang.h"

HuGuang::HuGuang(QObject *parent)
	: SingleWeaponThread(parent)
{
	name = "HuGuang";
	times = 5;
}

HuGuang::~HuGuang()
{}

void HuGuang::run()
{
	qDebug() << name;
	// 按下 1 键
	keyPress(0x31);
	// 等待 200ms
	QThread::msleep(200);
	// 按下 E 键
	keyPress(0x45);
}
