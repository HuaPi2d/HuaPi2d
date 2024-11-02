#pragma once

#include <QObject>
#include <QThread>
#include "universal/script/simulate/mouseKeyboradControl.h"


class LiangYi  : public QThread
{
	Q_OBJECT

public:
	LiangYi(QObject *parent);
	~LiangYi();

public:
	QString name;
	int times;

protected:
	void run() override;
};
