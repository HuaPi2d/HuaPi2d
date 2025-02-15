﻿#include "subaawidget.h"

SubAAWidget::SubAAWidget(QWidget *parent)
	: QWidget(parent)
	, ui(new Ui::SubAAWidgetClass())
{
	ui->setupUi(this);

	/* 添加widget至列表 */
	widgetList.append(ui->singleFunctionWidget);
	widgetList.append(ui->rocketSimulationWidget);

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

	/* 显示提示信息 */
	createRemindText();

	/* 设置列和行的大小填充 */
	ui->eulerResultTableWidget->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
	ui->eulerResultTableWidget->verticalHeader()->setSectionResizeMode(QHeaderView::Stretch);
	ui->eulerResultTableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);

	/* 表格输入框按下 Enter 触发信号 */
	connect(heigetLineEdit, &QLineEdit::returnPressed, this, [=]() {
		rhoLineEdit->setText(QString::number(calculate_air_density(heigetLineEdit->text().toDouble())));
		});
	/* 欧拉坐标变换矩阵转换按钮按下 */
	connect(ui->changePushButton, &QPushButton::clicked, this, &SubAAWidget::calculateEulerRotationMatrix);

	connect(ui->closePushButton, &QPushButton::clicked, this, [=]() {
		emit widgetClosed();
	});

	connect(Language, &GlobalVariableQString::valueChanged, this, [=]() {
		ui->retranslateUi(this);
		});
	loadSettings();
	ui->retranslateUi(this);
}

SubAAWidget::~SubAAWidget()
{
	saveSettigs();
	delete ui;
}

void SubAAWidget::saveSettigs()
{
	/* 保存设置 */
	/* 声明对象 */
	UsersSettings settings(qApp->applicationDirPath() + "/Settings/aaSettings.ini");

	/* 写入配置 */
	settings.beginGroup("aa");
	if (ui->eulerRotationParamsTextEdit->currentText == ui->eulerRotationParamsTextEdit->placeholderText) {
		settings.setValue("eulerConvertParams", "");
	}
	else {
		settings.setValue("eulerConvertParams", ui->eulerRotationParamsTextEdit->toHtml());
	}
	settings.setValue("currentPage", widgetList.indexOf(currentWidget));
	settings.endGroup();
}

void SubAAWidget::loadSettings()
{
	/* 声明对象 */
	UsersSettings settings(qApp->applicationDirPath() + "/Settings/aaSettings.ini");

	/* 读取配置 */
	settings.beginGroup("aa");
	// 欧拉旋转矩阵
	if (settings.value("eulerConvertParams").toString() != "") {
		ui->eulerRotationParamsTextEdit->setHtml(settings.value("eulerConvertParams").toString());
		ui->eulerRotationParamsTextEdit->currentText = settings.value("eulerConvertParams").toString();
	}
	currentWidget = widgetList[settings.value("currentPage", "0").toInt()];
	updateScreen();
	settings.endGroup();
}

void SubAAWidget::createRemindText()
{
	ui->eulerRotationParamsTextEdit->receivePlacholderText(QString(tr("(旋转轴, 旋转角度)x-1,y-2,z-3\n如：\n(3, 45)\n(2, 15)\n(3, -30)")));
}

void SubAAWidget::calculateEulerRotationMatrix()
{
	/* 获取输入参数 */
	QString inputText = ui->eulerRotationParamsTextEdit->toPlainText();

	/* 正则表达式匹配括号中的数字 */ 
	QRegularExpression re("\\((\\d+),\\s*(-?\\d+\\.\\d*|-?\\d+)\\)");
	QRegularExpressionMatchIterator i = re.globalMatch(inputText);

	/* 存储相关转换参数 */
	QVector<QVector2D> params;

	while (i.hasNext()) {
		QRegularExpressionMatch match = i.next();
		QVector2D single(match.captured(1).toInt(), match.captured(2).toDouble());
		params.append(single);
	}

	/* 计算欧拉坐标变换矩阵 */
	QVector<double> eulerRotationMatrix = euler_rotation_matrix_C(params);
}


/* 界面转换的更新 */
void SubAAWidget::updateScreen()
{
	for (QWidget* m_Widget : widgetList) {
		if (m_Widget == currentWidget) {
			m_Widget->show();
		}
		else {
			m_Widget->hide();
		}
	}
}

void SubAAWidget::updateLanguage()
{
	ui->retranslateUi(this);
}

void SubAAWidget::on_singleFunctionTestPushButton_clicked()
{
	qDebug() << "singleFunctionTestPushButton clicked";
	currentWidget = ui->singleFunctionWidget;
	updateScreen();
}

void SubAAWidget::on_rocketSimulationPushButton_clicked()
{
	qDebug() << "rocketSimulationPushButton clicked";
	currentWidget = ui->rocketSimulationWidget;
	updateScreen();
}
