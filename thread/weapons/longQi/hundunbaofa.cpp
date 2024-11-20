#include "hundunbaofa.h"

HunDunBaoFa::HunDunBaoFa(QObject *parent)
	: SingleWeaponThread(parent)
{
	name = "HunDunBaoFa";
	times = 60;
}

HunDunBaoFa::~HunDunBaoFa()
{}

void HunDunBaoFa::run()
{
	qDebug() << name;
	// 左键松开
	LeftUp();
	// 按下 C 键
	keyPress(0x43);
	// 按下左键
	LeftDown();
}
