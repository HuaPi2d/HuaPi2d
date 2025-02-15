#include "checkthreadstate.h"


// 检查线程是否正在运行
// -1 线程不存在
// 0 线程未在运行
// 1 线程正在运行
int checkThreadRunningState(QThread* thread)
{
	if (thread == nullptr || thread == new QThread()) {
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

void forceQuitThread(QThread* thread)
{
	if (checkThreadRunningState(thread) == 1)
	{
		qDebug() << "thread is running: " << thread->objectName();
	}
	if (checkThreadRunningState(thread) == 1)
	{
		thread->terminate();
	}
	if (checkThreadRunningState(thread) == 1)
	{
		thread->deleteLater();
	}
	thread = nullptr;
}
