#pragma once

#include <QObject>
#include <QThread>
#include "universal/script/simulate/mouseKeyboradControl.h"
#include "thread/weapons/singleweaponthread.h"


class TaiKeSiX  : public SingleWeaponThread
{
	Q_OBJECT

public:
	TaiKeSiX(QObject *parent);
	~TaiKeSiX();


protected:
	void run() override;
};
