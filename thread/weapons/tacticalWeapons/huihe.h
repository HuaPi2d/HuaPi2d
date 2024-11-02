#pragma once

#include <QObject>
#include <QThread>
#include "universal/script/simulate/mouseKeyboradControl.h"


class HuiHe  : public QThread
{
	Q_OBJECT

public:
	HuiHe(QObject *parent);
	~HuiHe();

public:
	QString name;
	int times;

protected:
	void run() override;
};
