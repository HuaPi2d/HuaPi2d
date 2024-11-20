#include "caijue.h"

CaiJue::CaiJue(QObject *parent)
	: SingleWeaponThread(parent)
{
	name = "CaiJue";
	times = 30;
}

CaiJue::~CaiJue()
{}

void CaiJue::run()
{
	qDebug() << name;
	// 按下 1 键
	keyPress(0x31);
	// 等待 100ms
	QThread::msleep(100);
	// 按下 E 键
	keyPress(0x45);
}
