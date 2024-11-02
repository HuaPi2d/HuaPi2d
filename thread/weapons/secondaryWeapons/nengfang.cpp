#include "nengfang.h"

NengFang::NengFang(QObject *parent)
	: QThread(parent)
{
	name = "NengFang";
	times = 35;
}

NengFang::~NengFang()
{}

void NengFang::run()
{
	qDebug() << name;
	// 按下 2 键
	keyPress(0x32);
	// 获取屏幕分辨率
	cv::Point screeenSize = getScreenResolution();
	// 鼠标下移
	MoveMouse(screeenSize.x / 2, screeenSize.y);
	// 按下鼠标左键两次
	for (int i = 0; i < 2; i++) {
		QThread::msleep(200);
		RightClick();
		QThread::msleep(100);
	}
	// 鼠标移回原位
	MoveMouse(screeenSize.x / 2, 1);
}
