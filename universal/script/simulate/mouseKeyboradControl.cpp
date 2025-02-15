#include "mouseKeyboradControl.h"

void MoveMouse(int x, int y) {
    INPUT input = { 0 };
    input.type = INPUT_MOUSE;
    input.mi.dx = (x * 65535) / GetSystemMetrics(SM_CXSCREEN); // 转换为屏幕坐标
    input.mi.dy = (y * 65535) / GetSystemMetrics(SM_CYSCREEN); // 转换为屏幕坐标
    input.mi.dwFlags = MOUSEEVENTF_MOVE | MOUSEEVENTF_ABSOLUTE;

    SendInput(1, &input, sizeof(INPUT));
}

void LeftClick() {
    INPUT input[2] = { 0 };

    // 按下左键
    input[0].type = INPUT_MOUSE;
    input[0].mi.dwFlags = MOUSEEVENTF_LEFTDOWN;

    // 松开左键
    input[1].type = INPUT_MOUSE;
    input[1].mi.dwFlags = MOUSEEVENTF_LEFTUP;

    SendInput(2, input, sizeof(INPUT));
}

void LeftDown()
{
    INPUT input[1] = { 0 };

    // 按下左键
    input[0].type = INPUT_MOUSE;
    input[0].mi.dwFlags = MOUSEEVENTF_LEFTDOWN;

    SendInput(1, input, sizeof(INPUT));
}

void LeftUp()
{
    INPUT input[1] = { 0 };

    // 松开左键
    input[0].type = INPUT_MOUSE;
    input[0].mi.dwFlags = MOUSEEVENTF_LEFTUP;

    SendInput(1, input, sizeof(INPUT));
}

void RightClick() {
    INPUT input[2] = { 0 };

    // 按下右键
    input[0].type = INPUT_MOUSE;
    input[0].mi.dwFlags = MOUSEEVENTF_RIGHTDOWN;

    // 松开右键
    input[1].type = INPUT_MOUSE;
    input[1].mi.dwFlags = MOUSEEVENTF_RIGHTUP;

    SendInput(2, input, sizeof(INPUT));
}

void RightDown()
{
    INPUT input[1] = { 0 };

    // 按下右键
    input[0].type = INPUT_MOUSE;
    input[0].mi.dwFlags = MOUSEEVENTF_RIGHTDOWN;

    SendInput(1, input, sizeof(INPUT));
}

void RightUp()
{
    INPUT input[1] = { 0 };

    // 松开右键
    input[0].type = INPUT_MOUSE;
    input[0].mi.dwFlags = MOUSEEVENTF_RIGHTUP;

    SendInput(1, input, sizeof(INPUT));
}


void MiddleClick() {
    INPUT input[2] = { 0 };

    // 按下中键
    input[0].type = INPUT_MOUSE;
    input[0].mi.dwFlags = MOUSEEVENTF_MIDDLEDOWN;

    // 松开中键
    input[1].type = INPUT_MOUSE;
    input[1].mi.dwFlags = MOUSEEVENTF_MIDDLEUP;

    SendInput(2, input, sizeof(INPUT));
}

void MiddleDown()
{
    INPUT input[1] = { 0 };

    // 按下中键
    input[0].type = INPUT_MOUSE;
    input[0].mi.dwFlags = MOUSEEVENTF_MIDDLEDOWN;

    SendInput(1, input, sizeof(INPUT));
}

void MiddleUp()
{
    INPUT input[1] = { 0 };

    // 松开中键
    input[0].type = INPUT_MOUSE;
    input[0].mi.dwFlags = MOUSEEVENTF_MIDDLEUP;

    SendInput(1, input, sizeof(INPUT));
}

void ScrollMouse(int amount) 
{
    INPUT input = { 0 };
    input.type = INPUT_MOUSE;
    input.mi.dwFlags = MOUSEEVENTF_WHEEL;
    input.mi.mouseData = amount;  // 正值向上滚动，负值向下滚动

    SendInput(1, &input, sizeof(INPUT));
}

