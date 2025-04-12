#include "filepro.h"

void createDirectoryIfNotExists(const QString& folderPath)
{
    QDir dir;

    // 判断文件夹是否存在
    if (!dir.exists(folderPath)) {
        // 尝试创建文件夹，支持多层文件夹的创建
        dir.mkpath(folderPath);
    }
}

// 复制文件
void copyFile(const QString& source, const QString& destination)
{
    if (!QFile::exists(source)) {
        return;
    }
    if (source == destination) {
        return;
    }
    QFile::copy(source, destination);
}

void copyDirectory(const QString& source, const QString& destination)
{
    if (!QDir(source).exists()) {
        return;
    }
    if (source == destination) {
        return;
    }

    QDir sourceDir(source);
    QDir destinationDir(destination);

    // 复制文件夹
    destinationDir.mkpath(destination);

    // 复制文件
    foreach (QFileInfo fileInfo, sourceDir.entryInfoList(QDir::Files)) {
        QString sourceFilePath = fileInfo.absoluteFilePath();
        QString destinationFilePath = destinationDir.filePath(fileInfo.fileName());
        copyFile(sourceFilePath, destinationFilePath);
    }

    // 复制子文件夹
    foreach (QFileInfo fileInfo, sourceDir.entryInfoList(QDir::Dirs | QDir::NoDotAndDotDot)) {
        QString sourceSubDirPath = fileInfo.absoluteFilePath();
        QString destinationSubDirPath = destinationDir.filePath(fileInfo.fileName());
        copyDirectory(sourceSubDirPath, destinationSubDirPath);
    }
}
