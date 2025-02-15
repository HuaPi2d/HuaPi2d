#ifndef SSJJSTRUCT_H
#define SSJJSTRUCT_H

#include <QString>


// 下一步操作
// restartSSJJ: 重启游戏
// InitializeGameScreen：初始化游戏界面
// enterGame：进入游戏
// runScriptFile：运行脚本
// settlement：结算
// fatalError：致命错误，运行停止
struct SSJJRunState
{
    QString remindText;
    QString errorType; // FatalError, Error, NoError, Remind, Success
    QString nextStep;
};

namespace Task {
    enum TaskType {
        LuanDou = 0,
        ZhuXian
    };
};

struct SingleTask {
    QString taskName;
    QString difficulty;
    QString script;
    Task::TaskType taskType;
};

#endif // SSJJSTRUCT_H