#ifndef SSJJSTRUCT_H
#define SSJJSTRUCT_H

#include <QString>


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

struct SingleTask {
    QString taskName;
    QString difficulty;
    QString script;
    Task::TaskType taskType;
};

#endif // SSJJSTRUCT_H