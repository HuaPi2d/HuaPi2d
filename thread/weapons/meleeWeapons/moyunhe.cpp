#include "moyunhe.h"

MoYunHe::MoYunHe(QObject *parent)
	: SingleWeaponThread(parent)
{
	name = "MoYunHe";
	times = 15;
}

MoYunHe::~MoYunHe()
{}

void MoYunHe::run()
{
	qDebug() << name;
	// ����3��
	keyPress(0x33);
	// ����E������
	msleep(100);
	keyPress(0x45);
	msleep(100);
	keyPress(0x45);
	// �ȴ�1000ms
	msleep(1000);
}
