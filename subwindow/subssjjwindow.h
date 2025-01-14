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
    // �½��ļ�
    void createNewFile();
    // ���ļ�
    void openFile();
    // �����ļ�
    void saveFile();
    // �˵����ĸ���
    void updateMenuBar(QWidget* currentWidget);
    // ��������
    void setPasswordMode();
    // ����ű��ļ�
    void importScriptalFiles();

// �˵���
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
    QMenu* autoScriptMenu;
    QAction* passWordModeAction;
    QAction* importScriptalFilesAction;

private:
    void disableMenuAndActions(QMenu* menu);
    void enableMenuAndActions(QMenu* menu);
};

#endif // SUBSSJJWINDOW_H
