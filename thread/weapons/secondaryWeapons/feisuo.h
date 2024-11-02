#pragma once

#include <QObject>
#include <QThread>
#include "universal/script/simulate/mouseKeyboradControl.h"

class FeiSuo  : public QThread
{
	Q_OBJECT

public:
	FeiSuo(QObject *parent);
	~FeiSuo();

public:
	QString name;
	int times;

protected:
	void run() override;
};
