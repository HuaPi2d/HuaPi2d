#include "suixing.h"

SuiXing::SuiXing(QObject *parent)
	: SingleWeaponThread(parent)
{
	name = "suixing";
	times = 4;
}

SuiXing::~SuiXing()
{}

void SuiXing::run()
{
	qDebug() << name;
	// ����1��
	keyPress(0x31);
	QThread::msleep(100);
	// ����E��
	keyPress(0x45);
	QThread::msleep(100);
	// ��������Ҽ�
	RightClick();
	QThread::msleep(1500);
	keyPress(0x45);
}
