#include "settlementthread.h"

SettlementThread::SettlementThread(QObject *parent)
    : BasicScriptThread{parent}
{}

void SettlementThread::receiveParams(SingleTask task)
{
    m_task = task;
}

void SettlementThread::run()
{
    SSJJRunState res = settlement(m_task);
    emit sendStates(res);
    if (res.errorType == "Success")
    {
        emit threadFinished();
    }
    QThread::msleep(1000);
    /* 返回初始化结果 */
    this->deleteLater();
}
