#pragma once

#include <QDialog>
#include "ui_aboutsoftwaredialog.h"

QT_BEGIN_NAMESPACE
namespace Ui { class AboutSoftwareDialogClass; };
QT_END_NAMESPACE

class AboutSoftwareDialog : public QDialog
{
	Q_OBJECT

public:
	AboutSoftwareDialog(QWidget *parent = nullptr);
	~AboutSoftwareDialog();

private:
	Ui::AboutSoftwareDialogClass *ui;
};
