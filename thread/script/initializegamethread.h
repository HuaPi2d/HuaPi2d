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
    SingleTask m_task;

public slots:
    void receiveParams(SingleTask task);

protected:
    void run() override;

public: signals:
    void sendStates(SSJJRunState ssjjRunState);
    void threadFinished();
};

#endif // INITIALIZEGAMETHREAD_H
