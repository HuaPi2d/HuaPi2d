#pragma once

#include <QThread>
#include "universal/script/simulate/mouseKeyboradControl.h"
#include "thread/weapons/singleweaponthread.h"

class SuiXing  : public SingleWeaponThread
{
	Q_OBJECT

public:
	SuiXing(QObject *parent);
	~SuiXing();

protected:
	void run() override;
};
