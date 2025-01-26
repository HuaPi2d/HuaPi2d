#include "colorpickerdialog.h"

ColorPickerDialog::ColorPickerDialog(QWidget *parent)
	: QColorDialog(parent)
	, ui(new Ui::ColorPickerDialogClass())
{
	ui->setupUi(this);

	this->setLocale(QLocale(QLocale::Chinese, QLocale::China));
}

ColorPickerDialog::~ColorPickerDialog()
{
	delete ui;
}
