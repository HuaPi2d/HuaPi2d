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
    // 确保源文件存在（对于资源文件需要使用特殊路径）
    QFile sourceFile(source);
    if (!sourceFile.exists()) {
        qWarning() << "Source file does not exist:" << source;
        return;
    }

    // 检查目标路径是否与源路径相同
    if (source == destination) {
        qDebug() << "source:" << source << "\ndestination:" << destination;
        qWarning() << "Source and destination paths are identical";
        return;
    }

    // 创建目标目录（如果不存在）
    createDirectoryIfNotExists(QFileInfo(destination).absolutePath());

    // 如果目标文件已存在，先删除它（因为QFile::copy不能覆盖已有文件）
    if (QFile::exists(destination) && !QFile::remove(destination)) {
        qWarning() << "Failed to remove existing destination file:" << destination;
        return;
    }

    // 对于资源文件，需要使用QFile的特殊处理
    if (source.startsWith(":/")) {
        // 资源文件需要以只读方式打开并手动复制内容
        if (!sourceFile.open(QIODevice::ReadOnly)) {
            qWarning() << "Failed to open resource file:" << source;
            return;
        }

        QFile destFile(destination);
        if (!destFile.open(QIODevice::WriteOnly)) {
            sourceFile.close();
            qWarning() << "Failed to open destination file for writing:" << destination;
            return;
        }

        // 复制文件内容
        destFile.write(sourceFile.readAll());

        // 关闭文件
        sourceFile.close();
        destFile.close();
    }
    else {
        // 普通文件可以直接使用QFile::copy
        if (!QFile::copy(source, destination)) {
            qWarning() << "Failed to copy file from" << source << "to" << destination;
        }
    }
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
