#include "subfilesconvertwidget.h"

SubFilesConvertWidget::SubFilesConvertWidget(QWidget *parent)
	: QWidget(parent)
	, ui(new Ui::SubFilesConvertWidgetClass())
{
	ui->setupUi(this);
	ui->remindTextEdit->setReadOnly(true);
	wirteRemindInfo("<p><span style=\"font-size: 13px; color: red;\">请选择需要转换的文件，输出结果保存在原文件夹</span></p><br>");

	// 进度条归零
	ui->convertProgressBar->setValue(0);

	connect(ui->closePushButton, &QPushButton::clicked, this, [=]() {
		emit widgetClosed();
		});

	/* 打开视频文件 */
	QAction* openVideoFileAction = new QAction(tr("打开视频文件"), this);
	openVideoFileAction->setIcon(QIcon(":/icon/resources/icons/PhFolderNotchOpen.svg"));
	connect(openVideoFileAction, &QAction::triggered, this, &SubFilesConvertWidget::openVideoFile);
	ui->originVideoPathLineEdit->addAction(openVideoFileAction, QLineEdit::TrailingPosition);
	QAction* openVideoOutputDirAction = new QAction(tr("选择视频文件输出目录"), this);
	openVideoOutputDirAction->setIcon(QIcon(":/icon/resources/icons/PhFolderNotchOpen.svg"));
	connect(openVideoOutputDirAction, &QAction::triggered, this, &SubFilesConvertWidget::openVideoOutputDir);
	ui->outputVideoPathLineEdit->addAction(openVideoOutputDirAction, QLineEdit::TrailingPosition);
	connect(ui->originVideoPathLineEdit, &QLineEdit::returnPressed, this, &SubFilesConvertWidget::openVideoFile);
	connect(ui->outputVideoPathLineEdit, &QLineEdit::returnPressed, this, &SubFilesConvertWidget::openVideoOutputDir);

	/* 连接信号 */
	// 开始视频转换线程
	connect(ui->beginToConvertVideoPushButton, &QPushButton::clicked, this, &SubFilesConvertWidget::beginToConvertVideo);

	// 加载配置
	loadSettings();
}

SubFilesConvertWidget::~SubFilesConvertWidget()
{
	saveSettings();
	delete ui;
}

void SubFilesConvertWidget::closeEvent(QCloseEvent* event)
{
	emit widgetClosed();
}

void SubFilesConvertWidget::saveSettings() 
{
	/* 声明对象 */
	QSettings setting(qApp->applicationDirPath() + "/userSettings.ini", QSettings::IniFormat);

	/* 写入配置 */
	setting.beginGroup("fileConvert");
	setting.setValue("oringinVideoPath", ui->originVideoPathLineEdit->text());
	setting.setValue("outputVideoPath", ui->outputVideoPathLineEdit->text());
	setting.endGroup();
}

void SubFilesConvertWidget::loadSettings() 
{
	/* 声明对象 */
	QSettings setting(qApp->applicationDirPath() + "/userSettings.ini", QSettings::IniFormat);

	/* 读取配置 */
	setting.beginGroup("fileConvert");
	ui->originVideoPathLineEdit->setText(setting.value("oringinVideoPath", "").toString());
	ui->outputVideoPathLineEdit->setText(setting.value("outputVideoPath", "").toString());
	setting.endGroup();
}

void SubFilesConvertWidget::openVideoFile()
{
	QString fileName = QFileDialog::getOpenFileName(this, tr("打开视频文件"), "", tr("视频文件 (*.mp4 *.avi *.flv *.mkv)"));
	if (fileName.isEmpty()) {
		return;
	}
	ui->originVideoPathLineEdit->setText(fileName);
}

void SubFilesConvertWidget::openVideoOutputDir()
{
	QString dirName = QFileDialog::getExistingDirectory(this, tr("选择视频文件输出目录"), "");
	if (dirName.isEmpty()) {
		return;
	}
	ui->outputVideoPathLineEdit->setText(dirName);
}

void SubFilesConvertWidget::beginToConvertVideo()
{
	VideoFileConvertThread* videoFileConvertThread = new VideoFileConvertThread(this);
	// 发送参数
	connect(this, &SubFilesConvertWidget::sendVideoConvertParams, videoFileConvertThread, &VideoFileConvertThread::receiveParams);
	emit sendVideoConvertParams(ui->originVideoPathLineEdit->text(), ui->outputVideoPathLineEdit->text(), QFileInfo(ui->originVideoPathLineEdit->text()).suffix(), ui->outputVideoTypeComboBox->currentText());

	// 接收状态信息
	connect(videoFileConvertThread, &VideoFileConvertThread::sendStateInfo, this, &SubFilesConvertWidget::receiveVideoConvertInfo);
	connect(videoFileConvertThread, &VideoFileConvertThread::sendTimeInfo, this, [=](int hours, int minutes, int seconds) {
		ui->totalTimeLabel->setText(QString("%1:%2:%3").arg(hours, 2, 10, QChar('0')).arg(minutes, 2, 10, QChar('0')).arg(seconds, 2, 10, QChar('0')));
		ui->convertProgressBar->setMaximum(hours * 3600 + minutes * 60 + seconds);
		ui->convertProgressBar->setValue(0);
		}, Qt::QueuedConnection);
	connect(videoFileConvertThread, &VideoFileConvertThread::sendProgressInfo, this, [=](int hours, int minutes, int seconds) {
		ui->currentTimeLabel->setText(QString("%1:%2:%3").arg(hours, 2, 10, QChar('0')).arg(minutes, 2, 10, QChar('0')).arg(seconds, 2, 10, QChar('0')));
		ui->convertProgressBar->setValue(hours * 3600 + minutes * 60 + seconds);
		}, Qt::QueuedConnection);

	// 开始线程
	videoFileConvertThread->start();
}

void SubFilesConvertWidget::receiveVideoConvertInfo(QString state)
{
	wirteRemindInfo(state);
}

// 添加提示信息
void SubFilesConvertWidget::wirteRemindInfo(QString htmlString)
{
	// 获取当前光标位置
	QTextCursor cursor = ui->remindTextEdit->textCursor();

	// 将光标移动到文档末尾
	cursor.movePosition(QTextCursor::End);

	cursor.insertHtml(htmlString);

	// 滚动到 QTextEdit 的末尾
	QScrollBar* scrollBar = ui->remindTextEdit->verticalScrollBar();
	scrollBar->setValue(scrollBar->maximum());
}
