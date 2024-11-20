#pragma once

#include <QObject>
#include <QThread>
#include "universal/script/simulate/mouseKeyboradControl.h"
#include "thread/weapons/singleweaponthread.h"


class HuiHe  : public SingleWeaponThread
{
	Q_OBJECT

public:
	HuiHe(QObject *parent);
	~HuiHe();

protected:
	void run() override;
};
