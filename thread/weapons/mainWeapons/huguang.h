#pragma once

#include <QThread>
#include "universal/script/simulate/mouseKeyboradControl.h"
#include "thread/weapons/singleweaponthread.h"

class HuGuang  : public SingleWeaponThread
{
	Q_OBJECT

public:
	HuGuang(QObject *parent);
	~HuGuang();

protected:
	void run() override;
};
