#include "urllabel.h"

UrlLabel::UrlLabel(QWidget *parent)
	: QLabel(parent)
{
	url = "";
}

UrlLabel::~UrlLabel()
{}

void UrlLabel::setUrl(const QString& url)
{
	this->url = url;
}

void UrlLabel::mousePressEvent(QMouseEvent * event)
{
	if (url == "" || url.isEmpty()) {
		if (event->button() == Qt::LeftButton)
		{
			QDesktopServices::openUrl(QUrl(text()));
		}
	}
	else {
		QDesktopServices::openUrl(QUrl(url));
	}
}
