#ifndef SHOWTEXTINSCREENWIDGET_H
#define SHOWTEXTINSCREENWIDGET_H

#include <QWidget>
#include <QLabel>
#include <QThread>
#include <QTimer>
#include "global/mainglobalvar.h"

class ShowTextInScreenWidget  : public QWidget
{
	Q_OBJECT

public:
	ShowTextInScreenWidget(QWidget* parent, QString text, QPoint position, int durationMs);
	~ShowTextInScreenWidget();

	static void showText(QWidget* parent, const QString& text, const QPoint& position, int durationMs);

private:
	QString text_;
	QPoint position_;
	int durationMs_;
	QLabel* label;

    void showTextInScreen();
};

#endif // SHOWTEXTINSCREENWIDGET_H
