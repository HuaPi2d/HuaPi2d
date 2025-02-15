#pragma once

#include <QLabel>
#include <QMouseEvent>
#include <QDesktopServices>


class UrlLabel  : public QLabel
{
	Q_OBJECT

public:
	UrlLabel(QWidget* parent);
	~UrlLabel();

	QString url;

public slots:
	void setUrl(const QString& url);
	
	// 重写mouseEvent方法，实现超链接的跳转功能
protected:
	void mousePressEvent(QMouseEvent* event) override;


};
