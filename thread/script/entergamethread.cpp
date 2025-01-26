#include "entergamethread.h"

EnterGameThread::EnterGameThread(QObject *parent)
    : BasicScriptThread{parent}
{}

EnterGameThread::~EnterGameThread()
{}

void EnterGameThread::receiveParams(SingleTask m_task, int m_loadingTimes)
{
    task = m_task;
    loadingTimes = m_loadingTimes;
}

void EnterGameThread::run()
{
    SSJJRunState res = enterGame(task, loadingTimes);
    emit sendStates(res);
    if (res.errorType == "NoError")
    {
        emit threadFinished();
    }
    QThread::msleep(1000);
    // 发送信号
    this->deleteLater();
}
