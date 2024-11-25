#ifndef SUBSSJJWIDGET_H
#define SUBSSJJWIDGET_H

#include <QWidget>
#include <QTableWidgetItem>
#include <QtNodes/internal/NodeData.hpp>
#include <QtNodes/internal/GraphicsView.hpp>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QFile>
#include <QStringListModel>
#include <QListview>

#include "thread/script/ssjjmainthread.h"
#include "thread/weapons/weaponbonusthread.h"
#include "universal/timeFun/pctime.h"
#include "thread/checkthreadstate.h"
#include "subwidgets/unversal/showtextinscreenwidget.h"


class QsciScintilla;

namespace Ui {
class SubSSJJWidget;
}

class SubSSJJWidget : public QWidget
{
    Q_OBJECT

public:
    explicit SubSSJJWidget(QWidget *parent = nullptr);
    ~SubSSJJWidget();

protected:
    void closeEvent(QCloseEvent *event) override;

private:
    Ui::SubSSJJWidget *ui;

    QString ssjjInstallPath;
    QVector<QWidget*> widgetList;
    QWidget* currentWidget;
    int currentRow;
    int runningState;
    SingleTask currentTask;
    SSJJMainThread* ssjjMainThread;
    QsciScintilla* scriptEditor;
    WeaponBonusThread* weaponBonusThread;
    QString resolutionPath;

private: signals:
    void widgetClosed();
    void sendStateInfo(QString state);
    void sendRestartParams(QString ssjjInstallPath);
    void sendInitializeGameParams(QString taskName);
    void sendSingleTask(SingleTask singleTask, QString ssjjInstallPath, int moveSpeed, int singleScriptTime, int loadingTimes);


private slots:
    void saveSettings();
    void loadSettings();
    void hideSomeItems();
    void on_testPushButton_clicked();
    void on_closePushButton_clicked();
    void writeRemindInfo(QString htmlString);
    void on_settingPushButton_clicked();
    void on_scriptPushButton_clicked();
    void on_writePushButton_clicked();
    void on_nodeEditorPushButton_clicked();
    void on_bonusPushButton_clicked();
    void on_terminateUnityPushButton_clicked();
    void on_terminateMicroClientPushButton_clicked();
    void on_terminateWDLaucherPushButton_clicked();
    void updateScreen();
    void on_chooseLauncherPathPushButton_clicked();
    void on_addTaskPushButton_clicked();
    void on_chooseLDScriptPathPushButton_clicked();
    void on_startPushButton_clicked();
    void on_endPushButton_clicked();
    void forceQuitSSJJThread();
    void receiveDisplayText(QString text);
    void on_singleBonusPushButton_clicked();
    void on_contineBonusPushButton_clicked();
    void on_stopBounsPushButton_clicked();
    void getSingleTask();
    void receiveRemindInfo(QString remindInfo);
    void sendNextTask(SSJJRunState res);
    void receiveFatalError();
    void clearRow(QTableWidgetItem *item);
    void createScriptEditor();
    void createNodeEditor();
    void updateCurrentWeaponList();
    void getBounsWeaponList();
    void saveBounsSettings(QString filePath);
    void loadBounsSettings(QString filePath);
    void on_saveBonusConfigPushButton_clicked();
    void readBonusJsonFiles();
};

#endif // SUBSSJJWIDGET_H
