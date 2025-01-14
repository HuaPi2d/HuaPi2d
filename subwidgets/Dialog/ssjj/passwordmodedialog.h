#pragma once

#include <QDialog>
#include "ui_passwordmodedialog.h"
#include <qabstractitemmodel.h>
#include <QRegularExpressionValidator>
#include <QRegularExpression>
#include "global/ssjjglobalvar.h"

QT_BEGIN_NAMESPACE
namespace Ui { class PasswordModeDialogClass; };
QT_END_NAMESPACE

class PasswordModeDialog : public QDialog
{
	Q_OBJECT

public:
	PasswordModeDialog(QWidget *parent = nullptr);
	~PasswordModeDialog();

	int getPasswordMode();

private:
	Ui::PasswordModeDialogClass *ui;
};
