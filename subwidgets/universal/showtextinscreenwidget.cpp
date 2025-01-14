#include "showtextinscreenwidget.h"

ShowTextInScreenWidget::ShowTextInScreenWidget(QWidget* parent, QString text, QPoint position, int durationMs)
	: QWidget(parent), text_(text), position_(position), durationMs_(durationMs)
{
	this->setWindowFlags(Qt::Tool | Qt::FramelessWindowHint | Qt::WindowStaysOnTopHint);
	this->setAttribute(Qt::WA_TranslucentBackground);
	this->setAttribute(Qt::WA_ShowWithoutActivating);
	this->move(position_);

	label = new QLabel(this);
	label->setText(text_);
	label->setStyleSheet("color: white; background-color: rgba(0, 0, 0, 128); padding: 10px; border-radius: 5px;");
	label->setFont(QFont("Arial", 12, QFont::Bold));
	label->adjustSize();

	// ���ô��ڴ�С�� QLabel ��ͬ
	this->resize(label->size());
}

ShowTextInScreenWidget::~ShowTextInScreenWidget()
{}

void ShowTextInScreenWidget::showText(QWidget* parent, const QString & text, const QPoint & position, int durationMs)
{
	if (developerMode == true)
	{
		ShowTextInScreenWidget* widget = new ShowTextInScreenWidget(parent, text, position, durationMs);
		widget->showTextInScreen();
	}
	
}

void ShowTextInScreenWidget::showTextInScreen()
{
	this->show();

	// �ȴ�ָ��ʱ������ٴ���
	// QThread::msleep(durationMs_);
	QTimer::singleShot(durationMs_, this, &ShowTextInScreenWidget::close);
}
