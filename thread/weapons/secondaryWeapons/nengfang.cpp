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
	// ���� 2 ��
	keyPress(0x32);
	// ��ȡ��Ļ�ֱ���
	cv::Point screeenSize = getScreenResolution();
	// �������
	MoveMouse(screeenSize.x / 2, screeenSize.y);
	// ��������������
	for (int i = 0; i < 2; i++) {
		QThread::msleep(200);
		RightClick();
		QThread::msleep(100);
	}
	// ����ƻ�ԭλ
	MoveMouse(screeenSize.x / 2, 1);
}
