#pragma once

#include <QObject>
#include <QThread>
#include "universal/script/simulate/mouseKeyboradControl.h"
#include "universal/script/image/snapShoot.h"

class NengFang  : public QThread
{
	Q_OBJECT

public:
	NengFang(QObject *parent);
	~NengFang();

public:
	QString name;
	int times;

protected:
	void run() override;
};
