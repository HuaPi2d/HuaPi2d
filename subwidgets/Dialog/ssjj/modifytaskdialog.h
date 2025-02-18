#pragma once

#include <QDialog>
#include <QWidget>
#include <QStringList>
#include <QIntValidator>

#include "ui_modifytaskdialog.h"
#include "ssjjCore/zx/zxlevels.h"
#include "struct/ssjj/ssjjstruct.h"

QT_BEGIN_NAMESPACE
namespace Ui { class ModifyTaskDialogClass; };
QT_END_NAMESPACE

class ModifyTaskDialog : public QDialog
{
	Q_OBJECT

public:
	ModifyTaskDialog(QWidget *parent = nullptr);
	~ModifyTaskDialog();

public slots:
	void setTaskType(Task::TaskType type);
	void setTaskName(const QString& name);
	void setDifficultyMode(QList<DifficultyMode> modes, QString currentMode);
	void setScriptList(QStringList scripts, QString currentScript);
	void setTimes(int runTimes, int finishedTimes);
	void updateUI();

	QString getDifficultyMode() const;
	QString getScript() const;
	int getRunTimes() const;
	int getFinishedTimes() const;

private:
	Ui::ModifyTaskDialogClass *ui;

	Task::TaskType m_taskType;
	QString m_taskName;
	QList<DifficultyMode> m_difficultyModes;
	QString m_currentMode;
	QStringList m_scriptList;
	QString m_currentScript;
	int m_runTimes;
	int m_finishedTimes;
};
