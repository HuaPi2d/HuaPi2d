#pragma once

#include <QThread>
#include "universal/script/simulate/mouseKeyboradControl.h"


class HuGuang  : public QThread
{
	Q_OBJECT

public:
	HuGuang(QObject *parent);
	~HuGuang();

public:
	QString name;
	int times;

protected:
	void run() override;
};
