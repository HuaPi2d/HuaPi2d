#include "tianshu.h"

TianShu::TianShu(QObject *parent)
	: SingleWeaponThread(parent)
{
	name = "TianShu";
	times = 10;
}

TianShu::~TianShu()
{}

void TianShu::run()
{
	qDebug() << name;
	// ���� 4 ��
	keyPress(0x34);
	// �ȴ� 100ms
	QThread::msleep(100);
}
