#include "submdwindow.h"

SubMDWindow::SubMDWindow(QWidget *parent)
	: QMdiSubWindow(parent)
{
	SubMDWidget* subMDWidget = new SubMDWidget(this);
	this->setWidget(subMDWidget);
	this->setWindowFlags(Qt::FramelessWindowHint);

	connect(subMDWidget, &SubMDWidget::widgetClosed, this, [=]() {
		emit windowDestroyed(this);
		this->deleteLater();
		});
	connect(subMDWidget, &SubMDWidget::sendStateInfo, this, &SubMDWindow::sendStateInfo);
}

SubMDWindow::~SubMDWindow()
{}
