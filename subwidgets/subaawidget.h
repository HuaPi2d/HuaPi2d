#pragma once

#include <QWidget>
#include <QDoubleValidator>
#include <QLineEdit>
#include <QTableWidgetItem>
#include "ui_subaawidget.h"
#include "Aerospace/air/standardairmodel.h"

QT_BEGIN_NAMESPACE
namespace Ui { class SubAAWidgetClass; };
QT_END_NAMESPACE
	
class SubAAWidget : public QWidget
{
	Q_OBJECT

public:
	SubAAWidget(QWidget *parent = nullptr);
	~SubAAWidget();

public slots:
	

public: signals:
	void widgetClosed();
	void sendStateInfo(QString state);

private:
	Ui::SubAAWidgetClass *ui;
};
