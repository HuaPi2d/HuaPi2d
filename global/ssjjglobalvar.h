#pragma once
#ifndef SSJJGLOBALVAR_H
#define SSJJGLOBALVAR_H

#include <QString>
#include <QThread>
#include <QLabel>
#include <QPoint>
#include "global/basicGlobalVar/globalvariableqstring.h"

extern QString resolutionPath;
extern bool isRunScriptThreadRunning;
extern GlobalVariableQString* textToShowInScreen;
// 密码模式
// 1: 固定密码
// 2: 日期密码
// 3: 无密码
extern int passWordMode;
extern QString enterGamePassWord;
// 屏幕中心点
extern QPoint centerPoint;

#endif // SSJJGLOBALVAR_H
