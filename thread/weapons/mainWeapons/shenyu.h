#pragma once

#include <QObject>
#include <QThread>
#include "universal/script/simulate/mouseKeyboradControl.h"

class ShenYu  : public QThread
{
	Q_OBJECT

public:
	ShenYu(QObject *parent);
	~ShenYu();

public:
	QString name;
	int times;

protected:
	void run() override;
};
