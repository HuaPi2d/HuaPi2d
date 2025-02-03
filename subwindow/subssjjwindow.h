#ifndef SUBSSJJWINDOW_H
#define SUBSSJJWINDOW_H

#include <QMdiSubWindow>
#include <QMainWindow>
#include <QVBoxLayout>
#include <QMenuBar>
#include <QAction>
#include <QMenu>
#include <QIcon>
#include <QPixmap>

#include "QHotkey/qhotkey.h"
#include "subwidgets/subssjjwidget.h"
#include "subwidgets/Dialog/createnewfiledialog.h"
#include "subwidgets/Dialog/ssjj/passwordmodedialog.h"

class SubSSJJWindow : public QMdiSubWindow
{
    Q_OBJECT
public:
    explicit SubSSJJWindow(QWidget *parent = nullptr);
    ~SubSSJJWindow();

public: signals:
    void getGlobalEditorConfig(EditorConfig globalEditorConfig);

signals:
    void sendStateInfo(QString state);
    void windowDestroyed(QMdiSubWindow *window);
    void downloadFinished();

private:
    QWidget *mainWidget;
    QVBoxLayout *mainLayout;
    QMenuBar *menuBar;
    SubSSJJWidget* subSSJJWidget;

    void addMenuBarAction();
    // 新建文件
    void createNewFile();
    // 打开文件
    void openFile();
    // 保存文件
    void saveFile();
    // 菜单栏的更新
    void updateMenuBar(QWidget* currentWidget);
    // 设置密码
    void setPasswordMode();
    // 导入脚本文件
    void importScriptalFiles();
    // 更新界面语言
    void retranslateUi();

// 菜单栏
private:
    QMenu* fileMenu;
    QMenu* editMenu;
    QAction* createNewFileAction;
    QAction* openFileAction;
    QAction* saveFileAction;

    QMenu* runMenu;
    QMenu* scriptTestMenu;
    QAction* testScriptAction;
    QAction* stopTestScriptAction;
    QMenu* bonusMenu;
    QAction* singleBonusAction;
    QAction* continuousBonusAction;
    QAction* stopBonusAction;
    QMenu* runAutoScriptMenu;
    QAction* startAutoScriptAction;
    QAction* stopAutoScriptAction;

    QMenu* settingMenu;
    QMenu* editorSettingsMenu;
    QAction* editorSettingsAction;
    QMenu* autoScriptMenu;
    QAction* passWordModeAction;
    QAction* importScriptalFilesAction;

private:
    void disableMenuAndActions(QMenu* menu);
    void enableMenuAndActions(QMenu* menu);
};

#endif // SUBSSJJWINDOW_H
