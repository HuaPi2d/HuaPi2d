#include "checkthreadstate.h"


// ����߳��Ƿ���������
// -1 �̲߳�����
// 0 �߳�δ������
// 1 �߳���������
int checkThreadRunningState(QThread* thread)
{
	if (thread == nullptr) {
		return -1;
	}
	else {
		if (thread->isRunning() == false) {
			return 0;
		}
		else{
			return 1;
		}
	}
}
