#pragma once

#include <QMdiSubWindow>

#include "subwidgets/submdwidget.h"

class SubMDWindow  : public QMdiSubWindow
{
	Q_OBJECT

public:
	SubMDWindow(QWidget *parent);
	~SubMDWindow();

public: signals:
	void sendStateInfo(QString state);
	void windowDestroyed(QMdiSubWindow* window);
};
