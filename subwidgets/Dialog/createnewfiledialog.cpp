#include "createnewfiledialog.h"


FileType emptyScriptalFileType = { ".scp (空的脚本)", ".scp", QList<FileType>()};
FileType luanDouScriptalFileType = { ".lscp (乱斗脚本)", ".lscp", QList<FileType>() };
FileType zhuXianScriptalFileType = { ".zscp (主线脚本)", ".zscp", QList<FileType>() };
QList<FileType> scriptalChildFileTypes = { emptyScriptalFileType, luanDouScriptalFileType, zhuXianScriptalFileType };
FileType scriptalFileType = { ".scp (脚本文件)", ".scp", scriptalChildFileTypes };

CreateNewFileDialog::CreateNewFileDialog(QWidget* parent, QList<FileType> fileTypes, QString defaultSavePath)
	: QDialog(parent)
	, ui(new Ui::CreateNewFileDialogClass())
{
	ui->setupUi(this);

	/* 窗口设置 */
	// 固定大小
	ui->zxWidget->hide();
	this->resize(400, 210);

	// 加载文件类型列表至 combox
	for (auto fileType : fileTypes) {
		ui->fileTypeComboBox->addItem(fileType.description);
	}

	// 默认选择第一个文件类型
	ui->fileTypeComboBox->setCurrentIndex(0);
	selectedFileType = fileTypes[0];

	// 子文件类型列表
	QList<FileType> childFileTypes = selectedFileType.childTypes;

	// 加载子文件类型列表至 combox
	for (auto fileType : childFileTypes) {
		ui->childFileTypesComboBox->addItem(fileType.description);
	}

	// 默认选择第一个子文件类型
	ui->childFileTypesComboBox->setCurrentIndex(0);
	selectedChildFileType = childFileTypes[0];
	ui->fileTypeSuffix->setText(selectedChildFileType.suffix);

	// 保存路径
	// 将相对路径转换为绝对路径
	QDir defaultSaveDir(defaultSavePath);
	defaultSavePath = defaultSaveDir.absolutePath();
	parentDir = defaultSavePath;
	ui->savePathLineEdit->setText(parentDir);

	// 更新
	QList<ZXChapter> zxChapters = ZXGameData::getChapters();
	ui->zxChaptersComboBox->clear();
	for (auto chapter : zxChapters) {
		ui->zxChaptersComboBox->addItem(chapter.name);
	}
	ui->zxChaptersComboBox->setCurrentIndex(0);
	connect(ui->zxChaptersComboBox, &QComboBox::currentTextChanged, this, [=](QString text) {
		for (ZXChapter chapter : zxChapters) {
			if (chapter.name == text) {
				zxLevels = chapter.levels;
				break;
			}
		}
		ui->zxLevelsComboBox->clear();
		for (auto level : zxLevels) {
			ui->zxLevelsComboBox->addItem(level.name);
		}
		});
	connect(ui->zxLevelsComboBox, &QComboBox::currentTextChanged, this, [=](QString text) {
		for (ZXLevel level : zxLevels) {
			if (level.name == text) {
				zxLevel = level;
				break;
			}
		}
		ui->zxDiffcultiesComboBox->clear();
		for (auto modes : zxLevel.modes) {
			ui->zxDiffcultiesComboBox->addItem(modes.name);
		}
		});
	ui->zxLevelsComboBox->setCurrentIndex(0);
	// 信号与槽连接
	connect(ui->fileTypeComboBox, &QComboBox::currentIndexChanged, this, [=](int index) {
		FileType selectedFileType = fileTypes[index];
		ui->childFileTypesComboBox->clear();
		for (auto fileType : selectedFileType.childTypes) {
			ui->childFileTypesComboBox->addItem(fileType.description);
		}
		});
	connect(ui->childFileTypesComboBox, &QComboBox::currentTextChanged, this, [=](QString text) {
		int index = ui->childFileTypesComboBox->findText(text);
		selectedChildFileType = childFileTypes[index];
		ui->fileTypeSuffix->setText(selectedChildFileType.suffix);
		if (selectedChildFileType.suffix == ".zscp") {
			ui->zxWidget->show();
			ui->savePathLineEdit->setText(parentDir + "/zx");
		}
		else if (selectedChildFileType.suffix == ".lscp") {
			ui->savePathLineEdit->setText(parentDir + "/ld");
		}
		else {
			ui->zxWidget->hide();
			this->resize(400, 210);
		}
		});
	connect(ui->cancelPushButton, &QPushButton::clicked, this, &QDialog::reject);
	connect(ui->confirmPushButton, &QPushButton::clicked, this, [=]() {
		// 检查文件名是否为空
		if (ui->fileNameLineEdit->text().isEmpty()) {
			QMessageBox::warning(this, "警告", "文件名不能为空！");
			return;
		}
		// 检查文件是否已经存在
		QFile file(ui->savePathLineEdit->text() + "/" + ui->fileNameLineEdit->text() + selectedChildFileType.suffix);
		if (file.exists()) {
			QMessageBox::warning(this, "警告", "文件已经存在！");
			return;
		}
		accept();
		});
}

CreateNewFileDialog::~CreateNewFileDialog()
{
	delete ui;
}

QString CreateNewFileDialog::getFileName()
{
	QString fileName = ui->fileNameLineEdit->text() + selectedChildFileType.suffix;
	return fileName;
}

QString CreateNewFileDialog::getSavePath()
{
	QString savePath = ui->savePathLineEdit->text();
	return savePath;
}

QList<FileAttribute> CreateNewFileDialog::getFileAttribute()
{
	if (selectedChildFileType.suffix == ".zscp") {
		FileAttribute chapter;
		chapter.name = "chapter";
		chapter.value = ui->zxChaptersComboBox->currentText();
		FileAttribute level;
		level.name = "level";
		level.value = ui->zxLevelsComboBox->currentText();
		FileAttribute difficulty;
		difficulty.name = "difficulty";
		difficulty.value = ui->zxDiffcultiesComboBox->currentText();
		FileAttribute speed;
		speed.name = "speed";
		speed.value = "";
		return QList<FileAttribute>() << chapter << level << difficulty << speed;
	}

	return QList<FileAttribute>();
}
