#include "subaawindow.h"


SubAAWindow::SubAAWindow(QWidget *parent)
	: QMdiSubWindow{ parent }
{
    SubAAWidget* subAAWidget = new SubAAWidget(this);
    this->setWidget(subAAWidget);
    this->setWindowFlags(Qt::FramelessWindowHint);

    connect(subAAWidget, &SubAAWidget::widgetClosed, this, [=]() {
        emit windowDestroyed(this);
        this->deleteLater();
    });
    connect(subAAWidget, &SubAAWidget::sendStateInfo, this, &SubAAWindow::sendStateInfo);
}
