#include "taikesix.h"

TaiKeSiX::TaiKeSiX(QObject *parent)
	: QThread(parent)
{
	name = "TaiKeSiX";
	times = 45;
}

TaiKeSiX::~TaiKeSiX()
{}

void TaiKeSiX::run()
{
	qDebug() << name;
	// ���� X ��
	keyPress(0x58);
	// �ȴ������ͷ�ʱ�� 3s
	QThread::msleep(3000);
}
