#include "subaawidget.h"

SubAAWidget::SubAAWidget(QWidget *parent)
	: QWidget(parent)
	, ui(new Ui::SubAAWidgetClass())
{
	ui->setupUi(this);

	/* 限制表格内输入数字*/
	QLineEdit *heigetLineEdit = new QLineEdit(ui->airTableWidget);
	QDoubleValidator* validator = new QDoubleValidator(heigetLineEdit);
	heigetLineEdit->setValidator(validator);
	heigetLineEdit->setAlignment(Qt::AlignCenter);
	ui->airTableWidget->setCellWidget(0, 0, heigetLineEdit);
	QLineEdit *rhoLineEdit = new QLineEdit(ui->airTableWidget);
	rhoLineEdit->setValidator(validator);
	rhoLineEdit->setAlignment(Qt::AlignCenter);
	ui->airTableWidget->setCellWidget(1, 0, rhoLineEdit);

	/* 表格输入框按下 Enter 触发信号 */
	connect(heigetLineEdit, &QLineEdit::returnPressed, this, [=]() {
		rhoLineEdit->setText(QString::number(calculate_air_density(heigetLineEdit->text().toDouble())));
		});

	connect(ui->closePushButton, &QPushButton::clicked, this, [=]() {
		emit widgetClosed();
	});
}

SubAAWidget::~SubAAWidget()
{
	delete ui;
}
