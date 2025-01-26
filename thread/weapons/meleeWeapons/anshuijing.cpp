#include "anshuijing.h"

AnShuiJing::AnShuiJing(QObject *parent)
	: SingleWeaponThread(parent)
{
	name = "AnShuiJing";
	times = 25;
}

AnShuiJing::~AnShuiJing()
{}

void AnShuiJing::run()
{
	qDebug() << name;
	// 按下 3 键
	keyPress(0x33);
	// 等待 200ms
	QThread::msleep(200);
	// 按下 E 键
	keyPress(0x45);
	// 等待技能释放时间 3s
	QThread::msleep(3500);
	// 切换为主武器
	keyPress(0x31);
}


