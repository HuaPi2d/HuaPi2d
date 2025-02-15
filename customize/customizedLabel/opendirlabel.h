#pragma once

#include <QLabel>
#include <QMouseEvent>
#include <QDesktopServices>

class OpendirLabel  : public QLabel
{
	Q_OBJECT

public:
	OpendirLabel(QWidget *parent);
	~OpendirLabel();

protected:
	void mousePressEvent(QMouseEvent* event) override;
};
