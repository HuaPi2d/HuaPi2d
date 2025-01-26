#pragma once

#include <QThread>
#include "universal/script/simulate/mouseKeyboradControl.h"
#include "universal/script/image/snapShoot.h"
#include "thread/weapons/singleweaponthread.h"

class LieXi  : public SingleWeaponThread
{
	Q_OBJECT

public:
	LieXi(QObject *parent);
	~LieXi();

protected:
	void run() override;
};
