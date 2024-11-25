#include "initializegamethread.h"

InitializeGameThread::InitializeGameThread(QObject *parent)
    : BasicScriptThread{parent}
{}

void InitializeGameThread::receiveParams(QString m_taskName)
{
    taskName = m_taskName;
}

void InitializeGameThread::run()
{
    SSJJRunState res = initiallizeGameScreen(taskName);
    emit sendStates(res);
    if (res.errorType == "NoError")
    {
        emit threadFinished();
    }
    QThread::msleep(1000);
    /* 返回初始化结果 */
    this->deleteLater();
}
