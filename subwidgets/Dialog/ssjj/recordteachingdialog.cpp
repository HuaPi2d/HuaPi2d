#include "recordteachingdialog.h"

RecordTeachingDialog::RecordTeachingDialog(QWidget *parent)
	: QDialog(parent)
	, ui(new Ui::RecordTeachingDialogClass())
{
	ui->setupUi(this);
	QHotkey* Enter = new QHotkey(QKeySequence("Enter"), true, this);
	connect(Enter, &QHotkey::activated, this, &RecordTeachingDialog::accept);
	connect(ui->nextStepPushButton, &QPushButton::clicked, this, &RecordTeachingDialog::accept);
	connect(ui->exitPushButton, &QPushButton::clicked, this, &RecordTeachingDialog::reject);
	ui->teachingTextEdit->setReadOnly(true);
	ui->currentScriptTypeLineEdit->setReadOnly(true);
}

RecordTeachingDialog::~RecordTeachingDialog()
{
	delete ui;
}

bool RecordTeachingDialog::getIfShowNextTime()
{
	return ui->ifShowCheckBox->isChecked();
}

void RecordTeachingDialog::receiveRecordType(int type)
{
	if (type == 0) {
		ui->currentScriptTypeLineEdit->setText(QCoreApplication::translate("RecordTeachingDialog", "空的脚本", nullptr));
	}
	else if (type == 1) {
		ui->currentScriptTypeLineEdit->setText(QCoreApplication::translate("RecordTeachingDialog", "主线脚本", nullptr));
	}
	else if (type == 2) {
		ui->currentScriptTypeLineEdit->setText(QCoreApplication::translate("RecordTeachingDialog", "乱斗脚本", nullptr));
	}
	if (type == 1) {
		ui->teachingTextEdit->insertHtml("<p style=\"font-size:24px;\ color:red;\">" +
			QCoreApplication::translate("RecordTeachingDialog", "当前为主线脚本录制模式，在录制前请阅读以下教程：", nullptr) + "</p><span style=\"font-size:16px;\"><ul><li>" + 
			QCoreApplication::translate("RecordTeachingDialog", "建议新建一个空的主线脚本文件，防止覆盖原有脚本的代码。", nullptr) + "<br></li><li>" + 
			QCoreApplication::translate("RecordTeachingDialog", "在录制前将游戏界面切换至对应关卡位置，如城市四：", nullptr) + "<br><img src = \""
			+ tips_file_path("tips/zhuxian_record_tip1.png") + "\" width=\"350\" alt=\"" + tr("提示图片") + "\"><br></li><li>" + 
			QCoreApplication::translate("RecordTeachingDialog", "在对应关卡界面内，按下进入录制按钮或Enter键后，等待脚本自动操作进入游戏。", nullptr) + 
			"<br></li><li>" + QCoreApplication::translate("RecordTeachingDialog", "进入游戏后，当屏幕出现如下提示后用户即可开始通过键盘操作，录制脚本。", nullptr) + 
			"<br><img src = \"" + tips_file_path("tips/zhuxian_record_tip2.png") + "\" width=\"450\" alt=\"" + tr("提示图片") + "\"><br></li>" + 
			"<li>" + QCoreApplication::translate("RecordTeachingDialog", "在录制过程中，不要操作您的鼠标，使用如下键盘操作代替：", nullptr) + "<br><ul><li>" + 
			QCoreApplication::translate("RecordTeachingDialog", "\"↑↓←→\"：转动视角", nullptr) + "<br></li><li>" + 
			QCoreApplication::translate("RecordTeachingDialog", "设置游戏内的鼠标灵敏度为40，确保按下两次→或←后，人物正好转动90度", nullptr) + "<br></li><li>" +
			QCoreApplication::translate("RecordTeachingDialog", "\"Enter\"：鼠标左键按下或松开", nullptr) + "<br></li><li>" + 
			QCoreApplication::translate("RecordTeachingDialog", "\"/\"：鼠标中键", nullptr) + "<br></li><li>" + 
			QCoreApplication::translate("RecordTeachingDialog", "\"L\"：设置标记点", nullptr) + "<br></li><li>" + 
			QCoreApplication::translate("RecordTeachingDialog", "尽量采用少的按键操作，这样可以减少脚本出错的概率", nullptr) + "<br></li></ul></li>" +
			"<li>" + QCoreApplication::translate("RecordTeachingDialog", "按F11结束脚本录制", nullptr) + "<br></li></ul></span>");
	}
	else{
		ui->teachingTextEdit->insertHtml("<font color='red'>" + QCoreApplication::translate("RecordTeachingDialog", "目前仅实现了主线脚本录制功能", nullptr) + "< / font>");
	}
}
