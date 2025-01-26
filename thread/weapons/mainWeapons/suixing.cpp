#include "suixing.h"

SuiXing::SuiXing(QObject *parent)
	: SingleWeaponThread(parent)
{
	name = "suixing";
	times = 4;
}

SuiXing::~SuiXing()
{}

void SuiXing::run()
{
	qDebug() << name;
	// 按下1键
	keyPress(0x31);
	QThread::msleep(100);
	// 按下E键
	keyPress(0x45);
	QThread::msleep(100);
	// 按下鼠标右键
	RightClick();
	QThread::msleep(1500);
	keyPress(0x45);
}
