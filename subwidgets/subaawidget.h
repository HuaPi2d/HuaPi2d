#pragma once

#include <QWidget>
#include <QDoubleValidator>
#include <QLineEdit>
#include <QTableWidgetItem>
#include <QSettings>
#include "ui_subaawidget.h"
#include "Aerospace/air/standardairmodel.h"
#include "Aerospace/universal/coordinate.h"
#include "universal/file/userssettings.h"


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
	QVector<QWidget*> widgetList;
	QWidget* currentWidget;

	void saveSettigs();
	void loadSettings();
	void createRemindText();
	void calculateEulerRotationMatrix();
	void updateScreen();

private slots:
	void on_singleFunctionTestPushButton_clicked();
	void on_rocketSimulationPushButton_clicked();
};
