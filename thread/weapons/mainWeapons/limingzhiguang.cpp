#include "limingzhiguang.h"

LiMingZhiGuang::LiMingZhiGuang(QObject *parent)
	: QThread(parent)
{
	name = "LiMingZhiGuang";
	times = -1;
}

LiMingZhiGuang::~LiMingZhiGuang()
{}

void LiMingZhiGuang::run()
{
	qDebug() << name;
	// 按下 1 键
	keyPress(0x31);
	// 按下 E 键
	keyPress(0x45);
}
