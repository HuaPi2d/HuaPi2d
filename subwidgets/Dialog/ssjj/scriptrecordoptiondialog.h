#pragma once

#include <QDialog>
#include "ui_scriptrecordoptiondialog.h"

QT_BEGIN_NAMESPACE
namespace Ui { class ScriptRecordOptionDialogClass; };
QT_END_NAMESPACE

class ScriptRecordOptionDialog : public QDialog
{
	Q_OBJECT

public:
	ScriptRecordOptionDialog(QWidget *parent = nullptr);
	~ScriptRecordOptionDialog();

	bool ifShowTeachingDialogOption();
	void setIfShowTeachingDialogOption(bool ifShow);

private:
	Ui::ScriptRecordOptionDialogClass *ui;
};
