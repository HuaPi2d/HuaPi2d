#ifndef MOUSEKEYBORADCONTROL_H
#define MOUSEKEYBORADCONTROL_H

#include <QApplication>
#include <QCursor>
#include <windows.h>
#include <QString>
#include <QThread>

// 模拟鼠标移动
void MoveMouse(int x, int y);

// 模拟鼠标左键点击
void LeftClick();
void LeftDown();
void LeftUp();

// 模拟鼠标右键点击
void RightClick();

// 模拟鼠标中键点击
void MiddleClick();


// 模拟鼠标滚轮滚动
void ScrollMouse(int amount);

// 在指定位置点击鼠标左键
void mouseClick(int x, int y);

// 模拟键盘操作
void keyPress(BYTE virtualKeyCode, int interval = 50);
void keyRelease(BYTE virtualKeyCode);
void keyDown(BYTE virtualKeyCode);

#endif // MOUSEKEYBORADCONTROL_H
