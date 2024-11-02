#include "anshi.h"

AnShi::AnShi(QObject *parent)
	: QThread(parent)
{
	name = "AnShi";
	times = -1;
}

AnShi::~AnShi()
{}

void AnShi::run()
{
	qDebug() << name;
	// 按下 4 键
	keyPress(0x34);
	// 等待扔完
	QThread::msleep(1800);
	// 松开鼠标左键
	LeftUp();
	LeftDown();
}
