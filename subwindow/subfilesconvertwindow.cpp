#include "subfilesconvertwindow.h"

SubFilesConvertWindow::SubFilesConvertWindow(QWidget *parent)
	: QMdiSubWindow(parent)
{
    SubFilesConvertWidget* subFilesConvertWidget = new SubFilesConvertWidget(this);
    this->setWidget(subFilesConvertWidget);
    this->setWindowFlags(Qt::FramelessWindowHint);

    connect(subFilesConvertWidget, &SubFilesConvertWidget::widgetClosed, this, [=]() {
        emit windowDestroyed(this);
        this->deleteLater();
        });
    connect(subFilesConvertWidget, &SubFilesConvertWidget::sendStateInfo, this, &SubFilesConvertWindow::sendStateInfo);
}

SubFilesConvertWindow::~SubFilesConvertWindow()
{}
