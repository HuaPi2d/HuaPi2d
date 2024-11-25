#include "checkthreadstate.h"


// 检查线程是否正在运行
// -1 线程不存在
// 0 线程未在运行
// 1 线程正在运行
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
