#pragma once

#include <QThread>
#include <QThread>
#include "universal/script/simulate/mouseKeyboradControl.h"

class LiMingZhiGuang  : public QThread
{
	Q_OBJECT

public:
	LiMingZhiGuang(QObject *parent);
	~LiMingZhiGuang();

public:
	QString name;
	int times;

protected:
	void run() override;
};
