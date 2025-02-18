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
#include <qlayout.h>
#include <QWeakPointer>

#include "thread/script/ssjjmainthread.h"
#include "thread/weapons/weaponbonusthread.h"
#include "universal/timeFun/pctime.h"
#include "thread/checkthreadstate.h"
#include "subwidgets/universal/showtextinscreenwidget.h"
#include "ui_subssjjwidget.h"
#include "ssjjCore/zx/zxlevels.h"
#include "DataBase/ssjj/ssjjscriptalfilesdatabase.h"
#include "thread/scriptTest/testscriptthread.h"
#include "universal/file/userssettings.h"
#include "ssjjCore/script/scripteditor/scplanguageeditor.h"
#include "subwidgets/Dialog/editor/editorsettingsdialog.h"
#include "struct/editor/EditorConfig.h"
#include "global/mainglobalvar.h"
#include "Dialog/ssjj/scriptrecordoptiondialog.h"
#include "Dialog/ssjj/recordteachingdialog.h"
#include "universal/filepath/filepath.h"
#include "thread/scriptRecord/scriptrecordthread.h"
#include "ssjjCore/script/recordToScp/keyboradrecordconvert.h"
#include "universal/file/excel.h"
#include "customize/customizedTableWidget/customizedtablewidget.h"
#include "Dialog/ssjj/modifytaskdialog.h"
#include "subwidgets/Dialog/createnewfiledialog.h"
#include "Dialog/ssjj/modifyfileattributesdialog.h"


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
    // 全局代码编辑器样式设置
    void loadSettings();

public:signals:
    void unregiseterHotkey();
    void regiseterHotkey();

public slots:
    // 创建新的文件编辑标签页
    void creatNewScriptEditorTab(QString fileName, QString filePath, QMap<QString, QString> fileAttributes, bool newFile = false);
    // 修改文件编辑标签页
    void modifyCurrentScriptEditorTab(QString fileName);
    void readFilesIntoSSJJDatabase(QDir dir);
    void saveFile();
    void testCurrentScript();
    void stopTestScript();
    // 调出代码编辑器设置对话框
    void showEditorSettingsDialog();
    // 更改编辑器配色
    void resetEditorsAppearances();
    void getGlobalEditorConfig(EditorConfig editorConfig);
    // 开始脚本录制
    void startScriptRecord();
    // 脚本录制设置对话框
    void showScriptRecordOptionDialog();
    // 任务列表操作
    void importTaskList();
    void exportTaskList();
    void createNewScpFile();
    void openScpFile();

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
    QPointer<WeaponBonusThread> weaponBonusThread;
    QPointer<TestScriptThread> testScriptThread;
    QPointer<ScriptRecordThread> scriptRecordThread;
    QsciScintilla* scriptEditor;
    QString resolutionPath;
    QList<ScpLanguageEditor *> scpLanguageEditors;
    ZXChapter currentChoosedZXChapter;                    // 当前页面选中的章节
    ZXLevel currentChoosedZXLevel;                        // 当前页面选中的关卡
    SSJJScriptalFilesDatabase* ssjjScriptalFilesDatabase; // 脚本文件数据库
    ScpLanguageEditor* currentScriptEditor;
    EditorConfig scpEditorConfig_dark;
    EditorConfig scpEditorConfig_light;
    EditorConfig globalEditorConfig;
    // 是否显示录制教学对话框
    bool ifShowRecordTeachingDialog;
    // 醒目的提示信息样式
    QString outlineStyleSheet;
    QHotkey* up_arrow;
    QHotkey* down_arrow;
    QHotkey* left_arrow;
    QHotkey* right_arrow;
    QHotkey* enter;
    QHotkey* middle;
    // 当前键盘是否可以控制鼠标
    bool ifCanControlMouse;

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
    // 录制脚本按钮按下
    void on_scriptRecordPushButton_clicked();
    void receiveDisplayText(QString text);
    void showTextOnScreen(QString text, QPoint pos, int time, QString labelStyleSheet);
    void on_singleBonusPushButton_clicked();
    void on_contineBonusPushButton_clicked();
    void on_stopBounsPushButton_clicked();
    void getSingleTask();
    void receiveRemindInfo(QString remindInfo);
    void sendNextTask(SSJJRunState res);
    void receiveFatalError();
    void clearRow(QTableWidgetItem *item);
    // 修改任务内容
    void modifyTask(QTableWidgetItem* item);
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
    void updateLDScriptPathComboBox();
    void currentTabChanged(int index);
    void getCurrentScriptEditor();
    void receiveWarningMessage(QString title, QString message);
    // 检测是否有线程在运行
    bool checkThreadState();
    // 处理录制结果
    void convertRecordResult();
    // 注册方向键
    void regiseterMouseHotkey();
    void unregiseterMouseHotkey();
    // 更改文件的属性值
    void modifyFileAttributes();
    // 终止一切当前运行的线程
    void terminateAllThreads();
};

#endif // SUBSSJJWIDGET_H
