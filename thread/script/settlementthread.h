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
    QString taskName;

public slots:
    void receiveParams(QString m_taskName);

protected:
    void run() override;

public: signals:
    void sendStates(SSJJRunState ssjjRunState);
    void threadFinished();
};

#endif // SETTLEMENTTHREAD_H
