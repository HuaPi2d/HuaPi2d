#include "feisuo.h"

FeiSuo::FeiSuo(QObject *parent)
	: SingleWeaponThread(parent)
{
	name = "FeiSuo";
	times = 18;
}

FeiSuo::~FeiSuo()
{}

void FeiSuo::run()
{
	qDebug() << name;
	// 按下 2 键
	keyPress(0x32);
	// 等待 200ms
	QThread::msleep(200);
	// 按下 E 键
	keyPress(0x45);
	// 等待 200ms
	QThread::msleep(1000);
}
