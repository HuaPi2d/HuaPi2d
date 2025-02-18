#include "modifytaskdialog.h"

ModifyTaskDialog::ModifyTaskDialog(QWidget *parent)
	: QDialog(parent)
	, ui(new Ui::ModifyTaskDialogClass())
{
	ui->setupUi(this);
	QIntValidator* validator = new QIntValidator(0, 999, this);
	ui->runTimesLineEdit->setValidator(validator);
	ui->finishedTimesLineEdit->setValidator(validator);

	connect(ui->confirmPushButton, &QPushButton::clicked, this, &ModifyTaskDialog::accept);
	connect(ui->cancelPushButton, &QPushButton::clicked, this, &ModifyTaskDialog::reject);
}

ModifyTaskDialog::~ModifyTaskDialog()
{
	delete ui;
}

void ModifyTaskDialog::setTaskType(Task::TaskType type)
{
	m_taskType = type;
}

void ModifyTaskDialog::setTaskName(const QString& name)
{
	m_taskName = name;
}

void ModifyTaskDialog::setDifficultyMode(QList<DifficultyMode> modes, QString currentMode)
{
	m_difficultyModes = modes;
	m_currentMode = currentMode;
}

void ModifyTaskDialog::setScriptList(QStringList scripts, QString currentScript)
{
	m_scriptList = scripts;
	m_currentScript = currentScript;
}

void ModifyTaskDialog::setTimes(int runTimes, int finishedTimes)
{
	m_runTimes = runTimes;
	m_finishedTimes = finishedTimes;
}

void ModifyTaskDialog::updateUI()
{
	ui->taskNameLabel->setText(m_taskName);
	for (DifficultyMode mode : m_difficultyModes) {
		ui->difficultyComboBox->addItem(mode.name);
	}
	ui->difficultyComboBox->setCurrentText(m_currentMode);
	ui->scriptComboBox->addItems(m_scriptList);
	if (m_currentScript != "未选择") {
		ui->scriptComboBox->setCurrentText(m_currentScript);
	}
	ui->runTimesLineEdit->setText(QString::number(m_runTimes));
	ui->finishedTimesLineEdit->setText(QString::number(m_finishedTimes));
}

QString ModifyTaskDialog::getDifficultyMode() const
{
	return ui->difficultyComboBox->currentText();
}

QString ModifyTaskDialog::getScript() const
{
	return ui->scriptComboBox->currentText();
}

int ModifyTaskDialog::getRunTimes() const
{
	return ui->runTimesLineEdit->text().toInt();
}

int ModifyTaskDialog::getFinishedTimes() const
{
	return ui->finishedTimesLineEdit->text().toInt();
}
