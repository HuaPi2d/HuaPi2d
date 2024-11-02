#pragma once

#include <QThread>
#include <QProcess>
#include <QDebug>
#include <QFileInfo>
#include <QRegularExpression>

#include "universal/media/ffmpegpro.h"

class VideoFileConvertThread  : public QThread
{
	Q_OBJECT

public:
	VideoFileConvertThread(QObject *parent);
	~VideoFileConvertThread();

private:
	QString input_file;
	QString output_path;
	QString output_file;
	QString original_type;
	QString target_type;
	QProcess *ffmpegProcess;

private slots:
	void getVideoTime();
	void convertAviToMp4();

protected:
	void run() override;

public slots:
	// 接收参数
	void receiveParams(const QString& m_input_file, const QString& m_output_path, const QString& m_original_type, const QString& m_target_type);

public: signals:
	void signal_start();
	void sendStateInfo(QString state_info);
	void sendProgressInfo(int hours, int minutes, int seconds);
	void sendTimeInfo(int hours, int minutes, int seconds);
};
