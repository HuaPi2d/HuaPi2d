#pragma once
#include <QDir>

void createDirectoryIfNotExists(const QString& folderPath);

void copyFile(const QString& source, const QString& destination);

void copyDirectory(const QString& source, const QString& destination);

