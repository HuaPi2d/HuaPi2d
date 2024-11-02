#include "liubi.h"

LiuBi::LiuBi(QObject *parent)
	: QThread(parent)
{
	name = "LiuBi";
	times = 30;
}

LiuBi::~LiuBi()
{}

void LiuBi::run()
{
	qDebug() << name;
	// µÈ´ý 200ms
	QThread::msleep(200);
	// °´ÏÂ X ¼ü
	keyPress(0x58);
}
