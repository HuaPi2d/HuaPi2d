#pragma once

#include <QThread>
#include "universal/script/simulate/mouseKeyboradControl.h"
#include "thread/weapons/singleweaponthread.h"

class LunHua  : public SingleWeaponThread
{
	Q_OBJECT

public:
	LunHua(QObject *parent);
	~LunHua();

protected:
	void run() override;
};
