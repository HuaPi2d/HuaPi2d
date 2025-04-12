#include "customizedtablewidget.h"

CustomizedTableWidget::CustomizedTableWidget(QWidget *parent)
	: QTableWidget(parent)
{
    m_deleteActionVisible = true;
}

CustomizedTableWidget::~CustomizedTableWidget()
{}

void CustomizedTableWidget::setDeleteActionVisible(bool enabled)
{
    m_deleteActionVisible = enabled;
}

void CustomizedTableWidget::mousePressEvent(QMouseEvent * event)
{
	QTableWidgetItem* item = this->itemAt(event->pos());
    if (item == nullptr) {
        return;
    }
	if (event->button() == Qt::LeftButton) {
		emit this->itemClicked(item);
	}
	else if (event->button() == Qt::RightButton) {
        emit this->rightItemClicked(item);
        QMenu menu(this);
        if (m_deleteActionVisible) {
            QAction* deleteAction = new QAction(this);
            deleteAction->setText("删除");
            deleteAction->setIcon(QIcon(":/icon/resources/icons/delete.svg"));
            menu.addAction(deleteAction);
            connect(deleteAction, &QAction::triggered, this, [=]() {
                emit deleteRow(item);
            });
        }
        QAction* modifyAction = new QAction(this);
        modifyAction->setText("修改");
        modifyAction->setIcon(QIcon(":/icon/resources/icons/PhNotePencilBold.svg"));
        menu.addAction(modifyAction);
        
        connect(modifyAction, &QAction::triggered, this, [=]() {
            emit modifyRow(item);
            });
        menu.exec(event->globalPosition().toPoint());
        menu.deleteLater();
	}
}
