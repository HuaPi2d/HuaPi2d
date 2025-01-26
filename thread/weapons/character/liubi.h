#pragma once

#include <QObject>
#include <QThread>
#include "universal/script/simulate/mouseKeyboradControl.h"
#include "thread/weapons/singleweaponthread.h"

class LiuBi  : public SingleWeaponThread
{
	Q_OBJECT

public:
	LiuBi(QObject *parent);
	~LiuBi();

protected:
	void run() override;
};
