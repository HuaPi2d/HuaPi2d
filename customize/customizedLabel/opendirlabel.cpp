#include "opendirlabel.h"

OpendirLabel::OpendirLabel(QWidget *parent)
	: QLabel(parent)
{
	
}

OpendirLabel::~OpendirLabel()
{}

void OpendirLabel::mousePressEvent(QMouseEvent * event)
{
	if (event->button() == Qt::LeftButton) {
		QDesktopServices::openUrl(QUrl::fromLocalFile(this->text()));
	}
}
