#include "meiying.h"

MeiYing::MeiYing(QObject *parent)
	: SingleWeaponThread(parent)
{
	name = "meiying";
	times = 30;
}

MeiYing::~MeiYing()
{}

void MeiYing::run()
{
	qDebug() << name;
	// ���� 1 ��
	keyPress(0x31);
	// �ȴ� 200ms
	QThread::msleep(200);
	// ���� E ��
	keyPress(0x45);
}
