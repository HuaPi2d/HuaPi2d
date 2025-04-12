#pragma once

#include <QTableWidget>
#include <QMouseEvent>
#include <QMenu>
#include <QAction>

class CustomizedTableWidget  : public QTableWidget
{
	Q_OBJECT

public:
	CustomizedTableWidget(QWidget *parent);
	~CustomizedTableWidget();

public: signals:
	void rightItemClicked(QTableWidgetItem* index);
public: signals:
	void deleteRow(QTableWidgetItem* index);
public: signals:
	void modifyRow(QTableWidgetItem* index);

public slots:
	void setDeleteActionVisible(bool enabled);

protected:
	void mousePressEvent(QMouseEvent *event) override;

private:
	bool m_deleteActionVisible;
};
