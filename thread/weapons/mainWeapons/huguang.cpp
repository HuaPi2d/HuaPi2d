#include "huguang.h"

HuGuang::HuGuang(QObject *parent)
	: SingleWeaponThread(parent)
{
	name = "HuGuang";
	times = 3;
}

HuGuang::~HuGuang()
{}

void HuGuang::run()
{
	qDebug() << name;
	// ���� 1 ��
	keyPress(0x31);
	// �ȴ� 200ms
	QThread::msleep(200);
	// ���� E ��
	keyPress(0x45);
}
