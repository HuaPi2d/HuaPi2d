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
	// ����1��
	keyPress(0x31);
	// ��ȡ��Ļ�ֱ���
	cv::Point screeenSize = getScreenResolution();
	// �������
	MoveMouse(screeenSize.x / 2, screeenSize.y);
	QThread::msleep(100);
	MoveMouse(screeenSize.x / 2, screeenSize.y);
	QThread::msleep(100);
	// ��������Ҽ�
	RightClick();
	QThread::msleep(100);
	// ����ƻ�ԭλ
	MoveMouse(screeenSize.x / 2, 1);
	QThread::msleep(100);
	MoveMouse(screeenSize.x / 2, 360);
}
