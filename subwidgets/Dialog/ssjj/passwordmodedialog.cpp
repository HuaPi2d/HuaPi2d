#include "passwordmodedialog.h"

PasswordModeDialog::PasswordModeDialog(QWidget *parent)
	: QDialog(parent)
	, ui(new Ui::PasswordModeDialogClass())
{
	ui->setupUi(this);

	this->resize(400, 150);
	// 固定窗口大小
	this->setFixedSize(this->width(), this->height());

	if (passWordMode == 1) {
		ui->mode1RadioButton->setChecked(true);
	}
	else if (passWordMode == 2) {
		ui->mode2RadioButton->setChecked(true);
	}
	else if (passWordMode == 3) {
		ui->mode3RadioButton->setChecked(true);
	}
	// 使密码输入框只能输入数字或字母
	ui->mode1PassWordLineEdit->setValidator(new QRegularExpressionValidator(QRegularExpression("[a-zA-Z0-9]+"), ui->mode1PassWordLineEdit));
	ui->mode1PassWordLineEdit->setText(enterGamePassWord);

	connect(ui->confirmPushButton, &QPushButton::clicked, this, &PasswordModeDialog::accept);
	connect(ui->cancelPushButton, &QPushButton::clicked, this, &PasswordModeDialog::reject);
}

PasswordModeDialog::~PasswordModeDialog()
{
	delete ui;
}

int PasswordModeDialog::getPasswordMode()
{
	if (ui->mode1RadioButton->isChecked())
	{
		if (ui->mode1PassWordLineEdit->text().isEmpty() == false) {
			enterGamePassWord = ui->mode1PassWordLineEdit->text();
		}
		else {
			enterGamePassWord = "HuaPi2D";
		}
		return 1;
	}
	else if (ui->mode2RadioButton->isChecked())
	{
		return 2;
	}
	else if (ui->mode3RadioButton->isChecked())
	{
		return 3;
	}
}
