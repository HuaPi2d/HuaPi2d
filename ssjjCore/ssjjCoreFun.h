#ifndef SSJJCOREFUN_H
#define SSJJCOREFUN_H

#include "universal/programController.h"
#include "universal/script/image/findPicPro.h"
#include "global/ssjjglobalvar.h"
#include "universal/file/fileattributes.h"
#include "universal/timeFun/pctime.h"
#include "ssjjCore/script/scriptcompiler/scriptcompiler.h"


// ��һ������
// restartSSJJ: ������Ϸ
// InitializeGameScreen����ʼ����Ϸ����
// enterGame��������Ϸ
// runScriptFile�����нű�
// settlement������
// fatalError��������������ֹͣ
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

// �����Ӧ�ؿ�
bool clickLevel(FileAttribute level);

// ѡ���Ӧ�Ѷ�
void chooseDifficulty(QString difficulty);

// �������߹ؿ�
void enterZXGame();

#endif // SSJJCOREFUN_H
