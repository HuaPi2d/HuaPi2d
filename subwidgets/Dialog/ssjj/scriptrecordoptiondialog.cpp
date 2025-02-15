#include "scriptrecordoptiondialog.h"

ScriptRecordOptionDialog::ScriptRecordOptionDialog(QWidget *parent)
	: QDialog(parent)
	, ui(new Ui::ScriptRecordOptionDialogClass())
{
	ui->setupUi(this);
	connect(ui->confirmPushButton, &QPushButton::clicked, this, &QDialog::accept);
	connect(ui->cancelPushButton, &QPushButton::clicked, this, &QDialog::reject);
}

ScriptRecordOptionDialog::~ScriptRecordOptionDialog()
{
	delete ui;
}

bool ScriptRecordOptionDialog::ifShowTeachingDialogOption()
{
	return ui->ifShowTeachingDialogCheckBox->isChecked();
}

void ScriptRecordOptionDialog::setIfShowTeachingDialogOption(bool ifShow)
{
	ui->ifShowTeachingDialogCheckBox->setChecked(ifShow);
}
