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
	
	// ��дmouseEvent������ʵ�ֳ����ӵ���ת����
protected:
	void mousePressEvent(QMouseEvent* event) override;


};
