#include "ssjjmainthread.h"
#include "global/ssjjglobalvar.h"

#include <QTimer>

SSJJMainThread::SSJJMainThread(QObject *parent)
    : BasicScriptThread{parent}
{
    nextStep = "initializeGameScreen";
    currentThread = nullptr;
    connect(currentThread, &BasicScriptThread::sendDisplayText, this, &SSJJMainThread::receiveDisplayText);
}

SSJJMainThread::~SSJJMainThread()
{}

// 初始化游戏画面
void SSJJMainThread::initializeGameScreen()
{
    /* 初始化游戏画面 */
    InitializeGameThread* initializeGameThread = new InitializeGameThread();
    initializeGameThread->moveToThread(this);
    changeCurrentThread(initializeGameThread);
    connect(this, &SSJJMainThread::sendInitializeGameParams, initializeGameThread, &InitializeGameThread::receiveParams);
    connect(initializeGameThread, &InitializeGameThread::sendStates, this, &SSJJMainThread::receiveInitializeGameStates);
    connect(initializeGameThread, &InitializeGameThread::sendDisplayText, this, &SSJJMainThread::receiveDisplayText);
    connect(initializeGameThread, &InitializeGameThread::threadFinished, this, [=]() {
        initializeGameThread->sendDisplayText("Initialize Success");
        });

    emit sendInitializeGameParams(task);
}

// 重启SSJJ
void SSJJMainThread::restartSSJJ()
{
    StartSSJJThread* startSSJJThread = new StartSSJJThread();
    startSSJJThread->moveToThread(this);
    changeCurrentThread(startSSJJThread);
    connect(this, &SSJJMainThread::sendRestartParams, startSSJJThread, &StartSSJJThread::receiveParams);
    connect(startSSJJThread, &StartSSJJThread::sendStates, this, &SSJJMainThread::receiveRestartSSJJStates);
    connect(startSSJJThread, &StartSSJJThread::sendDisplayText, this, &SSJJMainThread::receiveDisplayText);
    connect(startSSJJThread, &StartSSJJThread::threadFinished, this, [=]() {
        startSSJJThread->sendDisplayText("SSJJ重启成功");
        });

    emit sendRestartParams(ssjjInstallPath);
}

// 进入游戏
void SSJJMainThread::enterGame()
{
    EnterGameThread* enterGameThread = new EnterGameThread();
    enterGameThread->moveToThread(this);
    changeCurrentThread(enterGameThread);
    connect(this, &SSJJMainThread::sendEnterGameParams, enterGameThread, &EnterGameThread::receiveParams);
    connect(enterGameThread, &EnterGameThread::sendStates, this, &SSJJMainThread::receiveEnterGameStates);
    connect(enterGameThread, &EnterGameThread::sendDisplayText, this, &SSJJMainThread::receiveDisplayText);
    connect(enterGameThread, &EnterGameThread::threadFinished, this, [=]() {
        enterGameThread->sendDisplayText("Enter Game Success");
        });

    emit sendEnterGameParams(task, loadingTimes);

    // 设置时间限制
    QTimer::singleShot(1 * 60 * 1000, this, [=]() {
        if (enterGameThread->isRunning())
        {
            nextStep = "restartSSJJ";
            enterGameThread->terminate();
            enterGameThread->deleteLater();
            emit sendRemindInfo("<p>进入游戏超时</p><br>");
        }
        else {
            enterGameThread->terminate();
            enterGameThread->deleteLater();
        }
        });
}

// 运行脚本
void SSJJMainThread::runScript()
{
    // 运行脚本
    RunScriptThread* runScriptThread = new RunScriptThread();
    runScriptThread->moveToThread(this);
    changeCurrentThread(runScriptThread);
    connect(this, &SSJJMainThread::sendRunScriptParams, runScriptThread, &RunScriptThread::receiveParams);
    connect(runScriptThread, &RunScriptThread::sendStates, this, &SSJJMainThread::receiveRunScriptStates);
    connect(runScriptThread, &RunScriptThread::sendDisplayText, this, &SSJJMainThread::receiveDisplayText);
    connect(runScriptThread, &RunScriptThread::threadFinished, this, [=]() {
        runScriptThread->sendDisplayText("Script finished");
        });

    emit sendRunScriptParams(task, moveSpeed);

    // 设置时间限制
    QTimer::singleShot(singleScriptTime * 60 * 1000, this, [=]() {
        if (runScriptThread->isRunning())
        {
            nextStep = "restartSSJJ";
            runScriptThread->terminate();
            emit sendRemindInfo("<p>脚本运行超时</p><br>");
        }
        });
}

// 结算
void SSJJMainThread::settlement()
{
    SettlementThread* settlementThread = new SettlementThread();
    settlementThread->moveToThread(this);
    changeCurrentThread(settlementThread);
    connect(this, &SSJJMainThread::sendSettlementParams, settlementThread, &SettlementThread::receiveParams);
    connect(settlementThread, &SettlementThread::sendStates, this, &SSJJMainThread::receiveSettlementStates);
    connect(settlementThread, &SettlementThread::sendDisplayText, this, &SSJJMainThread::receiveDisplayText);
    connect(settlementThread, &SettlementThread::threadFinished, this, [=]() {
        settlementThread->sendDisplayText("Task finished");
        });

    emit sendSettlementParams(task);
}

