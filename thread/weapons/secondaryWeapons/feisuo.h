#pragma once

#include <QObject>
#include <QThread>
#include "universal/script/simulate/mouseKeyboradControl.h"
#include "thread/weapons/singleweaponthread.h"

class FeiSuo  : public SingleWeaponThread
{
	Q_OBJECT

public:
	FeiSuo(QObject *parent);
	~FeiSuo();

public:
	QString name;
	int times;

protected:
	void run() override;
};
