#pragma once

#include <QThread>
#include "universal/script/simulate/mouseKeyboradControl.h"
#include "thread/weapons/singleweaponthread.h"

class FengShen  : public SingleWeaponThread
{
	Q_OBJECT

public:
	FengShen(QObject *parent);
	~FengShen();

protected:
	void run() override;
};
