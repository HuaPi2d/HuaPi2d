#ifndef ENTERGAMETHREAD_H
#define ENTERGAMETHREAD_H

#include <QThread>
#include "ssjjCore/ssjjCoreFun.h"
#include "thread/script/basic/basicscriptthread.h"

class EnterGameThread : public BasicScriptThread
{
    Q_OBJECT
public:
    explicit EnterGameThread(QObject *parent = nullptr);
    ~EnterGameThread();

private:
    SingleTask task;
    int loadingTimes;

public slots:
    void receiveParams(SingleTask m_task, int m_loadingTimes);

protected:
    void run() override;

public: signals:
    void sendStates(SSJJRunState ssjjRunState);
    void threadFinished();
};

#endif // ENTERGAMETHREAD_H
