#include "ffmpegpro.h"

void convertAviToMp4(const QString& aviFilePath, const QString& mp4FilePath) {
    QProcess ffmpegProcess;

    // FFmpeg 命令行参数，指定输入文件和输出文件
    QStringList arguments;

    arguments << "-i" << aviFilePath  // 输入 AVI 文件
        << "-c:v" << "libx264"  // 使用 H.264 视频编解码器
        << "-c:a" << "aac"      // 使用 AAC 音频编解码器
        << "-strict" << "experimental" // 使用 AAC 时可能需要
        << "-b:a" << "192k"     // 设置音频比特率
        << "-b:v" << "1000k"    // 设置视频比特率（可以调整）
        << "-y"                 // 覆盖输出文件
        << mp4FilePath;         // 输出 MP4 文件路径

    // 启动 FFmpeg 进程
    ffmpegProcess.start("ffmpeg", arguments);

    // 等待进程结束
    if (ffmpegProcess.waitForFinished(6000000)) {
        qDebug() << "Conversion finished successfully!";
    }
    else {
        qDebug() << "Error during conversion: " << ffmpegProcess.errorString();
    }
}
