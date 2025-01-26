#pragma once

#include <QColorDialog>
#include <QLocale>
#include "ui_colorpickerdialog.h"

QT_BEGIN_NAMESPACE
namespace Ui { class ColorPickerDialogClass; };
QT_END_NAMESPACE

class ColorPickerDialog : public QColorDialog
{
	Q_OBJECT

public:
	ColorPickerDialog(QWidget *parent = nullptr);
	~ColorPickerDialog();

private:
	Ui::ColorPickerDialogClass *ui;
};
