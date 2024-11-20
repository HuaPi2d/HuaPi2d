#pragma once

#include <QObject>
#include <QThread>
#include "universal/script/simulate/mouseKeyboradControl.h"
#include "thread/weapons/singleweaponthread.h"

class HunDunBaoFa  : public SingleWeaponThread
{
	Q_OBJECT

public:
	HunDunBaoFa(QObject *parent);
	~HunDunBaoFa();

protected:
	void run() override;
};
