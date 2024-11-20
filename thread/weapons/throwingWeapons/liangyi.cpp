#include "liangyi.h"

LiangYi::LiangYi(QObject *parent)
	: SingleWeaponThread(parent)
{
	name = "LiangYi";
	times = 45;
}

LiangYi::~LiangYi()
{}

void LiangYi::run()
{
	qDebug() << name;
	// 按下 4 键
	keyPress(0x34);
	// 等待 200ms
	QThread::msleep(200);
	// 按下 E 键
	keyPress(0x45);
	// 等待 200ms
	QThread::msleep(100);
	// 切换为主武器
	keyPress(0x31);
}
