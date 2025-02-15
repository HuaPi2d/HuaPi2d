#include "aboutsoftwaredialog.h"

AboutSoftwareDialog::AboutSoftwareDialog(QWidget *parent)
	: QDialog(parent)
	, ui(new Ui::AboutSoftwareDialogClass())
{
	ui->setupUi(this);

	ui->websiteUrlLabel;
	ui->installPathLabel->setText(qApp->applicationDirPath());

	ui->treeWidget->addTopLevelDir("bonusConfig");
	ui->treeWidget->addTopLevelDir("music");
	ui->treeWidget->addTopLevelDir("QSS");
	ui->treeWidget->addTopLevelDir("scriptal");
	ui->treeWidget->addTopLevelDir("Settings");
	ui->treeWidget->addTopLevelDir("databases");

	ui->authorNameLabel->setUrl("https://space.bilibili.com/1923219446");
}

AboutSoftwareDialog::~AboutSoftwareDialog()
{
	delete ui;
}
