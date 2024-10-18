#include "videofileconvertthread.h"

VideoFileConvertThread::VideoFileConvertThread(QObject *parent)
	: QThread(parent)
{

}

VideoFileConvertThread::~VideoFileConvertThread()
{}

void VideoFileConvertThread::getVideoTime()
{
	// 初始化 QProcess
	ffmpegProcess = new QProcess(this);

	// 连接信号槽，实时读取标准输出
	connect(ffmpegProcess, &QProcess::readyReadStandardError, [=]() {
		QByteArray output = ffmpegProcess->readAllStandardError();
		QString outputStr = QString(output);

		QRegularExpression re("Duration: (\\d{2}):(\\d{2}):(\\d{2}\\.\\d{2})");
		QRegularExpressionMatch match = re.match(output);
		
		if (match.hasMatch()) {
			int hours = match.captured(1).toInt();
			int minutes = match.captured(2).toInt();
			int seconds = int(match.captured(3).toDouble());
			emit sendTimeInfo(hours, minutes, seconds);
		}
	});

	// FFprobe 命令行参数
	QStringList arguments;
	arguments << "-i" << input_file;

	// 启动 FFprobe 进程
	ffmpegProcess->start("ffmpeg", arguments);

	// 检查进程是否正常启动
	if (ffmpegProcess->waitForFinished()) {
		qDebug() << "Time information obtained successfully!";
	}
	else {
		qDebug() << "Error during time information obtaining: " << ffmpegProcess->errorString();
	}

	ffmpegProcess->deleteLater();
}

void VideoFileConvertThread::convertAviToMp4()
{
	// 初始化 QProcess
	ffmpegProcess = new QProcess(this);
	QRegularExpression re("time=(\\d{2}):(\\d{2}):(\\d{2}\\.\\d{2})");

	// 连接信号槽，实时读取标准错误输出
	connect(ffmpegProcess, &QProcess::readyReadStandardError, [=]() {
		QByteArray output = ffmpegProcess->readAllStandardError();
		QString outputStr = QString(output);
		qDebug() << outputStr;
		
		QRegularExpressionMatch match = re.match(output);

		if (match.hasMatch()) {
			int hours = match.captured(1).toInt();
			int minutes = match.captured(2).toInt();
			int seconds = int(match.captured(3).toDouble());
			emit sendProgressInfo(hours, minutes, seconds);
		}
	});

	// FFmpeg 命令行参数
	QStringList arguments;
	arguments << "-i" << input_file  // 输入 AVI 文件
		<< "-c:v" << "libx264"  // 使用 H.264 视频编解码器
		<< "-c:a" << "aac"      // 使用 AAC 音频编解码器
		<< "-strict" << "experimental" // 使用 AAC 时可能需要
		<< "-b:a" << "192k"     // 设置音频比特率
		<< "-b:v" << "1000k"    // 设置视频比特率（可以调整）
		<< "-y"                 // 覆盖输出文件
		<< output_file;         // 输出 MP4 文件路径

	// 启动 FFmpeg 进程
	ffmpegProcess->start("ffmpeg", arguments);

	// 检查进程是否正常启动
	if (ffmpegProcess->waitForFinished(6000000)) {
		qDebug() << "Conversion finished successfully!";
	}
	else {
		qDebug() << "Error during conversion: " << ffmpegProcess->errorString();
	}

	ffmpegProcess->deleteLater();
}

void VideoFileConvertThread::run()
{
	emit signal_start();
	if (input_file.isEmpty()) {
		emit sendStateInfo("<p><font color='red'>请选择要转换的视频文件</font></p><br>");
		return;
	}
	if (output_path.isEmpty()) {
		emit sendStateInfo("<p><font color='red'>请选择转换后文件路径</font></p><br>");
		return;
	}
	if (original_type == target_type) {
		emit sendStateInfo("<p><font color='red'>转换前后文件类型相同</font></p><br>");
		return;
	}
	emit sendStateInfo("<p><font color='green'>已开始转换...</font></p><br>");
	if (original_type == "avi" && target_type == "mp4") {
		this->getVideoTime();
		this->convertAviToMp4();
	}
	emit sendStateInfo("<p><font color='green'>转换结束</font></p><br>");
}

void VideoFileConvertThread::receiveParams(const QString& m_input_file, const QString& m_output_path, const QString& m_original_type, const QString& m_target_type)
{
	input_file = m_input_file;
	output_path = m_output_path;
	original_type = m_original_type;
	target_type = m_target_type;
	QFileInfo fileInfo(input_file);
	output_file = output_path + "/" + fileInfo.baseName() + "." + target_type;
}
