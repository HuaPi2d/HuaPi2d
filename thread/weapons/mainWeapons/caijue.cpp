#include "caijue.h"

CaiJue::CaiJue(QObject *parent)
	: SingleWeaponThread(parent)
{
	name = "CaiJue";
	times = 30;
}

CaiJue::~CaiJue()
{}

void CaiJue::run()
{
	qDebug() << name;
	// ���� 1 ��
	keyPress(0x31);
	// �ȴ� 100ms
	QThread::msleep(100);
	// ���� E ��
	keyPress(0x45);
}
