#pragma once

#include <QObject>
#include <QThread>
#include "universal/script/simulate/mouseKeyboradControl.h"
#include "thread/weapons/singleweaponthread.h"

class AnShuiJing  : public SingleWeaponThread
{
	Q_OBJECT

public:
	AnShuiJing(QObject *parent);
	~AnShuiJing();

protected:
	void run() override;
};
