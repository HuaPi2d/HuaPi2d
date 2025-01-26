#ifndef RUNSCRIPTTHREAD_H
#define RUNSCRIPTTHREAD_H

#include <QThread>
#include <QProcess>
#include "ssjjCore/ssjjCoreFun.h"
#include "thread/script/basic/basicscriptthread.h"

class RunScriptThread : public BasicScriptThread
{
    Q_OBJECT
public:
    explicit RunScriptThread(QObject *parent = nullptr);

private:
    SingleTask task;
    int speed;

public slots:
    void receiveParams(SingleTask m_task, int m_speed);

protected:
    void run() override;

public: signals:
    void sendStates(SSJJRunState ssjjRunState);
    void threadFinished();
};

#endif // RUNSCRIPTTHREAD_H
