#ifndef SCPLANFUNCTION_H
#define SCPLANFUNCTION_H

#include <QThread>

#include "universal/script/simulate/mouseKeyboradControl.h"
#include "universal/script/image/snapShoot.h"

// �ƶ�����

// �ƶ�ʱ�任��
double convertMoveTime(double time, int scriptMoveSpeed, int userMoveSpeed);

// ��ͬ�����ƶ�
void move_back(int time);
void move_forward(int time);
void move_left(int time);
void move_right(int time);
QString move(QString direction, int time);

// �ӽ�ת��
QString turn_around(int angle, bool isInGame);
QString turn_up_down(int angle, bool isInGame);

// ����ƶ�
QString move_mouse_to(int x, int y);
void random_move_mouse();

// ��갴��
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

// ��ʱ
void sleep_ms(int ms);
void sleep_s(int s);

// ���̰���
void key_press(QString key);
void key_up(QString key);
void key_down(QString key);


#endif // SCPLANFUNCTION_H
