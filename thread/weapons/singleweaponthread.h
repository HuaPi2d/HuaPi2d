#pragma once

#include <QThread>

class SingleWeaponThread  : public QThread
{
	Q_OBJECT

public:
	SingleWeaponThread(QObject *parent);
	~SingleWeaponThread();

public:
	QString name;
	int times;
};
