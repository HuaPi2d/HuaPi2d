#include "hundunbaofa.h"

HunDunBaoFa::HunDunBaoFa(QObject *parent)
	: SingleWeaponThread(parent)
{
	name = "HunDunBaoFa";
	times = 60;
}

HunDunBaoFa::~HunDunBaoFa()
{}

void HunDunBaoFa::run()
{
	qDebug() << name;
	// ����ɿ�
	LeftUp();
	// ���� C ��
	keyPress(0x43);
	// �������
	LeftDown();
}
