#ifndef CHECKTHREADSTATE_H
#define CHECKTHREADSTATE_H

#include <QThread>

// 检查线程是否正在运行
// -1 线程不存在
// 0 线程未在运行
// 1 线程正在运行
int checkThreadRunningState(QThread* thread);

#endif
