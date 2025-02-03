#include "colorpickerdialog.h"

ColorPickerDialog::ColorPickerDialog(QWidget *parent)
	: QColorDialog(parent)
	, ui(new Ui::ColorPickerDialogClass())
{
	ui->setupUi(this);

	this->setWindowTitle(tr("颜色选择器"));

	this->setLocale(QLocale(QLocale::Chinese, QLocale::China));

	connect(Language, &GlobalVariableQString::valueChanged, this, [=]() {
		ui->retranslateUi(this);
		});
	reloadLanguage(Language->value());
}

ColorPickerDialog::~ColorPickerDialog()
{
	delete ui;
}
