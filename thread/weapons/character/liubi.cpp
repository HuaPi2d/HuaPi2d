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
	// �ȴ� 200ms
	QThread::msleep(200);
	// ���� X ��
	keyPress(0x58);
}
