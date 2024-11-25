#include "settlementthread.h"

SettlementThread::SettlementThread(QObject *parent)
    : BasicScriptThread{parent}
{}

void SettlementThread::receiveParams(QString m_taskName)
{
    taskName = m_taskName;
}

void SettlementThread::run()
{
    SSJJRunState res = settlement(taskName);
    emit sendStates(res);
    if (res.errorType == "Success")
    {
        emit threadFinished();
    }
    QThread::msleep(1000);
    /* 返回初始化结果 */
    this->deleteLater();
}
