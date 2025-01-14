#pragma once
#ifndef SSJJGLOBALVAR_H
#define SSJJGLOBALVAR_H

#include <QString>
#include <QThread>
#include <QLabel>
#include "global/basicGlobalVar/globalvariableqstring.h"

extern QString resolutionPath;
extern bool isRunScriptThreadRunning;
extern GlobalVariableQString* textToShowInScreen;
// ����ģʽ
// 1: �̶�����
// 2: ��������
// 3: ������
extern int passWordMode;
extern QString enterGamePassWord;

#endif // SSJJGLOBALVAR_H
