#include "lunhua.h"

LunHua::LunHua(QObject *parent)
	: SingleWeaponThread(parent)
{
	name = "LunHua";
	times = 1;
}

LunHua::~LunHua()
{}

void LunHua::run()
{
	qDebug() << name;
	// �ϻ�����
	ScrollMouse(2);
}
