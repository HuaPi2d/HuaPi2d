#pragma once

#include <QObject>
#include <QThread>
#include "universal/script/simulate/mouseKeyboradControl.h"

class LiuBi  : public QThread
{
	Q_OBJECT

public:
	LiuBi(QObject *parent);
	~LiuBi();

public:
	QString name;
	int times;

protected:
	void run() override;
};