// 切换线程
void SSJJMainThread::changeCurrentThread(BasicScriptThread* thread)
{
    currentThread = thread;
}

void SSJJMainThread::runCurrentThread()
{
    currentThread->start();
    currentThread->wait();
}

void SSJJMainThread::run()
{
    /* 开始运行 */
    QThread::msleep(1000);
    if(task.taskName != ""){
        while(true){
            emit sendDisplayText(nextStep);
            if(nextStep == "initializeGameScreen"){
                this->initializeGameScreen();
            }
            else if(nextStep == "enterGame"){
                this->enterGame();
            }
            else if(nextStep == "restartSSJJ"){
                this->restartSSJJ();
            }
            else if(nextStep == "fatalError"){
                emit sendFatalError();
                QThread::msleep(1000);
                break;
            }
            else if(nextStep == "runScriptFile"){
                this->runScript();
            }
            else if(nextStep == "settlement"){
                this->settlement();
            }
            else if(nextStep == "stopThread"){
                break;
            }
            else {
                break;
            }
            runCurrentThread();
        }
    }
}

void SSJJMainThread::receiveTask(SingleTask m_task, QString m_ssjjInstallPath, int m_moveSpeed, int m_singleScriptTime, int m_loadingTimes)
{
    emit sendDisplayText("当前任务:" + m_task.taskName);
    task = m_task;
    ssjjInstallPath = m_ssjjInstallPath;
    moveSpeed = m_moveSpeed;
    singleScriptTime = m_singleScriptTime;
    loadingTimes = m_loadingTimes;
}

void SSJJMainThread::stopThread()
{
    if(currentThread != nullptr){
        currentThread->quit();
        currentThread->terminate();
    }
    nextStep = "stopThread";
}

void SSJJMainThread::receiveInitializeGameStates(SSJJRunState ssjjRunState)
{
    emit sendRemindInfo(ssjjRunState.remindText);
    nextStep = ssjjRunState.nextStep;
    //if(ssjjRunState.errorType == "NoError"){
    //    nextStep = "enterGame";
    //}
    //else if(ssjjRunState.errorType == "Error"){
    //    nextStep = "restartSSJJ";
    //}
    //else if (ssjjRunState.errorType == "Remind") {
    //    nextStep = "enterGame";
    //}
}

void SSJJMainThread::receiveRestartSSJJStates(SSJJRunState ssjjRunState)
{
    emit sendRemindInfo(ssjjRunState.remindText);
    nextStep = ssjjRunState.nextStep;
    //if(ssjjRunState.errorType == "FatalError"){
    //    nextStep = "fatalError";
    //}
    //else if(ssjjRunState.errorType == "Error"){
    //    nextStep = "restartSSJJ";
    //}
    //else if(ssjjRunState.errorType == "NoError"){
    //    nextStep = "initializeGameScreen";
    //}
}

void SSJJMainThread::receiveEnterGameStates(SSJJRunState ssjjRunState)
{
    emit sendRemindInfo(ssjjRunState.remindText);
    nextStep = ssjjRunState.nextStep;
    //if(ssjjRunState.errorType == "NoError"){
    //    nextStep = "runScriptFile";
    //}
    //else if(ssjjRunState.errorType == "Error"){
    //    nextStep = "restartSSJJ";
    //}
    //else if(ssjjRunState.errorType == "Remind"){
    //    nextStep = "enterGame";
    //}
    //else if(ssjjRunState.errorType == "FatalError"){
    //    nextStep = "fatalError";
    //}
    //else if(ssjjRunState.errorType == "Ini"){
    //    nextStep = "InitializeGameScreen";
    //}
}

void SSJJMainThread::receiveRunScriptStates(SSJJRunState ssjjRunState)
{
    emit sendRemindInfo(ssjjRunState.remindText);
    nextStep = ssjjRunState.nextStep;
    //if(ssjjRunState.errorType == "NoError"){
    //    nextStep = "settlement";
    //}
    //else if(ssjjRunState.errorType == "FatalError"){
    //    nextStep = "fatalError";
    //}
    //else if(ssjjRunState.errorType == "Error"){
    //    nextStep = "restartSSJJ";
    //}
}

void SSJJMainThread::receiveSettlementStates(SSJJRunState ssjjRunState)
{
    if(ssjjRunState.errorType == "Success"){
        emit singleTaskFinished(ssjjRunState);
    }
    emit sendRemindInfo(ssjjRunState.remindText);
    nextStep = ssjjRunState.nextStep;
    QThread::msleep(100);
}

void SSJJMainThread::receiveResolutionPath(QString m_resolution_path)
{
    resolutionPath = m_resolution_path;
}
