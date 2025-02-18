#include "scplanfunction.h"

double convertMoveTime(double time, int scriptMoveSpeed, int userMoveSpeed)
{
	return time * (scriptMoveSpeed + 635) / (userMoveSpeed + 635);
}

void move_back(int time)
{
	keyDown(KEY_S);
	QThread::msleep(time);
	keyRelease(KEY_S);
}

void move_forward(int time)
{
	keyDown(KEY_W);
	QThread::msleep(time);
	keyRelease(KEY_W);
}

void move_left(int time)
{
	keyDown(KEY_A);
	QThread::msleep(time);
	keyRelease(KEY_A);
}

void move_right(int time)
{
	keyDown(KEY_D);
	QThread::msleep(time);
	keyRelease(KEY_D);
}

QString move(QString direction, int time)
{
	if (direction == "W" || direction == "w") {
		move_forward(time);
		return QString();
	}
	else if (direction == "S" || direction == "s") {
		move_back(time);
		return QString();
	}
	else if (direction == "A" || direction == "a") {
		move_left(time);
		return QString();
	}
	else if (direction == "D" || direction == "d") {
		move_right(time);
		return QString();
	}
	// 不存在的方向
	return "unknown direction: " + direction;
}

QString turn_around(int angle, bool isInGame)
{
	cv::Mat oldScreen = captureScreen(45, 125, 185, 265);
	// 保存图片
	//cv::imwrite("oldScreen.png", oldScreen);
	cv::Point resolution = getScreenResolution();
	int x = resolution.x / 2;
	int y = resolution.y / 2;
	if (x - 20 * angle < 0 || x + 20 * angle > resolution.x) {
		return QString("turn around angle %1 is out of screen").arg(angle);
	}
	MoveMouse(x - 20 * angle, y);
	QThread::msleep(100);
	return QString();
}

QString turn_up_down(int angle, bool isInGame)
{
	cv::Mat oldScreen = captureScreen(45, 125, 185, 265);
	cv::Point resolution = getScreenResolution();
	int x = resolution.x / 2;
	int y = resolution.y / 2;
	if (y - 20 * angle < 0 || y + 20 * angle > resolution.y) {
		return QString("turn up/down angle %1 is out of screen").arg(angle);
	}
	MoveMouse(x, y - 20 * angle);
	QThread::msleep(100);
	return QString();
}

QString move_mouse_to(int x, int y)
{
	cv::Point resolution = getScreenResolution();
	if (x < 0 || x > resolution.x || y < 0 || y > resolution.y) {
		return QString("move mouse to (%1, %2) where is out of screen").arg(x).arg(y);
	}
	MoveMouse(x, y);
	return QString();
}

void random_move_mouse()
{
	cv::Point resolution = getScreenResolution();
	int x = rand() % resolution.x;
	int y = rand() % resolution.y;
	MoveMouse(x, y);
}

QString left_click(int x, int y)
{
	QString res = move_mouse_to(x, y);
	if (res != QString()) {
		return res;
	}
	else {
		LeftClick();
		return QString();
	}
}

void left_click()
{
	LeftClick();
}

QString left_down(int x, int y)
{
	QString res = move_mouse_to(x, y);
	if (res != QString()) {
		return res;
	}
	else {
		LeftDown();
		return QString();
	}
}

void left_down()
{
	LeftDown();
}

QString left_up(int x, int y)
{
	QString res = move_mouse_to(x, y);
	if (res != QString()) {
		return res;
	}
	else {
		LeftUp();
		return QString();
	}
}

void left_up()
{
	LeftUp();
}

QString right_click(int x, int y)
{
	QString res = move_mouse_to(x, y);
	if (res != QString()) {
		return res;
	}
	else {
		RightClick();
		return QString();
	}
}

void right_click()
{
	RightClick();
}

QString right_down(int x, int y)
{
	QString res = move_mouse_to(x, y);
	if (res != QString()) {
		return res;
	}
	else {
		RightDown();
		return QString();
	}
}

void right_down()
{
	RightDown();
}

QString right_up(int x, int y)
{
	QString res = move_mouse_to(x, y);
	if (res != QString()) {
		return res;
	}
	else {
		RightUp();
		return QString();
	}
}

void right_up()
{
	RightUp();
}

QString middle_click(int x, int y)
{
	QString res = move_mouse_to(x, y);
	if (res != QString()) {
		return res;
	}
	else {
		MiddleClick();
		return QString();
	}
}

void middle_click()
{
	MiddleClick();
}

QString middle_down(int x, int y)
{
	QString res = move_mouse_to(x, y);
	if (res != QString()) {
		return res;
	}
	else {
		MiddleDown();
		return QString();
	}
}

void middle_down()
{
	MiddleDown();
}

QString middle_up(int x, int y)
{
	QString res = move_mouse_to(x, y);
	if (res != QString()) {
		return res;
	}
	else {
		MiddleUp();
		return QString();
	}
}

void middle_up()
{
	MiddleUp();
}

void sleep_ms(int ms)
{
	QThread::msleep(ms);
}

void sleep_s(int s)
{
	QThread::msleep(s * 1000);
}

void key_press(QString key)
{
	keyPress(key);
}

void key_up(QString key)
{
	keyRelease(key);
}

void key_down(QString key)
{
	keyDown(key);
}

