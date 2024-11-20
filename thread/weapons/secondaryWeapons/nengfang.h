#pragma once

#include <QObject>
#include <QThread>
#include "universal/script/simulate/mouseKeyboradControl.h"
#include "universal/script/image/snapShoot.h"
#include "thread/weapons/singleweaponthread.h"

class NengFang  : public SingleWeaponThread
{
	Q_OBJECT

public:
	NengFang(QObject *parent);
	~NengFang();

protected:
	void run() override;
};
