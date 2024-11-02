#pragma once

#include <QObject>
#include <QThread>
#include "universal/script/simulate/mouseKeyboradControl.h"

class HunDunBaoFa  : public QThread
{
	Q_OBJECT

public:
	HunDunBaoFa(QObject *parent);
	~HunDunBaoFa();

public:
	QString name;
	int times;

protected:
	void run() override;
};
