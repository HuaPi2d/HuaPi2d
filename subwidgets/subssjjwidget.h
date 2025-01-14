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
#include <QList>
#include <QTabWidget>
#include <QSettings>
#include <QFileDialog>
#include <QMessageBox>
#include <QIntValidator>
#include <QComboBox>
#include <QScrollBar>
#include <Qsci/qsciscintilla.h>
#include <qlayout.h>

#include "thread/script/ssjjmainthread.h"
#include "thread/weapons/weaponbonusthread.h"
#include "universal/timeFun/pctime.h"
#include "thread/checkthreadstate.h"
#include "subwidgets/universal/showtextinscreenwidget.h"
#include "ssjjCore/script/scripteditor/scplanguageeditor.h"
#include "ui_subssjjwidget.h"
#include "ssjjCore/zx/zxlevels.h"
#include "DataBase/ssjj/ssjjscriptalfilesdatabase.h"
#include "thread/scriptTest/testscriptthread.h"


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

    Ui::SubSSJJWidget* ui;
    void loadSettings();

public slots:
    // 创建新的文件编辑标签页
    void creatNewScriptEditorTab(QString fileName, QString filePath, QList<FileAttribute> fileAttributes);
    void readFilesIntoSSJJDatabase(QDir dir);
    void saveFile();
    void testCurrentScript();
    void stopTestScript();

protected:
    void closeEvent(QCloseEvent *event) override;

private:
    QString ssjjInstallPath;
    QVector<QWidget*> widgetList;
    QWidget* currentWidget;
    int currentRow;
    int runningState;
    SingleTask currentTask;
    QPointer<SSJJMainThread> ssjjMainThread;
    QsciScintilla* scriptEditor;
    WeaponBonusThread* weaponBonusThread;
    QPointer<TestScriptThread> testScriptThread;
    QString resolutionPath;
    QList<ScpLanguageEditor *> scpLanguageEditors;
    ZXChapter currentChoosedZXChapter;                    // 当前页面选中的章节
    ZXLevel currentChoosedZXLevel;                        // 当前页面选中的关卡
    SSJJScriptalFilesDatabase* ssjjScriptalFilesDatabase; // 脚本文件数据库
    ScpLanguageEditor* currentScriptEditor;

private: signals:
    void widgetClosed();
    void sendStateInfo(QString state);
    void sendRestartParams(QString ssjjInstallPath);
    void sendInitializeGameParams(QString taskName);
    void sendSingleTask(SingleTask singleTask, QString ssjjInstallPath, int moveSpeed, int singleScriptTime, int loadingTimes);
    void updateMenuBar(QWidget* currentWidget);


private slots:
    void saveSettings();
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
    // 选择乱斗脚本
    void on_chooseLDScriptPathPushButton_clicked();
    // 选择主线关卡脚本
    void on_chooseZXScriptPathPushButton_clicked();
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
    void createNodeEditor();
    void updateCurrentWeaponList();
    void getBounsWeaponList();
    void saveBounsSettings(QString filePath);
    void loadBounsSettings(QString filePath);
    void on_saveBonusConfigPushButton_clicked();
    void readBonusJsonFiles();
    void closeTab(int index);
    void updateZXLevelChooseComboBox();
    void updateZXDiffcultyChooseComboBox();
    void updateZXScriptPathComboBox();
    void currentTabChanged(int index);
    void getCurrentScriptEditor();
    void receiveWarningMessage(QString title, QString message);
};

#endif // SUBSSJJWIDGET_H
