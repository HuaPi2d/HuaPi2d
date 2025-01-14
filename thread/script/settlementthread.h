#ifndef SETTLEMENTTHREAD_H
#define SETTLEMENTTHREAD_H

#include <QThread>
#include "ssjjCore/ssjjCoreFun.h"
#include "thread/script/basic/basicscriptthread.h"

class SettlementThread : public BasicScriptThread
{
    Q_OBJECT
public:
    explicit SettlementThread(QObject *parent = nullptr);

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

#endif // SETTLEMENTTHREAD_H
