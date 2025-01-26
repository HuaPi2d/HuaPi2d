#pragma once

#include <QThread>
#include "universal/script/simulate/mouseKeyboradControl.h"
#include "thread/weapons/singleweaponthread.h"

class CaiJue  : public SingleWeaponThread
{
	Q_OBJECT

public:
	CaiJue(QObject *parent);
	~CaiJue();

protected:
	void run() override;
};
