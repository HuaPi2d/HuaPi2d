#include "shenyu.h"

ShenYu::ShenYu(QObject *parent)
	: SingleWeaponThread(parent)
{
	name = "ShenYu";
	times = 3;
}

ShenYu::~ShenYu()
{}

void ShenYu::run()
{
	qDebug() << name;
	// ���� 1 ��
	keyPress(0x31);
	// �ȴ� 200ms
	QThread::msleep(200);
	// ���� E ��
	keyPress(0x45);
}
