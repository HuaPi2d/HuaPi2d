#pragma once

#include <QThread>
#include <QThread>
#include "universal/script/simulate/mouseKeyboradControl.h"
#include "thread/weapons/singleweaponthread.h"

class LiMingZhiGuang  : public SingleWeaponThread
{
	Q_OBJECT

public:
	LiMingZhiGuang(QObject *parent);
	~LiMingZhiGuang();

protected:
	void run() override;
};
