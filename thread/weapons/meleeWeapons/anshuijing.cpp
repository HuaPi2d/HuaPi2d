#include "anshuijing.h"

AnShuiJing::AnShuiJing(QObject *parent)
	: SingleWeaponThread(parent)
{
	name = "AnShuiJing";
	times = 25;
}

AnShuiJing::~AnShuiJing()
{}

void AnShuiJing::run()
{
	qDebug() << name;
	// ���� 3 ��
	keyPress(0x33);
	// �ȴ� 200ms
	QThread::msleep(200);
	// ���� E ��
	keyPress(0x45);
	// �ȴ������ͷ�ʱ�� 3s
	QThread::msleep(3500);
	// �л�Ϊ������
	keyPress(0x31);
}


