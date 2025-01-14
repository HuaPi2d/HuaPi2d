#include "mouseKeyboradControl.h"

void MoveMouse(int x, int y) {
    INPUT input = { 0 };
    input.type = INPUT_MOUSE;
    input.mi.dx = (x * 65535) / GetSystemMetrics(SM_CXSCREEN); // ת��Ϊ��Ļ����
    input.mi.dy = (y * 65535) / GetSystemMetrics(SM_CYSCREEN); // ת��Ϊ��Ļ����
    input.mi.dwFlags = MOUSEEVENTF_MOVE | MOUSEEVENTF_ABSOLUTE;

    SendInput(1, &input, sizeof(INPUT));
}

void LeftClick() {
    INPUT input[2] = { 0 };

    // �������
    input[0].type = INPUT_MOUSE;
    input[0].mi.dwFlags = MOUSEEVENTF_LEFTDOWN;

    // �ɿ����
    input[1].type = INPUT_MOUSE;
    input[1].mi.dwFlags = MOUSEEVENTF_LEFTUP;

    SendInput(2, input, sizeof(INPUT));
}

void LeftDown()
{
    INPUT input[1] = { 0 };

    // �������
    input[0].type = INPUT_MOUSE;
    input[0].mi.dwFlags = MOUSEEVENTF_LEFTDOWN;

    SendInput(1, input, sizeof(INPUT));
}

void LeftUp()
{
    INPUT input[1] = { 0 };

    // �ɿ����
    input[0].type = INPUT_MOUSE;
    input[0].mi.dwFlags = MOUSEEVENTF_LEFTUP;

    SendInput(1, input, sizeof(INPUT));
}

void RightClick() {
    INPUT input[2] = { 0 };

    // �����Ҽ�
    input[0].type = INPUT_MOUSE;
    input[0].mi.dwFlags = MOUSEEVENTF_RIGHTDOWN;

    // �ɿ��Ҽ�
    input[1].type = INPUT_MOUSE;
    input[1].mi.dwFlags = MOUSEEVENTF_RIGHTUP;

    SendInput(2, input, sizeof(INPUT));
}

void MiddleClick() {
    INPUT input[2] = { 0 };

    // �����м�
    input[0].type = INPUT_MOUSE;
    input[0].mi.dwFlags = MOUSEEVENTF_MIDDLEDOWN;

    // �ɿ��м�
    input[1].type = INPUT_MOUSE;
    input[1].mi.dwFlags = MOUSEEVENTF_MIDDLEUP;

    SendInput(2, input, sizeof(INPUT));
}

void ScrollMouse(int amount) 
{
    INPUT input = { 0 };
    input.type = INPUT_MOUSE;
    input.mi.dwFlags = MOUSEEVENTF_WHEEL;
    input.mi.mouseData = amount;  // ��ֵ���Ϲ�������ֵ���¹���

    SendInput(1, &input, sizeof(INPUT));
}

void mouseClick(int x, int y) 
{
    MoveMouse(x, y);
    LeftClick();
    MoveMouse(1, 1);
}

void keyPress(BYTE virtualKeyCode, int interval) 
{
    keyDown(virtualKeyCode);
    QThread::msleep(interval);
    keyRelease(virtualKeyCode);
}

void keyRelease(BYTE virtualKeyCode)
{
    keybd_event(virtualKeyCode, 0, 2, 0);
}

void keyDown(BYTE virtualKeyCode)
{
    keybd_event(virtualKeyCode, 0, 0, 0);
}

void inputString(QString str)
{
    for (int i = 0; i < str.length(); i++) {
        keyPress(str[i].toLatin1(), 10);
    }
}
