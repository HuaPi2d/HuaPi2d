#pragma once

#include <QDialog>

#include "ui_recordteachingdialog.h"
#include "universal/filepath/filepath.h"
#include "QHotkey/qhotkey.h"


QT_BEGIN_NAMESPACE
namespace Ui { class RecordTeachingDialogClass; };
QT_END_NAMESPACE

class RecordTeachingDialog : public QDialog
{
	Q_OBJECT

public:
	RecordTeachingDialog(QWidget *parent = nullptr);
	~RecordTeachingDialog();

	bool getIfShowNextTime();
	void receiveRecordType(int type);

private:
	Ui::RecordTeachingDialogClass *ui;
};
