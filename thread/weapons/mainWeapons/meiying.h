#pragma once

#include <QThread>
#include "universal/script/simulate/mouseKeyboradControl.h"
#include "thread/weapons/singleweaponthread.h"

class MeiYing  : public SingleWeaponThread
{
	Q_OBJECT

public:
	MeiYing(QObject *parent);
	~MeiYing();

protected:
	void run() override;
};
