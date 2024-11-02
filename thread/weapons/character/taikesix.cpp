#include "taikesix.h"

TaiKeSiX::TaiKeSiX(QObject *parent)
	: QThread(parent)
{
	name = "TaiKeSiX";
	times = 45;
}

TaiKeSiX::~TaiKeSiX()
{}

void TaiKeSiX::run()
{
	qDebug() << name;
	// 按下 X 键
	keyPress(0x58);
	// 等待技能释放时间 3s
	QThread::msleep(3000);
}