void mouseClick(int x, int y) 
{
    MoveMouse(x, y);
    LeftClick();
    MoveMouse(1, 1);
}

BYTE getVirtualKeyCode(QString key)
{
    if(key == "A" || key == "a")return 0x41;
    if(key == "B" || key == "b")return 0x42;
    if(key == "C" || key == "c")return 0x43;
    if(key == "D" || key == "d")return 0x44;
    if(key == "E" || key == "e")return 0x45;
    if(key == "F" || key == "f")return 0x46;
    if(key == "G" || key == "g")return 0x47;
    if(key == "H" || key == "h")return 0x48;
    if(key == "I" || key == "i")return 0x49;
    if(key == "J" || key == "j")return 0x4A;
    if(key == "K" || key == "k")return 0x4B;
    if(key == "L" || key == "l")return 0x4C;
    if(key == "M" || key == "m")return 0x4D;
    if(key == "N" || key == "n")return 0x4E;
    if(key == "O" || key == "o")return 0x4F;
    if(key == "P" || key == "p")return 0x50;
    if(key == "Q" || key == "q")return 0x51;
    if(key == "R" || key == "r")return 0x52;
    if(key == "S" || key == "s")return 0x53;
    if(key == "T" || key == "t")return 0x54;
    if(key == "U" || key == "u")return 0x55;
    if(key == "V" || key == "v")return 0x56;
    if(key == "W" || key == "w")return 0x57;
    if(key == "X" || key == "x")return 0x58;
    if(key == "Y" || key == "y")return 0x59;
    if(key == "Z" || key == "z")return 0x5A;

    if(key == "1")return 0x31;
    if(key == "2")return 0x32;
    if(key == "3")return 0x33;
    if(key == "4")return 0x34;
    if(key == "5")return 0x35;
    if(key == "6")return 0x36;
    if(key == "7")return 0x37;
    if(key == "8")return 0x38;
    if(key == "9")return 0x39;
    if(key == "0")return 0x30;

    if(key == "LEFT" || key == "left")return 0x25;
    if(key == "RIGHT" || key == "right")return 0x27;
    if(key == "UP" || key == "up")return 0x26;
    if(key == "DOWN" || key == "down")return 0x28;

    if(key == "SPACE" || key == "space")return 0x20;
    if(key == "ENTER" || key == "enter")return 0x0D;
    if(key == "ESC" || key == "esc")return 0x1B;

    if(key == "SHIFT" || key == "shift")return 0x10;
    if(key == "CTRL" || key == "ctrl")return 0x11;
    if(key == "ALT" || key == "alt")return 0x12;

    if(key == "F1")return 0x70;
    if(key == "F2")return 0x71;
    if(key == "F3")return 0x72;
    if(key == "F4")return 0x73;
    if(key == "F5")return 0x74;
    if(key == "F6")return 0x75;
    if(key == "F7")return 0x76;
    if(key == "F8")return 0x77;
    if(key == "F9")return 0x78;
    if(key == "F10")return 0x79;
    if(key == "F11")return 0x7A;
    if(key == "F12")return 0x7B;

    if(key == "HOME" || key == "home")return 0x24;
    if(key == "END" || key == "end")return 0x23;
    if(key == "PAGEUP" || key == "pageup")return 0x21;
    if(key == "PAGEDOWN" || key == "pagedown")return 0x22;

    // 返回 * 的虚拟键码
    return 0x2A;
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
        keyPress(getVirtualKeyCode(str[i]), 10);
    }
}

void keyPress(QString singleKey)
{
    keyPress(getVirtualKeyCode(singleKey));
}

void keyRelease(QString singleKey)
{
    keyRelease(getVirtualKeyCode(singleKey));
}

void keyDown(QString singleKey)
{
    keyDown(getVirtualKeyCode(singleKey));
}
