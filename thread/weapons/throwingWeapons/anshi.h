#pragma once

#include <QThread>
#include "universal/script/simulate/mouseKeyboradControl.h"

class AnShi  : public QThread
{
	Q_OBJECT

public:
	AnShi(QObject *parent);
	~AnShi();

public:
	QString name;
	int times;

protected:
	void run() override;
};
