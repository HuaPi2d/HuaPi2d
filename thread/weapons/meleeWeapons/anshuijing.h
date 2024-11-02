#pragma once

#include <QObject>
#include <QThread>
#include "universal/script/simulate/mouseKeyboradControl.h"

class AnShuiJing  : public QThread
{
	Q_OBJECT

public:
	AnShuiJing(QObject *parent);
	~AnShuiJing();

public:
	QString name;
	int times;

protected:
	void run() override;
};
