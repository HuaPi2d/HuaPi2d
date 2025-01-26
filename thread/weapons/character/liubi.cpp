#include "liubi.h"

LiuBi::LiuBi(QObject *parent)
	: SingleWeaponThread(parent)
{
	name = "LiuBi";
	times = 30;
}

LiuBi::~LiuBi()
{}

void LiuBi::run()
{
	qDebug() << name;
	// 等待 200ms
	QThread::msleep(200);
	// 按下 X 键
	keyPress(0x58);
}
