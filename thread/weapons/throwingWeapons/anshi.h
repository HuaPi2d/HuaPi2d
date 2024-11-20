#pragma once

#include <QThread>
#include "universal/script/simulate/mouseKeyboradControl.h"
#include "thread/weapons/singleweaponthread.h"


class AnShi  : public SingleWeaponThread
{
	Q_OBJECT

public:
	AnShi(QObject *parent);
	~AnShi();

protected:
	void run() override;
};
