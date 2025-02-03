#pragma once

#include <QWidget>
#include <QMenu>
#include <QModelIndex>
#include <QMouseEvent>

#include "ui_knowledgeitemwidget.h"
#include "global/mainglobalvar.h"

class KnowledgeItemWidget : public QWidget
{
	Q_OBJECT

public:
	KnowledgeItemWidget(QWidget *parent = nullptr, int index = -1, QString question = "", QString answer = "");
	~KnowledgeItemWidget();

private:
	Ui::KnowledgeItemWidgetClass ui;
	int m_index;
	QString m_question;
	QString m_answer;

public: signals:
	void deleteItem(int index);

protected:
	void mousePressEvent(QMouseEvent* event) override;
};
