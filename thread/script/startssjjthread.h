#ifndef STARTSSJJTHREAD_H
#define STARTSSJJTHREAD_H

#include <QThread>
#include <QErrorMessage>

#include "ssjjCore/ssjjCoreFun.h"
#include "thread/script/basic/basicscriptthread.h"

class StartSSJJThread : public BasicScriptThread
{
    Q_OBJECT
public:
    explicit StartSSJJThread(QObject *parent = nullptr);

private:
    QString ssjjInstallPath;

public slots:
    void receiveParams(QString m_ssjjInstallPath);

protected:
    void run() override;

public: signals:
    void sendStates(SSJJRunState ssjjRunState);
    void threadFinished();
};

#endif // STARTSSJJTHREAD_H
