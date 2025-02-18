#pragma once

#include <QList>
#include <QFileInfo>
#include <QFile>
#include <QDir>
#include <iostream>
#include <fstream>
#include <string>
#include <windows.h>
#include <propsys.h> // IPropertyStore, PropVariant
#include <propkey.h> // Property keys (e.g., PKEY_Title)
#include <shlobj.h>
#include <QMap>
#include <QRegularExpression>


// 文件属性结构体
struct FileAttribute {
	QString name;
	QString value;
};


// 读取文件属性(Qt接口)
//QList<FileAttribute> readFileAttributes(const QString& fullFilePath);

//QList<FileAttribute> readFileAttributes(const QString& filePath, const QString& fullFileName);

//QList<FileAttribute> readFileAttributes(const QFile &file);

QMap<QString, QString> readFileAttributesMap(const QString& fullFilePath);

QList<FileAttribute> readZSCPFileAttributes(const std::wstring fullFilePath);

// 为文件写入属性(Qt接口)
void writeFileAttributes(const QString& fullFilePath, const QList<FileAttribute>& attributes);

void writeFileAttributes(const QString& fullFilePath, const QMap<QString, QString>& attributes);

// 检查扩展属性是否存在
bool checkExtAttribute(const wchar_t* fullFilePath, const wchar_t* attributeName);

// 删除扩展属性
bool deleteExtAttribute(const wchar_t* fullFilePath, const wchar_t* attributeName);

// 读取扩展属性
std::wstring readExtAttribute(const wchar_t* fullFilePath, const wchar_t* attributeName);

// 写入扩展属性
bool writeExtAttribute(const wchar_t* fullFilePath, const wchar_t* attributeName, const wchar_t* content);

bool saveStringFile(QFileInfo fileInfo, QString content);
