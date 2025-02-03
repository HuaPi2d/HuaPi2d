#pragma once

#include <QWidget>
#include <QScrollBar>
#include <QAction>
#include <QFileDialog>
#include <QSettings>
#include <thread/fileConvert/videofileconvertthread.h>
#include <QFileInfo>

#include "ui_subfilesconvertwidget.h"
#include "global/mainglobalvar.h"

QT_BEGIN_NAMESPACE
namespace Ui { class SubFilesConvertWidgetClass; };
QT_END_NAMESPACE

class SubFilesConvertWidget : public QWidget
{
	Q_OBJECT

public:
	SubFilesConvertWidget(QWidget *parent = nullptr);
	~SubFilesConvertWidget();

protected:
	void closeEvent(QCloseEvent* event) override;

private slots:
	void saveSettings();
	void loadSettings();
	void wirteRemindInfo(QString htmlString);
	void openVideoFile();
	// 选择视频输出文件路径
	void openVideoOutputDir();
	// 开始转换
	void beginToConvertVideo();
	// 接收子线程信息
	void receiveVideoConvertInfo(QString state);
	

public: signals:
	void widgetClosed();
	void sendStateInfo(QString state);
	// 传递视频转换参数
	void sendVideoConvertParams(const QString& input_file, const QString& output_path, const QString& original_type, const QString& target_type);

private:
	Ui::SubFilesConvertWidgetClass *ui;
};
