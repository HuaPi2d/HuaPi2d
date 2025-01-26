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
