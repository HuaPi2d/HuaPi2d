#include "tianshu.h"

TianShu::TianShu(QObject *parent)
	: SingleWeaponThread(parent)
{
	name = "TianShu";
	times = 14;
}

TianShu::~TianShu()
{}

void TianShu::run()
{
	qDebug() << name;
	// ���� 4 ��
	// keyPress(0x34);
	// ���� F ��
	keyPress(0x46);
	// �ȴ� 100ms
	// QThread::msleep(100);
}
