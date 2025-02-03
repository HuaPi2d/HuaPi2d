#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <Python.h>
#include <QMainWindow>
#include <QSignalMapper>
#include <QButtonGroup>
#include <QMdiSubWindow>
#include <QMenu>
#include <QtAdvancedStylesheet.h>
#include <QmlStyleUrlInterceptor.h>

#include "otherwidgets/downloadlistwidget.h"
#include "otherwidgets/singledownloadframe.h"
#include "global/mainglobalvar.h"
#include "QHotkey/qhotkey.h"
#include "universal/file/userssettings.h"
#include "struct/editor/EditorConfig.h"


namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    void saveSettings();
    void loadSettings();
    void hideSomeItems();
    void loadThemes();
    void readCustomizedThemeFile();
    void onCustomizedThemesTriggered();
    void updateTheme();
    void loadDefaultSettings();
    void getRandomSentence();
    void copySentence();
    void changeToWorkingUI();
    void showStateInfo(QString state);
    void checkSubWindows(QMdiSubWindow *window);
    void createMusicSubWindow();
    void createVideoSubWindow();
    void createTextTransitionSubWindow();
    void createRandomLearningSubWindow();
    void createSSJJSubWindow();
    void createAASubWindow();
    void createFilesConvertSubWindow();
    void createHardwareDevelopmentSubWindow();
    void addDownloadTask(SingleDownloadFrame *singleDownloadFrame);
    // 切换语言
    void changeLanguage(QString language);

private:
    Ui::MainWindow *ui;
    QString appDir;
    QString currentTheme;                       // 当前主题
    int currentThemeType;
    QSignalMapper *signalMapper;
    QButtonGroup *taskButtonList;
    QWidgetList *subWindowList;
    DownloadListWidget *downloadListWidget;
    acss::QtAdvancedStylesheet* advancedStyleSheet;
    EditorConfig editorConfig;

private slots:
    void onThemeActionTriggered();
    void onStyleManagerStylesheetChanged();
    // 改变窗口大小
    void receiveDsizeInfo(int dwidth, int dheight);
    void testPythonCode();

public: signals:
    void isAnyChildWindow();
    void updateQuickWidgetTheme(acss::QtAdvancedStylesheet* advancedStyleSheet);
    void getGlobalEditorConfig(EditorConfig globalEditorConfig);
    // 切换语言
    void languageChanged(QString language);
};
#endif // MAINWINDOW_H
