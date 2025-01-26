#pragma once

#include <QObject>
#include <QWidget>
#include <QLabel>
#include <QColorDialog>
#include <QPushButton>
#include <QTimer>
#include <QGridLayout>
#include <QColor>
#include <windows.h>


class ColorPicker  : public QWidget
{
	Q_OBJECT

public:
	ColorPicker(QWidget *parent);
	~ColorPicker();

private:
	QLabel* colorLabel;
	QLabel* rgbValue;
	QLabel* hexValue;
	QTimer* timer;
	QColor color;

private slots:
	void showColorDialog();

private:
	void updateDisplay();
	void updateColor();
};
