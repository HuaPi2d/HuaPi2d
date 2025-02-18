#include "modifyfileattributesdialog.h"

ModifyFileAttributesDialog::ModifyFileAttributesDialog(QWidget *parent)
	: QDialog(parent)
	, ui(new Ui::ModifyFileAttributesDialogClass())
{
	ui->setupUi(this);

	// ¸üĞÂ
	QList<ZXChapter> zxChapters = ZXGameData::getChapters();
	ui->chapterComboBox->clear();
	for (auto chapter : zxChapters) {
		ui->chapterComboBox->addItem(chapter.name);
	}
	connect(ui->chapterComboBox, &QComboBox::currentTextChanged, this, [=](QString text) {
		for (ZXChapter chapter : zxChapters) {
			if (chapter.name == text) {
				zxLevels = chapter.levels;
				break;
			}
		}
		ui->levelComboBox->clear();
		for (auto level : zxLevels) {
			ui->levelComboBox->addItem(level.name);
		}
		});
	connect(ui->levelComboBox, &QComboBox::currentTextChanged, this, [=](QString text) {
		ui->fileNameLineEdit->setText(text);
		for (ZXLevel level : zxLevels) {
			if (level.name == text) {
				zxLevel = level;
				break;
			}
		}
		ui->difficultyComboBox->clear();
		for (auto modes : zxLevel.modes) {
			ui->difficultyComboBox->addItem(modes.name);
		}
		});
	connect(ui->confirmPushButton, &QPushButton::clicked, this, &ModifyFileAttributesDialog::accept);
	connect(ui->cancelPushButton, &QPushButton::clicked, this, &ModifyFileAttributesDialog::reject);
}

ModifyFileAttributesDialog::~ModifyFileAttributesDialog()
{
	delete ui;
}

void ModifyFileAttributesDialog::setFileName(const QString& fileName)
{
	ui->fileNameLineEdit->setText(fileName);
}

void ModifyFileAttributesDialog::setFilePath(const QString& fileInfo)
{
	fileAttributesMap = readFileAttributesMap(fileInfo);
	chapter = fileAttributesMap["chapter"];
	level = fileAttributesMap["level"];
	difficulty = fileAttributesMap["difficulty"];
	ui->chapterComboBox->setCurrentText(chapter);
	emit ui->chapterComboBox->currentTextChanged(chapter);
	ui->levelComboBox->setCurrentText(level);
	emit ui->levelComboBox->currentTextChanged(level);
	ui->difficultyComboBox->setCurrentText(difficulty);
	emit ui->difficultyComboBox->currentTextChanged(difficulty);
}

QString ModifyFileAttributesDialog::getFileName() const
{
	return ui->fileNameLineEdit->text();
}

QMap<QString, QString> ModifyFileAttributesDialog::getFileAttributesMap()
{
	fileAttributesMap["chapter"] = ui->chapterComboBox->currentText();
	fileAttributesMap["level"] = ui->levelComboBox->currentText();
	fileAttributesMap["difficulty"] = ui->difficultyComboBox->currentText();
	return fileAttributesMap;
}
