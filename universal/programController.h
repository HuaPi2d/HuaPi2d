#pragma once

#ifndef PROGRAMCONTROLLER_H
#define PROGRAMCONTROLLER_H
#ifndef _SILENCE_CXX17_CODECVT_HEADER_DEPRECATION_WARNING
#define _SILENCE_CXX17_CODECVT_HEADER_DEPRECATION_WARNING
#endif


#include <windows.h>
#include <QString>
#include <QDir>
#include <tlhelp32.h>
#include <iostream>
#include <codecvt>
#include <locale>
#include <QSettings>
#include <QThread>

// 获取注册表内容
QString getRegDitValue(QString RegDitPath, QString key);

void runProgramAsAdmin(const QString &program, const QStringList &arguments);

// 将宽字符 (WCHAR) 转换为 std::string
std::string ws2s(const std::wstring& wstr);

DWORD findProcessByName(const std::string& processName);

void TerminateProcessByID(DWORD processID);

void TerminateProcessByName(const std::string& processName);

bool TerminateProcessByNameAndCheck(const std::string& processName, int nums = 3);

bool TerminateProcessTree(DWORD processID);

#endif // PROGRAMCONTROLLER_H
