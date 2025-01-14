#pragma once

#include <QThread>
#include "universal/script/simulate/mouseKeyboradControl.h"
#include "thread/weapons/singleweaponthread.h"

class MoYunHe  : public SingleWeaponThread
{
	Q_OBJECT

public:
	MoYunHe(QObject *parent);
	~MoYunHe();

protected:
	void run() override;
};
