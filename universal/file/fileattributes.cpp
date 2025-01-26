#include "fileattributes.h"

// 读取文件属性函数（根据文件后缀实现不同的读取功能）
QList<FileAttribute> readFileAttributes(const QString& fullFilePath) {
    std::wstring fullFilePathStd = fullFilePath.toStdWString();
    QFileInfo fileInfo(fullFilePath);
    QString suffix = fileInfo.suffix();
    if (suffix == "zscp") {
        return readZSCPFileAttributes(fullFilePathStd);
    }

    return QList<FileAttribute>();
}

// 重载函数：通过文件路径和文件名组合读取属性
QList<FileAttribute> readFileAttributes(const QString& filePath, const QString& fullFileName) {
    QString fullFilePath = filePath + "/" + fullFileName;
    return readFileAttributes(fullFilePath);
}

// 重载函数：通过 QFile 对象读取属性
QList<FileAttribute> readFileAttributes(const QFile& file) {
    return readFileAttributes(file.fileName());
}

QList<FileAttribute> readZSCPFileAttributes(const std::wstring fullFilePath)
{
    FileAttribute zscp_chapter = {
        "chapter",
        QString::fromStdWString(readExtAttribute(fullFilePath.c_str(), L"chapter"))
    };
    FileAttribute zscp_level = {
        "level",
        QString::fromStdWString(readExtAttribute(fullFilePath.c_str(), L"level"))
    };
    FileAttribute zscp_difficulty = {
        "difficulty",
        QString::fromStdWString(readExtAttribute(fullFilePath.c_str(), L"difficulty"))
    };
    FileAttribute zscp_speed = {
        "speed",
        QString::fromStdWString(readExtAttribute(fullFilePath.c_str(), L"speed"))
    };

    QList<FileAttribute> attributes;
    attributes.append(zscp_chapter);
    attributes.append(zscp_level);
    attributes.append(zscp_difficulty);
    attributes.append(zscp_speed);

    return attributes;
}

void writeFileAttributes(const QString& fullFilePath, const QList<FileAttribute>& attributes)
{
    std::wstring fullFilePathStd = fullFilePath.toStdWString();
    for (const FileAttribute& attribute : attributes)
    {
        writeExtAttribute(fullFilePathStd.c_str(), attribute.name.toStdWString().c_str(), attribute.value.toStdWString().c_str());
    }
}

// 检查扩展属性是否存在
bool checkExtAttribute(const wchar_t* fullFilePath, const wchar_t* attributeName) {
    if (fullFilePath == nullptr || attributeName == nullptr) {
        std::wcerr << L"Invalid input parameters.\n";
        return false;
    }

    wchar_t full_path[512];
    _snwprintf(full_path, sizeof(full_path) / sizeof(wchar_t), L"%s:%s", fullFilePath, attributeName);

    // 打开扩展属性文件流
    HANDLE file = CreateFileW(
        full_path,
        GENERIC_READ,
        0,
        NULL,
        OPEN_EXISTING,
        FILE_ATTRIBUTE_NORMAL,
        NULL
    );

    if (file == INVALID_HANDLE_VALUE) {
        std::wcerr << L"Attribute not found: " << attributeName << L"\n";
        return false;
    }

    CloseHandle(file);
    // std::wcout << L"Attribute found: " << attributeName << L"\n";
    return true;
}

// 删除扩展属性
bool deleteExtAttribute(const wchar_t* fullFilePath, const wchar_t* attributeName) {
    if (fullFilePath == nullptr || attributeName == nullptr) {
        std::wcerr << L"Invalid input parameters.\n";
        return false;
    }

    wchar_t full_path[512];
    _snwprintf(full_path, sizeof(full_path) / sizeof(wchar_t), L"%s:%s", fullFilePath, attributeName);

    if (checkExtAttribute(fullFilePath, attributeName)) {
        // 使用 DeleteFileW 删除扩展属性
        if (!DeleteFileW(full_path)) {
            DWORD errorCode = GetLastError();
            std::wcerr << L"Failed to delete attribute with error code: " << errorCode << L"\n";
            return false;
        }
    }

    std::wcout << L"Attribute deleted successfully.\n";
    return true;
}

// 读取扩展属性
std::wstring readExtAttribute(const wchar_t* fullFilePath, const wchar_t* attributeName) {
    if (fullFilePath == nullptr || attributeName == nullptr) {
        std::wcerr << L"Invalid input parameters.\n";
        return L"";
    }

    wchar_t full_path[512];
    _snwprintf(full_path, sizeof(full_path) / sizeof(wchar_t), L"%s:%s", fullFilePath, attributeName);

    HANDLE file = CreateFileW(
        full_path,
        GENERIC_READ,
        0,
        NULL,
        OPEN_EXISTING,
        FILE_ATTRIBUTE_NORMAL,
        NULL
    );

    if (file == INVALID_HANDLE_VALUE) {
        std::wcerr << L"Attribute not found: " << attributeName << L"\n";
        return L"";
    }

    wchar_t buffer[1024] = { 0 };
    DWORD bytesRead;
    if (!ReadFile(file, buffer, sizeof(buffer) - sizeof(wchar_t), &bytesRead, NULL)) {
        DWORD errorCode = GetLastError();
        std::wcerr << L"Failed to read attribute with error code: " << errorCode << L"\n";
        CloseHandle(file);
        return L"";
    }

    CloseHandle(file);
    return std::wstring(buffer, bytesRead / sizeof(wchar_t));
}

// 写入扩展属性
bool writeExtAttribute(const wchar_t* fullFilePath, const wchar_t* attributeName, const wchar_t* content) {
    if (fullFilePath == nullptr || attributeName == nullptr || content == nullptr) {
        std::wcerr << L"Invalid input parameters.\n";
        return false;
    }

    wchar_t full_path[512];
    _snwprintf(full_path, sizeof(full_path) / sizeof(wchar_t), L"%s:%s", fullFilePath, attributeName);

    HANDLE file = CreateFileW(
        full_path,
        GENERIC_WRITE,
        0,
        NULL,
        CREATE_ALWAYS,
        FILE_ATTRIBUTE_NORMAL,
        NULL
    );

    if (file == INVALID_HANDLE_VALUE) {
        DWORD errorCode = GetLastError();
        std::wcerr << L"Failed to open attribute file with error code: " << errorCode << L"\n";
        return false;
    }

    DWORD bytesWritten;
    if (!WriteFile(file, content, wcslen(content) * sizeof(wchar_t), &bytesWritten, NULL)) {
        DWORD errorCode = GetLastError();
        std::wcerr << L"Failed to write attribute content with error code: " << errorCode << L"\n";
        CloseHandle(file);
        return false;
    }

    CloseHandle(file);
    // std::wcout << L"Attribute written successfully.\n";
    return true;
}

bool saveStringFile(QFileInfo fileInfo, QString content)
{
    QDir dir = fileInfo.absoluteDir();
    if (!fileInfo.absoluteDir().exists())
    {
        // 创建目录
        if (!dir.mkpath("."))
        {
            std::wcerr << L"Failed to create directory: " << dir.absolutePath().toStdWString() << L"\n";
            return false;
        }
    }

    QFile file(fileInfo.absoluteFilePath());
    if (!file.open(QIODevice::WriteOnly))
    {
        std::wcerr << L"Failed to open file for writing: " << fileInfo.absoluteFilePath().toStdWString() << L"\n";
        return false;
    }

    QTextStream out(&file);
    out << content;
    file.close();

    return true;
}

