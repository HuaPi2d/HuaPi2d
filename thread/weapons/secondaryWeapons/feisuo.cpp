#include "feisuo.h"

FeiSuo::FeiSuo(QObject *parent)
	: SingleWeaponThread(parent)
{
	name = "FeiSuo";
	times = 18;
}

FeiSuo::~FeiSuo()
{}

void FeiSuo::run()
{
	qDebug() << name;
	// ���� 2 ��
	keyPress(0x32);
	// �ȴ� 200ms
	QThread::msleep(200);
	// ���� E ��
	keyPress(0x45);
	// �ȴ� 200ms
	QThread::msleep(1000);
}
