#pragma once

#include <QObject>
#include <QThread>
#include "universal/script/simulate/mouseKeyboradControl.h"


class TaiKeSiX  : public QThread
{
	Q_OBJECT

public:
	TaiKeSiX(QObject *parent);
	~TaiKeSiX();

public:
	QString name;
	int times;

protected:
	void run() override;
};
