#pragma once

#include <QObject>
#include <QThread>
#include "universal/script/simulate/mouseKeyboradControl.h"
#include "thread/weapons/singleweaponthread.h"

class LiangYi  : public SingleWeaponThread
{
	Q_OBJECT

public:
	LiangYi(QObject *parent);
	~LiangYi();

protected:
	void run() override;
};
