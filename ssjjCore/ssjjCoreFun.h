#ifndef SSJJCOREFUN_H
#define SSJJCOREFUN_H

#include "universal/programController.h"
#include "universal/script/image/findPicPro.h"
#include "global/ssjjglobalvar.h"

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

struct SingleTask{
    QString taskName;
    QString difficulty;
    QString script;
};

QString checkCurrentState(int waitTime = 0);

cv::Point findAndClick(QString picPath, int timeLimit = 3000, int interval = 1000);

cv::Point findAndClickAndConfirm(QString picPath, int waitTime = 1000, int timeLimit = 3000, int interval = 1000);

bool findAndClickAndCheck(QString picPath, int waitTime = 1000, int timeLimit = 3000, int interval = 1000);

SSJJRunState restartSSJJ(QString ssjjInstallPath);

SSJJRunState initiallizeGameScreen(QString taskName);

SSJJRunState enterGame(SingleTask task, int loadingTimes);

SSJJRunState runScript(SingleTask task, int speed);

SSJJRunState settlement(QString taskName);

#endif // SSJJCOREFUN_H
