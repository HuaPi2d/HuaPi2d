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
	// ���� 1 ��
	keyPress(0x31);
	// ���� E ��
	keyPress(0x45);
}
