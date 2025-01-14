#include "initializegamethread.h"

InitializeGameThread::InitializeGameThread(QObject *parent)
    : BasicScriptThread{parent}
{}

void InitializeGameThread::receiveParams(SingleTask task)
{
    m_task = task;
}

void InitializeGameThread::run()
{
    SSJJRunState res = initiallizeGameScreen(m_task);
    emit sendStates(res);
    if (res.errorType == "NoError")
    {
        emit threadFinished();
    }
    QThread::msleep(1000);
    /* 返回初始化结果 */
    this->deleteLater();
}
