#ifndef CHECKTHREADSTATE_H
#define CHECKTHREADSTATE_H

#include <QThread>

// ����߳��Ƿ���������
// -1 �̲߳�����
// 0 �߳�δ������
// 1 �߳���������
int checkThreadRunningState(QThread* thread);

#endif
