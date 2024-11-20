#include "meiying.h"

MeiYing::MeiYing(QObject *parent)
	: SingleWeaponThread(parent)
{
	name = "meiying";
	times = 30;
}

MeiYing::~MeiYing()
{}

void MeiYing::run()
{
	qDebug() << name;
	// 按下 1 键
	keyPress(0x31);
	// 等待 200ms
	QThread::msleep(200);
	// 按下 E 键
	keyPress(0x45);
}
