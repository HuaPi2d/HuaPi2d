#include "liexi.h"

LieXi::LieXi(QObject *parent)
	: SingleWeaponThread(parent)
{
	name = "LieXi";
	times = 5;
}

LieXi::~LieXi()
{}

void LieXi::run()
{
	qDebug() << name;
	// 按下1键
	keyPress(0x31);
	// 获取屏幕分辨率
	cv::Point screeenSize = getScreenResolution();
	// 鼠标下移
	MoveMouse(screeenSize.x / 2, screeenSize.y);
	QThread::msleep(100);
	MoveMouse(screeenSize.x / 2, screeenSize.y);
	QThread::msleep(100);
	// 按下鼠标右键
	RightClick();
	QThread::msleep(100);
	// 鼠标移回原位
	MoveMouse(screeenSize.x / 2, 1);
	QThread::msleep(100);
	MoveMouse(screeenSize.x / 2, 360);
}
