#include "createnewfiledialog.h"


FileType emptyScriptalFileType = { ".scp (空的脚本)", ".scp", QList<FileType>() };
FileType luanDouScriptalFileType = { ".lscp (乱斗脚本)", ".lscp", QList<FileType>() };
FileType zhuXianScriptalFileType = { ".zscp (主线脚本)", ".zscp", QList<FileType>() };
QList<FileType> scriptalChildFileTypes = { emptyScriptalFileType, luanDouScriptalFileType, zhuXianScriptalFileType };
FileType scriptalFileType = { ".scp (脚本文件)", ".scp", scriptalChildFileTypes };

CreateNewFileDialog::CreateNewFileDialog(QWidget* parent, QList<FileType> fileTypes, QString defaultSavePath)
	: QDialog(parent)
	, ui(new Ui::CreateNewFileDialogClass())
{
	ui->setupUi(this);

	// 更新语言
	connect(Language, &GlobalVariableQString::valueChanged, this, [=]() {
		ui->retranslateUi(this);
		});
	reloadLanguage(Language->value());

	/* 窗口设置 */
	// 固定大小
	ui->zxWidget->hide();
	this->resize(600, 210);

	// 更新
	QList<ZXChapter> zxChapters = ZXGameData::getChapters();
	ui->zxChaptersComboBox->clear();
	for (auto chapter : zxChapters) {
		ui->zxChaptersComboBox->addItem(chapter.name);
	}
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
		if (selectedChildFileType.suffix == ".zscp") {
			ui->fileNameLineEdit->setText(text);
		}
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
	emit ui->zxChaptersComboBox->currentTextChanged(ui->zxChaptersComboBox->currentText());

	// 保存路径
	// 将相对路径转换为绝对路径
	QDir defaultSaveDir(defaultSavePath);
	defaultSavePath = defaultSaveDir.absolutePath();
	parentDir = defaultSavePath;
	ui->savePathLineEdit->setText(parentDir);
	
	// 子文件类型
	connect(ui->childFileTypesComboBox, &QComboBox::currentIndexChanged, this, [=](int index) {
		if(index == -1)
			return;
		selectedChildFileType = childFileTypes[index];
		ui->fileTypeSuffix->setText(selectedChildFileType.description);
		if (selectedChildFileType.suffix == ".zscp") {
			ui->zxWidget->show();
			ui->savePathLineEdit->setText(parentDir + "/zx");
		}
		else if (selectedChildFileType.suffix == ".lscp") {
			ui->zxWidget->hide();
			ui->savePathLineEdit->setText(parentDir + "/ld");
			this->resize(600, 210);
		}
		else {
			ui->zxWidget->hide();
			ui->savePathLineEdit->setText(parentDir);
			this->resize(600, 210);
		}
		});

	// 顶级文件类型
	for (auto fileType : fileTypes) {
		ui->fileTypeComboBox->addItem(fileType.description);
	}
	connect(ui->fileTypeComboBox, &QComboBox::currentIndexChanged, this, [=](int index) {
		if(index == -1)
			return;
		selectedFileType = fileTypes[index];
		childFileTypes = selectedFileType.childTypes;
		ui->childFileTypesComboBox->clear();
		// 加载子文件类型列表至 combox
		for (auto fileType : childFileTypes) {
			ui->childFileTypesComboBox->addItem(fileType.description);
		}
		emit ui->childFileTypesComboBox->currentIndexChanged(0);
		});

	emit ui->fileTypeComboBox->currentIndexChanged(0);

	connect(ui->cancelPushButton, &QPushButton::clicked, this, &QDialog::reject);
	connect(ui->confirmPushButton, &QPushButton::clicked, this, [=]() {
		// 检查文件名是否为空
		if (ui->fileNameLineEdit->text().isEmpty()) {
			QMessageBox::warning(this, tr("警告"), tr("文件名不能为空！"));
			return;
		}
		// 检查文件是否已经存在
		QFile file(ui->savePathLineEdit->text() + "/" + ui->fileNameLineEdit->text() + selectedChildFileType.suffix);
		if (file.exists()) {
			QMessageBox::warning(this, tr("警告"), tr("文件已经存在！"));
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

QMap<QString, QString> CreateNewFileDialog::getFileAttributeMap()
{
	if (selectedChildFileType.suffix == ".zscp") {
		QMap<QString, QString> attributeMap;
		attributeMap["chapter"] = ui->zxChaptersComboBox->currentText();
		attributeMap["level"] = ui->zxLevelsComboBox->currentText();
		attributeMap["difficulty"] = ui->zxDiffcultiesComboBox->currentText();
		attributeMap["speed"] = "";
		return attributeMap;
	}
	return QMap<QString, QString>();
}
