#ifndef INITIALIZEGAMETHREAD_H
#define INITIALIZEGAMETHREAD_H

#include <QThread>

#include "ssjjCore/ssjjCoreFun.h"
#include "thread/script/basic/basicscriptthread.h"

class InitializeGameThread : public BasicScriptThread
{
    Q_OBJECT
public:
    explicit InitializeGameThread(QObject *parent = nullptr);

private:
    QString taskName;

public slots:
    void receiveParams(QString m_taskName);

protected:
    void run() override;

public: signals:
    void sendStates(SSJJRunState ssjjRunState);
    void threadFinished();
};

#endif // INITIALIZEGAMETHREAD_H
