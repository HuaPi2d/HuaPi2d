#include "knowledgeitemwidget.h"

KnowledgeItemWidget::KnowledgeItemWidget(QWidget* parent, int index, QString question, QString answer)
	: QWidget(parent)
{
	ui.setupUi(this);
	m_index = index;
	m_question = question;
	m_answer = answer;
	ui.questionTextLabel->setText(m_question);
	ui.answerTextLabel->setText(m_answer);
	ui.questionTextLabel->setAlignment(Qt::AlignLeft | Qt::AlignTop);
	ui.answerTextLabel->setAlignment(Qt::AlignLeft | Qt::AlignTop);

	// 更新语言
	connect(Language, &GlobalVariableQString::valueChanged, this, [=]() {
		ui.retranslateUi(this);
		});
	reloadLanguage(Language->value());
}

KnowledgeItemWidget::~KnowledgeItemWidget()
{}

void KnowledgeItemWidget::mousePressEvent(QMouseEvent * event)
{
	if (event->button() == Qt::RightButton) 
	{
		if (m_index != -1)
		{
			QMenu menu(this);
			QAction* deleteAction = new QAction(tr("删除"), this);
			deleteAction->setIcon(QIcon(":/icon/resources/icons/delete.svg"));
			menu.addAction(deleteAction);
			connect(deleteAction, &QAction::triggered, this, [=]() {
				// 向主列表框发送删除数据的信号
				emit deleteItem(m_index);
				});
			menu.exec(event->globalPosition().toPoint());
		}
	}
}
