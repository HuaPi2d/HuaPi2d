#include "anshi.h"

AnShi::AnShi(QObject *parent)
	: QThread(parent)
{
	name = "AnShi";
	times = -1;
}

AnShi::~AnShi()
{}

void AnShi::run()
{
	qDebug() << name;
	// ���� 4 ��
	keyPress(0x34);
	// �ȴ�����
	QThread::msleep(1800);
	// �ɿ�������
	LeftUp();
	LeftDown();
}
