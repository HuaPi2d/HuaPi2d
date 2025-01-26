#pragma once

#include <QObject>
#include <QThread>
#include "universal/script/simulate/mouseKeyboradControl.h"
#include "thread/weapons/singleweaponthread.h"

class ShenYu  : public SingleWeaponThread
{
	Q_OBJECT

public:
	ShenYu(QObject *parent);
	~ShenYu();

protected:
	void run() override;
};
