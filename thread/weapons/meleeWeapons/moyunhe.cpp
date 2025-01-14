#include "moyunhe.h"

MoYunHe::MoYunHe(QObject *parent)
	: SingleWeaponThread(parent)
{
	name = "MoYunHe";
	times = 15;
}

MoYunHe::~MoYunHe()
{}

void MoYunHe::run()
{
	qDebug() << name;
	// 按下3键
	keyPress(0x33);
	// 按下E键两次
	msleep(100);
	keyPress(0x45);
	msleep(100);
	keyPress(0x45);
	// 等待1000ms
	msleep(1000);
}
