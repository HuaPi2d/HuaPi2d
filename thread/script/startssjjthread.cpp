#include "startssjjthread.h"

StartSSJJThread::StartSSJJThread(QObject *parent)
    : BasicScriptThread{parent}
{}

void StartSSJJThread::receiveParams(QString m_ssjjInstallPath)
{
    ssjjInstallPath = m_ssjjInstallPath;
}

void StartSSJJThread::run()
{
    SSJJRunState state = restartSSJJ(ssjjInstallPath);
    emit sendStates(state);
    if (state.errorType == "NoError") {
        emit threadFinished();
    }
    // 发送信号
    this->deleteLater();
}
