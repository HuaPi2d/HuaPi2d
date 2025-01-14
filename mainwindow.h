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
#include "global/mainglobalvar.h"
#include "QHotkey/qhotkey.h"


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

private:
    Ui::MainWindow *ui;
    QString appDir;
    QString currentTheme;                       // ��ǰ����
    int currentThemeType;
    QSignalMapper *signalMapper;
    QButtonGroup *taskButtonList;
    QWidgetList *subWindowList;
    DownloadListWidget *downloadListWidget;
    acss::QtAdvancedStylesheet* advancedStyleSheet;

private slots:
    void onThemeActionTriggered();
    void onStyleManagerStylesheetChanged();
    // �ı䴰�ڴ�С
    void receiveDsizeInfo(int dwidth, int dheight);

public: signals:
    void isAnyChildWindow();
    void updateQuickWidgetTheme(acss::QtAdvancedStylesheet* advancedStyleSheet);
};
#endif // MAINWINDOW_H
