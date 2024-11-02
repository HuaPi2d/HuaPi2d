#include "ffmpegpro.h"

void convertAviToMp4(const QString& aviFilePath, const QString& mp4FilePath) {
    QProcess ffmpegProcess;

    // FFmpeg �����в�����ָ�������ļ�������ļ�
    QStringList arguments;

    arguments << "-i" << aviFilePath  // ���� AVI �ļ�
        << "-c:v" << "libx264"  // ʹ�� H.264 ��Ƶ�������
        << "-c:a" << "aac"      // ʹ�� AAC ��Ƶ�������
        << "-strict" << "experimental" // ʹ�� AAC ʱ������Ҫ
        << "-b:a" << "192k"     // ������Ƶ������
        << "-b:v" << "1000k"    // ������Ƶ�����ʣ����Ե�����
        << "-y"                 // ��������ļ�
        << mp4FilePath;         // ��� MP4 �ļ�·��

    // ���� FFmpeg ����
    ffmpegProcess.start("ffmpeg", arguments);

    // �ȴ����̽���
    if (ffmpegProcess.waitForFinished(6000000)) {
        qDebug() << "Conversion finished successfully!";
    }
    else {
        qDebug() << "Error during conversion: " << ffmpegProcess.errorString();
    }
}
