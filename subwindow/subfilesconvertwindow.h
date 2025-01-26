#pragma once

#include <QObject>
#include <QMdiSubWindow>
#include <QWidget>

#include "subwidgets/subfilesconvertwidget.h"

class SubFilesConvertWindow  : public QMdiSubWindow
{
	Q_OBJECT

public:
	SubFilesConvertWindow(QWidget *parent);
	~SubFilesConvertWindow();

public: signals:
	void sendStateInfo(QString state);
	void windowDestroyed(QMdiSubWindow* window);
};
