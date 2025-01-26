#ifndef SSJJCOREFUN_H
#define SSJJCOREFUN_H

#include "universal/programController.h"
#include "universal/script/image/findPicPro.h"
#include "global/ssjjglobalvar.h"
#include "universal/file/fileattributes.h"
#include "universal/timeFun/pctime.h"
#include "ssjjCore/script/scriptcompiler/scriptcompiler.h"


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

struct SingleTask{
    QString taskName;
    QString difficulty;
    QString script;
    Task::TaskType taskType;
};

QString checkCurrentState(int waitTime = 0);

cv::Point findAndClick(QString picPath, int timeLimit = 3000, int interval = 1000);

cv::Point findAndClickAndConfirm(QString picPath, int waitTime = 1000, int timeLimit = 3000, int interval = 1000);

bool findAndClickAndCheck(QString picPath, int waitTime = 1000, int timeLimit = 3000, int interval = 1000);

SSJJRunState restartSSJJ(QString ssjjInstallPath);

SSJJRunState initiallizeGameScreen(SingleTask task);

SSJJRunState enterGame(SingleTask task, int loadingTimes);

SSJJRunState runScript(SingleTask task, int speed);

SSJJRunState settlement(SingleTask task);

void turnToPage(FileAttribute chapter, FileAttribute level);

// 点击对应关卡
bool clickLevel(FileAttribute level);

// 选择对应难度
void chooseDifficulty(QString difficulty);

// 进入主线关卡
void enterZXGame();

#endif // SSJJCOREFUN_H
