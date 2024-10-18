#pragma once

#include <QMdiSubWindow>
#include "subwidgets/subaawidget.h"

class SubAAWindow  : public QMdiSubWindow
{
	Q_OBJECT

public:
	explicit SubAAWindow(QWidget* parent = nullptr);

public: signals:
	void sendStateInfo(QString state);
	void windowDestroyed(QMdiSubWindow* window);
};
