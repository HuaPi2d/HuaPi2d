#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QSignalMapper>
#include <QButtonGroup>
#include <QMdiSubWindow>
#include <QMenu>
#include <QtAdvancedStylesheet.h>
#include <QmlStyleUrlInterceptor.h>

#include "otherwidgets/downloadlistwidget.h"
#include "otherwidgets/singledownloadframe.h"


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

private slots:
    void onThemeActionTriggered();
    void onStyleManagerStylesheetChanged();

public: signals:
    void isAnyChildWindow();
    void updateQuickWidgetTheme(acss::QtAdvancedStylesheet* advancedStyleSheet);
};
#endif // MAINWINDOW_H
