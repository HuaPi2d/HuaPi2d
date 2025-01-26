#pragma once

#include <QThread>
#include "universal/script/simulate/mouseKeyboradControl.h"
#include "thread/weapons/singleweaponthread.h"


class TianShu  : public SingleWeaponThread
{
	Q_OBJECT

public:
	TianShu(QObject *parent);
	~TianShu();

protected:
	void run() override;
};
