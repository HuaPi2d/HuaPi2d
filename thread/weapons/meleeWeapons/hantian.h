#pragma once

#include <QThread>
#include "universal/script/simulate/mouseKeyboradControl.h"
#include "thread/weapons/singleweaponthread.h"

class HanTian  : public SingleWeaponThread
{
	Q_OBJECT

public:
	HanTian(QObject *parent);
	~HanTian();

protected:
	void run() override;
};
