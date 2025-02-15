#ifndef SCPLANFUNCTION_H
#define SCPLANFUNCTION_H

#include <QThread>

#include "universal/script/simulate/mouseKeyboradControl.h"
#include "universal/script/image/snapShoot.h"

// 移动函数

// 移动时间换算
double convertMoveTime(double time, int scriptMoveSpeed, int userMoveSpeed);

// 向不同方向移动
void move_back(int time);
void move_forward(int time);
void move_left(int time);
void move_right(int time);
QString move(QString direction, int time);

// 视角转动
QString turn_around(int angle, bool isInGame);
QString turn_up_down(int angle, bool isInGame);

// 鼠标移动
QString move_mouse_to(int x, int y);
void random_move_mouse();

// 鼠标按键
QString left_click(int x, int y);
void left_click();
QString left_down(int x, int y);
void left_down();
QString left_up(int x, int y);
void left_up();
QString right_click(int x, int y);
void right_click();
QString right_down(int x, int y);
void right_down();
QString right_up(int x, int y);
void right_up();
QString middle_click(int x, int y);
void middle_click();
QString middle_down(int x, int y);
void middle_down();
QString middle_up(int x, int y);
void middle_up();

// 延时
void sleep_ms(int ms);
void sleep_s(int s);

// 键盘按键
void key_press(QString key);
void key_up(QString key);
void key_down(QString key);


#endif // SCPLANFUNCTION_H
