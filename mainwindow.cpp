#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "subwindow/subvideowindow.h"
#include "universal/getnetwork.h"
#include "subwindow/submusicwindow.h"
#include "subwindow/subtexttransitionwindow.h"
#include "subwindow/randomlearningwindow.h"
#include "subwindow/subssjjwindow.h"
#include "subwindow/subaawindow.h"
#include "subwindow/subfilesconvertwindow.h"
#include "universal/filepro.h"

#include <qdockwidget.h>
#include <QSettings>
#include <QDebug>
#include <QFontDialog>
#include <QClipboard>
#include <QToolBar>
#include <QLabel>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>

#define _STR(x) #x
#define STRINGIFY(x) _STR(x)


namespace Syntax {
    enum Lexer {
        scp = 0,
    };
}


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    /* 隐藏内容 */
    // ui->defend_groupBox->hide();
    // ui->learning_groupBox->hide();
    // ui->net_groupBox->hide();
    // ui->menubar->hide();
    /* 加载主题 */
    loadThemes();

    /* 读取用户配置 */
    loadSettings();

    /* 更新好文好句 */
    getRandomSentence();

    /* 隐藏工作区，任务栏，禁用返回按钮 */
    ui->processToolBar->hide();
    ui->mutiWindowWidget->hide();

    /* 构建关于子窗口的簇 */
    signalMapper = new QSignalMapper(this);
    connect(signalMapper, SIGNAL(mapped(QWidget*)), this, SLOT(activateSubWindow(QWidget*)));
    taskButtonList = new QButtonGroup(this);
    taskButtonList->setExclusive(true);
    subWindowList = new QWidgetList;

    /* 添加一个按钮到菜单栏右侧 */
    QPushButton *downloadContentButton = new QPushButton(QIcon(":/icon/resources/icons/downloading.svg"), "", this);
    downloadContentButton->setStyleSheet("QPushButton {border: none;}");
    downloadContentButton->setToolTip("下载内容");
    ui->menubar->setCornerWidget(downloadContentButton, Qt::TopRightCorner);

    /* 加载QQuickWidget控件 */
    // ui->quickWidget->hide();

    connect(downloadContentButton, &QPushButton::clicked, [=](){
        // 获取按钮的全局位置
        QPoint buttonPos = downloadContentButton->mapToGlobal(QPoint(0, 0));

        // 计算无标题框窗口应显示的位置
        QPoint popupPos(buttonPos.x() + downloadContentButton->width() - downloadListWidget->width(), buttonPos.y() + downloadContentButton->height());

        // 设置无标题框窗口的位置
        downloadListWidget->show();
        downloadListWidget->move(popupPos);
    });

    /* 创建下载历史列表 */
    downloadListWidget = new DownloadListWidget(this);
    downloadListWidget->hide();

    /* 恢复默认配置 */
    connect(ui->reset_theme_action, &QAction::triggered, this, &MainWindow::loadDefaultSettings);

    /* 创建子窗口 */
    // 视频解析窗口
    connect(ui->video_action, &QAction::triggered, ui->video_pushButton, &QPushButton::click);
    connect(ui->video_pushButton, &QPushButton::clicked, this, &MainWindow::createVideoSubWindow);
    // 音乐盒窗口
    connect(ui->music_action, &QAction::triggered, ui->music_pushButton, &QPushButton::click);
    connect(ui->music_pushButton, &QPushButton::clicked, this, &MainWindow::createMusicSubWindow);
    // 文字转语音窗口
    connect(ui->textTransitionAction, &QAction::triggered, ui->textTransitionPushButton, &QPushButton::click);
    connect(ui->textTransitionPushButton, &QPushButton::clicked, this, &MainWindow::createTextTransitionSubWindow);
    // 随机复习窗口
    connect(ui->ranLearning_action, &QAction::triggered, ui->randomLearningPushButton, &QPushButton::click);
    connect(ui->randomLearningPushButton, &QPushButton::clicked, this, &MainWindow::createRandomLearningSubWindow);
    // 生死狙击窗口
    connect(ui->ssjj_action, &QAction::triggered, ui->ssjjPushButton, &QPushButton::click);
    connect(ui->ssjjPushButton, &QPushButton::clicked, this, &MainWindow::createSSJJSubWindow);
    // 航空航天窗口
    connect(ui->aa_action, &QAction::triggered, ui->aaPushButton, &QPushButton::click);
    connect(ui->aaPushButton, &QPushButton::clicked, this, &MainWindow::createAASubWindow);
    // 硬件开发窗口
    connect(ui->hardwareDevelopment_action, &QAction::triggered, ui->hardwareDevelopmentPushButton, &QPushButton::click);
    connect(ui->hardwareDevelopmentPushButton, &QPushButton::clicked, this, &MainWindow::createHardwareDevelopmentSubWindow);
    // 文件转换窗口
    connect(ui->filesConvert_action, &QAction::triggered, ui->filesConvertPushButton, &QPushButton::click);
    connect(ui->filesConvertPushButton, &QPushButton::clicked, this, &MainWindow::createFilesConvertSubWindow);

    // 开发者模式
    QHotkey* HUA = new QHotkey(QKeySequence("Ctrl+Shift+H"), true);
    connect(HUA, &QHotkey::activated, this, [=]() {
        if (developerMode == false) {
            this->setWindowTitle(this->windowTitle() + " - 开发者模式");
            developerMode = true;
        }
        else {
            this->setWindowTitle(this->windowTitle().replace(" - 开发者模式", ""));
            developerMode = false;
        }
        });

    /* 获取随机句子 */
    connect(ui->getRandomSentencePushButton, &QPushButton::clicked, this, &MainWindow::getRandomSentence); // 获取随机好文好句
    connect(ui->copySentencePushButton, &QPushButton::clicked, this, &MainWindow::copySentence);  // 复制到剪切板

    /* 显示与隐藏任务栏 */
    connect(ui->processBarControlPushButton, &QPushButton::clicked, this, [=](){
        if(ui->processToolBar->isHidden() == true)
        {
            ui->processToolBar->show();
            ui->processBarControlPushButton->setIcon(QIcon(":/icon/resources/icons/rightRight.svg"));
        }
        else
        {
            ui->processToolBar->hide();
            ui->processBarControlPushButton->setIcon(QIcon(":/icon/resources/icons/leftLeft.svg"));
        }
    });

    /* 回到主菜单 */
    connect(ui->home_action, &QAction::triggered, this, [=](){
        ui->processToolBar->setVisible(false);
        ui->mutiWindowWidget->setVisible(false);
        ui->mainWidget->setVisible(true);
    });

    connect(ui->function_action, &QAction::triggered, this, [=](){
        ui->mutiWindowWidget->setVisible(true);
        ui->mainWidget->setVisible(false);
    });
}

MainWindow::~MainWindow()
{
    saveSettings();
    delete ui;
}

void MainWindow::onThemeActionTriggered()
{
    auto action = qobject_cast<QAction*>(sender());
    currentTheme = action->text();
    currentThemeType = 0;
    updateTheme();
}

void MainWindow::onStyleManagerStylesheetChanged()
{
    qApp->setStyleSheet(advancedStyleSheet->styleSheet());
}

void MainWindow::receiveDsizeInfo(int dwidth, int dheight)
{
    // 获取当前窗口大小
    QSize currentSize = this->size();

    // 设置窗口大小
    this->resize(QSize(currentSize.width() + dwidth, currentSize.height() + dheight));
}

// 保存配置
void MainWindow::saveSettings()
{
    /* 声明对象 */
    UsersSettings settings(appDir + "/Settings/globalSettings.ini");

    /* 写入配置 */
    settings.beginGroup("window");
    settings.setValue("theme", currentTheme); // 写入主题配置
    settings.setValue("width", this->width());
    settings.setValue("height", this->height());
    settings.setValue("isMaximized", this->isMaximized()); // 窗口大小
    settings.setValue("developerMode", developerMode); // 开发者模式
    settings.beginGroup("text");
    settings.setValue("randomSentence", ui->randomSentenceLabel->text());
    settings.endGroup();
    settings.endGroup();

    /* 声明对象 */
    UsersSettings editorConfigSettings(appDir + "/Settings/editorConfig.ini");

    /* 写入编辑器配置 */
    editorConfigSettings.beginGroup("EditorConfig");
    editorConfigSettings.setValue("font", editorConfig.font.font.toString());
    editorConfigSettings.setValue("fontSize", editorConfig.font.size);
    editorConfigSettings.beginGroup("color");
    editorConfigSettings.setValue("background", editorConfig.color.background.name());
    editorConfigSettings.setValue("text", editorConfig.color.text.name());
    editorConfigSettings.setValue("keyword", editorConfig.color.keyword.name());
    editorConfigSettings.setValue("number", editorConfig.color.number.name());
    editorConfigSettings.setValue("string", editorConfig.color.string.name());
    editorConfigSettings.setValue("operateur", editorConfig.color.operateur.name());
    editorConfigSettings.setValue("function", editorConfig.color.function.name());
    editorConfigSettings.setValue("variable", editorConfig.color.variable.name());
    editorConfigSettings.setValue("comment", editorConfig.color.comment.name());
    editorConfigSettings.endGroup();
    editorConfigSettings.endGroup();
}

// 读取配置
void MainWindow::loadSettings()
{
    /* 声明对象 */
    UsersSettings settings(appDir + "/Settings/globalSettings.ini");

    /* 读取配置 */
    //// 窗口配置
    settings.beginGroup("window");
    // 主题
    currentTheme = settings.value("theme", "light_blue").toString();
    updateTheme();
    // 窗口大小
    this->resize(settings.value("width", 1385).toInt(), settings.value("height", 820).toInt());
    // 是否最大化
    if (settings.value("isMaximized", false).toBool() == true)
        this->showMaximized();
    // 开发者模式
    developerMode = settings.value("developerMode", false).toBool();
    // 文本
    settings.beginGroup("text");
    ui->randomSentenceLabel->setText(settings.value("randomSentence", "正在获取随机句子...").toString());
    settings.endGroup();
    settings.endGroup();

    hideSomeItems();

    // 全局编辑器配置
    /* 声明对象 */
    UsersSettings editorConfigSettings(appDir + "/Settings/editorConfig.ini");

    /* 读取编辑器配置 */
    //// 编辑器配置
    editorConfigSettings.beginGroup("EditorConfig");
    editorConfig.font.font = editorConfigSettings.value("font", QFont("Consolas")).value<QFont>();
    editorConfig.font.size = editorConfigSettings.value("fontSize", 12).toInt();
    editorConfigSettings.beginGroup("color");
    editorConfig.color.background = QColor(editorConfigSettings.value("background", "#FFFFFF").toString());
    editorConfig.color.text = QColor(editorConfigSettings.value("text", "#000000").toString());
    editorConfig.color.keyword = QColor(editorConfigSettings.value("keyword", "#0077aa").toString());
    editorConfig.color.number = QColor(editorConfigSettings.value("number", "#990000").toString());
    editorConfig.color.string = QColor(editorConfigSettings.value("string", "#009999").toString());
    editorConfig.color.operateur = QColor(editorConfigSettings.value("operateur", "#990099").toString());
    editorConfig.color.function = QColor(editorConfigSettings.value("function", "#0000FF").toString());
    editorConfig.color.variable = QColor(editorConfigSettings.value("variable", "#000099").toString());
    editorConfig.color.comment = QColor(editorConfigSettings.value("comment", "#999999").toString());
    editorConfigSettings.endGroup();
    editorConfigSettings.endGroup();
}

void MainWindow::hideSomeItems()
{
    if (developerMode == false) {
        ui->video_pushButton->setVisible(false);
        ui->files_groupBox->setVisible(false);
        ui->learning_groupBox->setVisible(false);
        ui->aa_action->setVisible(false);
        ui->hardwareDevelopment_action->setVisible(false);
        ui->filesConvert_action->setVisible(false);
        ui->video_action->setVisible(false);
        ui->ranLearning_action->setVisible(false);
    }
}

void MainWindow::loadThemes()
{
    // 加载预设主题
    /* 获取程序运行路径 */
    appDir = qApp->applicationDirPath();

    /* 样式表有关信息 */
    advancedStyleSheet = new acss::QtAdvancedStylesheet(this);
    QDir stylesDir;
    if (stylesDir.exists(appDir + "/../../styles") == true)
    {
        advancedStyleSheet->setStylesDirPath(appDir + "/../../styles");  // 设定主题文件路径
    }
    else
    {
        advancedStyleSheet->setStylesDirPath(appDir + "/styles");  // 设定主题文件路径
    }
    advancedStyleSheet->setOutputDirPath(appDir + "/output");
    advancedStyleSheet->setCurrentStyle("qt_material");
    advancedStyleSheet->setDefaultTheme();
    advancedStyleSheet->updateStylesheet();
    qApp->setStyleSheet(advancedStyleSheet->styleSheet());
    connect(advancedStyleSheet, &acss::QtAdvancedStylesheet::stylesheetChanged, this, &MainWindow::onStyleManagerStylesheetChanged);

    /* 连接主题更换信号 */
    for (QAction* action : ui->menu_styles->actions()) {
        connect(action, &QAction::triggered, this, &MainWindow::onThemeActionTriggered);
    }

    /* 添加菜单栏主题选项 */
    for (const auto& Theme : advancedStyleSheet->themes())
    {
        QAction* a = new QAction(Theme);
        ui->menu_styles->addAction(a);
        QObject::connect(a, &QAction::triggered, this, &MainWindow::onThemeActionTriggered);
    }

    // 读取主题文件
    readCustomizedThemeFile();
}

void MainWindow::readCustomizedThemeFile()
{
    // 读取文件目录
    QString directoryPath = "QSS";  // 指定路径
    createDirectoryIfNotExists(directoryPath);
    QDir dir(directoryPath);

    // 只获取后缀名为.qss的文件
    QStringList filters;
    filters << "*.qss";
    QStringList qssFiles = dir.entryList(filters, QDir::Files);

    foreach(QString fileName, qssFiles) {
        QFileInfo fileInfo(fileName);
        QString baseName = fileInfo.baseName();  // 去除后缀的文件名

        // 创建QAction并加入菜单
        QAction* fileAction = ui->menu_qss->addAction(baseName);

        // 将QAction的点击信号连接到槽函数
        connect(fileAction, &QAction::triggered, this, &MainWindow::onCustomizedThemesTriggered);
    }
}

void MainWindow::loadDefaultSettings()
{
    /* 读取默认配置 */
    currentTheme = "light_blue";
    updateTheme(); // 读取主题设置
}

void MainWindow::onCustomizedThemesTriggered()
{
    auto action = qobject_cast<QAction*>(sender());
    currentTheme = action->text();
    currentThemeType = 1;
    updateTheme();
}

void MainWindow::updateTheme()
{
    if (currentThemeType == 0) {
        if (currentTheme.contains("dark"))
        {
            ui->processToolBar->setStyleSheet("QToolBar{background-color:rgb(55, 61, 67)}");
        }
        else if (currentTheme.contains("light"))
        {
            ui->processToolBar->setStyleSheet("QToolBar{background-color:rgb(250,250,250)}");
        }
        advancedStyleSheet->setCurrentTheme(currentTheme);
        advancedStyleSheet->updateStylesheet();
        qApp->setStyleSheet(advancedStyleSheet->styleSheet());
        emit updateQuickWidgetTheme(advancedStyleSheet);
    }
    else {
        // 自定义主题
        QFile file("QSS/" + currentTheme + ".qss");
        if (file.open(QFile::ReadOnly)) {
            QString styleSheet = QLatin1String(file.readAll());
            qApp->setStyleSheet(styleSheet);  // 只对当前窗口应用 QSS 样式表
        }
    }
    
}

void MainWindow::getRandomSentence()
{
    /* 声明对象 */
    GetNetWork *getSentence = new GetNetWork(this);

    /* 连接信号与槽 */
    connect(getSentence, &GetNetWork::sendData, this, [=](QByteArray data){
        ui->randomSentenceLabel->setText(data);
    });
    connect(getSentence, &GetNetWork::sendError, this, [=](){
        showStateInfo("请求出现错误，更新失败");
    });

    /* 发送请求 */
    getSentence->getData(QUrl("https://uapis.cn/api/say"));
}

void MainWindow::copySentence()
{
    /* 声明对象 */
    QClipboard *clipBoard = QApplication::clipboard();

    /* 写入剪切板 */
    clipBoard->setText(ui->randomSentenceLabel->text());
}

void MainWindow::changeToWorkingUI()
{
    /* 隐藏主界面，显示功能界面 */
    ui->mainWidget->hide();
    ui->mutiWindowWidget->show();

    /* 更改按钮图标 */
    if(ui->processToolBar->isHidden() == true)
    {
        ui->processBarControlPushButton->setIcon(QIcon(":/icon/resources/icons/leftLeft.svg"));
    }
    else
    {
        ui->processBarControlPushButton->setIcon(QIcon(":/icon/resources/icons/rightRight.svg"));
    }
}

void MainWindow::showStateInfo(QString state)
{
    ui->statusbar->showMessage(state, 3000);
}

void MainWindow::checkSubWindows(QMdiSubWindow *window) {
    if (window == nullptr)
    {
        ui->mainWidget->show();
        ui->processToolBar->hide();
        ui->mutiWindowWidget->hide();
    }
    else {
        window->showMaximized();
    }
}

void MainWindow::createMusicSubWindow()
{
    /* 隐藏主界面，显示功能界面 */
    if(ui->mainWidget->isHidden() == false)
        changeToWorkingUI();

    /* 创建音乐界面子窗口 */
    SubMusicWindow *subMusicWindow = new SubMusicWindow(this);  // 创建子窗口
    ui->functionArea->addSubWindow(subMusicWindow);  // 添加到工作区
    subMusicWindow->showMaximized();  // 最大化显示
    subWindowList->append(subMusicWindow); // 加入子窗口列表

    /* 创建任务栏按钮 */
    QPushButton *taskButton = new QPushButton(this);
    taskButton->setIcon(QIcon(":/icon/resources/icons/MaterialSymbolsGenresOutline.svg"));
    taskButton->setCheckable(true);
    taskButton->setChecked(true);
    taskButton->setStyleSheet("QPushButton {border: none; background-color:rgb(231, 234, 248);}QPushButton:hover {background-color:rgb(249, 239, 241);}QPushButton:checked {background-color:grey;}");
    ui->processToolBar->addWidget(taskButton);
    this->taskButtonList->addButton(taskButton);

    /* 将任务栏按钮和子窗口连接起来 */
    connect(taskButton, &QPushButton::clicked, signalMapper, QOverload<>::of(&QSignalMapper::map));
    signalMapper->setMapping(taskButton, subMusicWindow);

    connect(taskButton, &QPushButton::clicked, this, [=](){
        subMusicWindow->showMaximized();
    });

    /* 状态信息接收 */
    connect(subMusicWindow, &SubMusicWindow::sendStateInfo, this, &MainWindow::showStateInfo);

    /* 窗口销毁时的事件 */
    connect(subMusicWindow, &SubMusicWindow::windowDestroyed, [this, taskButton, &subMusicWindow](QMdiSubWindow *window) {
        /* 在List中剔除对应窗口，按钮 */
        this->taskButtonList->removeButton(taskButton);
        for(int i = 0; i<this->subWindowList->count(); i++)
        {
            if(this->subWindowList->at(i) == window)
            {
                this->subWindowList->remove(i);
                break;
            }
        }

        /* 判断是否回到主界面 */
        if(this->subWindowList->empty() == true)
        {
            ui->processToolBar->setVisible(false);
            ui->mutiWindowWidget->setVisible(false);
            ui->mainWidget->setVisible(true);
        }
        else
        {
            this->subWindowList->at(0)->showMaximized();
        }

        /* 销毁任务栏按钮 */
        taskButton->deleteLater();
    });

    /* 下载任务 */
    connect(subMusicWindow, &SubMusicWindow::addDownloadTask, this, &MainWindow::addDownloadTask);
}

void MainWindow::createVideoSubWindow()
{
    /* 隐藏主界面，显示功能界面 */
    if(ui->mainWidget->isHidden() == false)
        changeToWorkingUI();

    /* 创建音乐界面子窗口 */
    SubVideoWindow *subVideoWindow = new SubVideoWindow(this);  // 创建子窗口
    ui->functionArea->addSubWindow(subVideoWindow);  // 添加到工作区
    subVideoWindow->showMaximized();  // 最大化显示
    subWindowList->append(subVideoWindow); // 加入子窗口列表

    /* 创建任务栏按钮 */
    QPushButton *taskButton = new QPushButton(this);
    taskButton->setIcon(QIcon(":/icon/resources/icons/global.svg"));
    taskButton->setCheckable(true);
    taskButton->setChecked(true);
    taskButton->setStyleSheet("QPushButton {border: none; background-color:rgb(231, 234, 248);}QPushButton:hover {background-color:rgb(249, 239, 241);}QPushButton:checked {background-color:grey;}");
    ui->processToolBar->addWidget(taskButton);
    this->taskButtonList->addButton(taskButton);

    /* 状态信息接收 */
    connect(subVideoWindow, &SubVideoWindow::sendStateInfo, this, &MainWindow::showStateInfo);

    /* 将任务栏按钮和子窗口连接起来 */
    connect(taskButton, &QPushButton::clicked, signalMapper, QOverload<>::of(&QSignalMapper::map));
    signalMapper->setMapping(taskButton, subVideoWindow);

    connect(taskButton, &QPushButton::clicked, this, [=](){
        subVideoWindow->showMaximized();
    });

    /* 窗口销毁时的事件 */
    connect(subVideoWindow, &SubVideoWindow::windowDestroyed, [this, taskButton](QMdiSubWindow *window) {
        /* 在List中剔除对应窗口，按钮 */
        this->taskButtonList->removeButton(taskButton);
        for(int i = 0; i<this->subWindowList->count(); i++)
        {
            if(this->subWindowList->at(i) == window)
            {
                this->subWindowList->remove(i);
                break;
            }
        }

        /* 判断是否回到主界面 */
        if(this->subWindowList->empty() == true)
        {
            ui->processToolBar->setVisible(false);
            ui->mutiWindowWidget->setVisible(false);
            ui->mainWidget->setVisible(true);
        }
        else
        {
            this->subWindowList->at(0)->showMaximized();
        }

        /* 销毁任务栏按钮 */
        taskButton->deleteLater();
    });

    /* 主题一致 */
    connect(this, &MainWindow::updateQuickWidgetTheme, subVideoWindow, &SubVideoWindow::updateQuickWidgetTheme);
    updateTheme();
}

void MainWindow::createTextTransitionSubWindow()
{
    /* 隐藏主界面，显示功能界面 */
    if(ui->mainWidget->isHidden() == false)
        changeToWorkingUI();

    /* 创建音乐界面子窗口 */
    SubTextTransitionWindow *subTextTransitionWindow = new SubTextTransitionWindow(this);  // 创建子窗口
    ui->functionArea->addSubWindow(subTextTransitionWindow);  // 添加到工作区
    subTextTransitionWindow->showMaximized();  // 最大化显示
    subWindowList->append(subTextTransitionWindow); // 加入子窗口列表

    /* 创建任务栏按钮 */
    QPushButton *taskButton = new QPushButton(this);
    taskButton->setIcon(QIcon(":/icon/resources/icons/textTransition.png"));
    taskButton->setCheckable(true);
    taskButton->setChecked(true);
    taskButton->setStyleSheet("QPushButton {border: none; background-color:rgb(231, 234, 248);}QPushButton:hover {background-color:rgb(249, 239, 241);}QPushButton:checked {background-color:grey;}");
    ui->processToolBar->addWidget(taskButton);
    this->taskButtonList->addButton(taskButton);

    /* 将任务栏按钮和子窗口连接起来 */
    connect(taskButton, &QPushButton::clicked, signalMapper, QOverload<>::of(&QSignalMapper::map));
    signalMapper->setMapping(taskButton, subTextTransitionWindow);

    connect(taskButton, &QPushButton::clicked, this, [=](){
        subTextTransitionWindow->showMaximized();
    });

    /* 状态信息接收 */
    connect(subTextTransitionWindow, &SubTextTransitionWindow::sendStateInfo, this, &MainWindow::showStateInfo);

    /* 窗口销毁时的事件 */
    connect(subTextTransitionWindow, &SubTextTransitionWindow::windowDestroyed, [this, taskButton, &subTextTransitionWindow](QMdiSubWindow *window) {
        /* 在List中剔除对应窗口，按钮 */
        this->taskButtonList->removeButton(taskButton);
        for(int i = 0; i<this->subWindowList->count(); i++)
        {
            if(this->subWindowList->at(i) == window)
            {
                this->subWindowList->remove(i);
                break;
            }
        }

        /* 判断是否回到主界面 */
        if(this->subWindowList->empty() == true)
        {
            ui->processToolBar->setVisible(false);
            ui->mutiWindowWidget->setVisible(false);
            ui->mainWidget->setVisible(true);
        }
        else
        {
            this->subWindowList->at(0)->showMaximized();
        }

        /* 销毁任务栏按钮 */
        taskButton->deleteLater();
    });

    /* 下载任务 */
    connect(subTextTransitionWindow, &SubTextTransitionWindow::addDownloadTask, this, &MainWindow::addDownloadTask);
}

void MainWindow::createRandomLearningSubWindow()
{
    /* 隐藏主界面，显示功能界面 */
    if(ui->mainWidget->isHidden() == false)
        changeToWorkingUI();

    /* 创建音乐界面子窗口 */
    RandomLearningWindow *randomLearningWindow = new RandomLearningWindow(this);  // 创建子窗口
    ui->functionArea->addSubWindow(randomLearningWindow);  // 添加到工作区
    randomLearningWindow->showMaximized();  // 最大化显示
    subWindowList->append(randomLearningWindow); // 加入子窗口列表

    /* 创建任务栏按钮 */
    QPushButton *taskButton = new QPushButton(this);
    taskButton->setIcon(QIcon(":/icon/resources/icons/randomLearning.png"));
    taskButton->setCheckable(true);
    taskButton->setChecked(true);
    taskButton->setStyleSheet("QPushButton {border: none; background-color:rgb(231, 234, 248);}QPushButton:hover {background-color:rgb(249, 239, 241);}QPushButton:checked {background-color:grey;}");
    ui->processToolBar->addWidget(taskButton);
    this->taskButtonList->addButton(taskButton);

    /* 将任务栏按钮和子窗口连接起来 */
    connect(taskButton, &QPushButton::clicked, signalMapper, QOverload<>::of(&QSignalMapper::map));
    signalMapper->setMapping(taskButton, randomLearningWindow);

    connect(taskButton, &QPushButton::clicked, this, [=](){
        randomLearningWindow->showMaximized();
    });

    /* 状态信息接收 */
    connect(randomLearningWindow, &RandomLearningWindow::sendStateInfo, this, &MainWindow::showStateInfo);

    /* 窗口销毁时的事件 */
    connect(randomLearningWindow, &RandomLearningWindow::windowDestroyed, [this, taskButton, &randomLearningWindow](QMdiSubWindow *window) {
        /* 在List中剔除对应窗口，按钮 */
        this->taskButtonList->removeButton(taskButton);
        for(int i = 0; i<this->subWindowList->count(); i++)
        {
            if(this->subWindowList->at(i) == window)
            {
                this->subWindowList->remove(i);
                break;
            }
        }

        /* 判断是否回到主界面 */
        if(this->subWindowList->empty() == true)
        {
            ui->processToolBar->setVisible(false);
            ui->mutiWindowWidget->setVisible(false);
            ui->mainWidget->setVisible(true);
        }
        else
        {
            this->subWindowList->at(0)->showMaximized();
        }

        /* 销毁任务栏按钮 */
        taskButton->deleteLater();
    });

    // 连接窗口大小变化信号
    connect(randomLearningWindow, &RandomLearningWindow::sendDsizeInfo, this, &MainWindow::receiveDsizeInfo);
}

void MainWindow::createSSJJSubWindow()
{
    /* 隐藏主界面，显示功能界面 */
    if(ui->mainWidget->isHidden() == false)
        changeToWorkingUI();

    /* 创建生死狙击界面子窗口 */
    SubSSJJWindow *subSSJJWindow = new SubSSJJWindow(this);  // 创建子窗口
    ui->functionArea->addSubWindow(subSSJJWindow);  // 添加到工作区
    subSSJJWindow->showMaximized();  // 最大化显示
    subWindowList->append(subSSJJWindow); // 加入子窗口列表

    /* 创建任务栏按钮 */
    QPushButton *taskButton = new QPushButton(this);
    taskButton->setIcon(QIcon(":/icon/resources/icons/ico.ico"));
    taskButton->setCheckable(true);
    taskButton->setChecked(true);
    taskButton->setStyleSheet("QPushButton {border: none; background-color:rgb(231, 234, 248);}QPushButton:hover {background-color:rgb(249, 239, 241);}QPushButton:checked {background-color:grey;}");
    ui->processToolBar->addWidget(taskButton);
    this->taskButtonList->addButton(taskButton);

    /* 将任务栏按钮和子窗口连接起来 */
    connect(taskButton, &QPushButton::clicked, signalMapper, QOverload<>::of(&QSignalMapper::map));
    signalMapper->setMapping(taskButton, subSSJJWindow);

    /* 传递全局编辑器配置 */
    connect(this, &MainWindow::getGlobalEditorConfig, subSSJJWindow, &SubSSJJWindow::getGlobalEditorConfig);
    emit getGlobalEditorConfig(editorConfig);

    connect(taskButton, &QPushButton::clicked, this, [=](){
        subSSJJWindow->showMaximized();
    });

    /* 状态信息接收 */
    connect(subSSJJWindow, &SubSSJJWindow::sendStateInfo, this, &MainWindow::showStateInfo);

    /* 窗口销毁时的事件 */
    connect(subSSJJWindow, &SubSSJJWindow::windowDestroyed, [this, taskButton, &subSSJJWindow](QMdiSubWindow *window) {
        /* 在List中剔除对应窗口，按钮 */
        this->taskButtonList->removeButton(taskButton);
        for(int i = 0; i<this->subWindowList->count(); i++)
        {
            if(this->subWindowList->at(i) == window)
            {
                this->subWindowList->remove(i);
                break;
            }
        }

        /* 判断是否回到主界面 */
        if(this->subWindowList->empty() == true)
        {
            ui->processToolBar->setVisible(false);
            ui->mutiWindowWidget->setVisible(false);
            ui->mainWidget->setVisible(true);
        }
        else
        {
            this->subWindowList->at(0)->showMaximized();
        }

        /* 销毁任务栏按钮 */
        taskButton->deleteLater();
    });
}

void MainWindow::createAASubWindow()
{
    /* 隐藏主界面，显示功能界面 */
    if (ui->mainWidget->isHidden() == false)
        changeToWorkingUI();

    /* 创建音乐界面子窗口 */
    SubAAWindow* subAAWindow = new SubAAWindow(this);  // 创建子窗口
    ui->functionArea->addSubWindow(subAAWindow);  // 添加到工作区
    subAAWindow->showMaximized();  // 最大化显示
    subWindowList->append(subAAWindow); // 加入子窗口列表

    /* 创建任务栏按钮 */
    QPushButton* taskButton = new QPushButton(this);
    taskButton->setIcon(QIcon(":/icon/resources/icons/MaterialSymbolsMediaLinkOutline.svg"));
    taskButton->setCheckable(true);
    taskButton->setChecked(true);
    taskButton->setStyleSheet("QPushButton {border: none; background-color:rgb(231, 234, 248);}QPushButton:hover {background-color:rgb(249, 239, 241);}QPushButton:checked {background-color:grey;}");
    ui->processToolBar->addWidget(taskButton);
    this->taskButtonList->addButton(taskButton);

    /* 状态信息接收 */
    connect(subAAWindow, &SubAAWindow::sendStateInfo, this, &MainWindow::showStateInfo);

    /* 将任务栏按钮和子窗口连接起来 */
    connect(taskButton, &QPushButton::clicked, signalMapper, QOverload<>::of(&QSignalMapper::map));
    signalMapper->setMapping(taskButton, subAAWindow);

    connect(taskButton, &QPushButton::clicked, this, [=]() {
        subAAWindow->showMaximized();
    });

    /* 窗口销毁时的事件 */
    connect(subAAWindow, &SubAAWindow::windowDestroyed, [this, taskButton](QMdiSubWindow* window) {
        /* 在List中剔除对应窗口，按钮 */
        this->taskButtonList->removeButton(taskButton);
        for (int i = 0; i < this->subWindowList->count(); i++)
        {
            if (this->subWindowList->at(i) == window)
            {
                this->subWindowList->remove(i);
                break;
            }
        }

        /* 判断是否回到主界面 */
        if (this->subWindowList->empty() == true)
        {
            ui->processToolBar->setVisible(false);
            ui->mutiWindowWidget->setVisible(false);
            ui->mainWidget->setVisible(true);
        }
        else
        {
            this->subWindowList->at(0)->showMaximized();
        }

        /* 销毁任务栏按钮 */
        taskButton->deleteLater();
    });
}

void MainWindow::createFilesConvertSubWindow()
{
    /* 隐藏主界面，显示功能界面 */
    if (ui->mainWidget->isHidden() == false)
        changeToWorkingUI();

    /* 创建音乐界面子窗口 */
    SubFilesConvertWindow* subFilesConvertWindow = new SubFilesConvertWindow(this);  // 创建子窗口
    ui->functionArea->addSubWindow(subFilesConvertWindow);  // 添加到工作区
    subFilesConvertWindow->showMaximized();  // 最大化显示
    subWindowList->append(subFilesConvertWindow); // 加入子窗口列表

    /* 创建任务栏按钮 */
    QPushButton* taskButton = new QPushButton(this);
    taskButton->setIcon(QIcon(":/icon/resources/icons/filesConvert.svg"));
    taskButton->setCheckable(true);
    taskButton->setChecked(true);
    taskButton->setStyleSheet("QPushButton {border: none; background-color:rgb(231, 234, 248);}QPushButton:hover {background-color:rgb(249, 239, 241);}QPushButton:checked {background-color:grey;}");
    ui->processToolBar->addWidget(taskButton);
    this->taskButtonList->addButton(taskButton);

    /* 状态信息接收 */
    connect(subFilesConvertWindow, &SubFilesConvertWindow::sendStateInfo, this, &MainWindow::showStateInfo);

    /* 将任务栏按钮和子窗口连接起来 */
    connect(taskButton, &QPushButton::clicked, signalMapper, QOverload<>::of(&QSignalMapper::map));
    signalMapper->setMapping(taskButton, subFilesConvertWindow);

    connect(taskButton, &QPushButton::clicked, this, [=]() {
        subFilesConvertWindow->showMaximized();
        });

    /* 窗口销毁时的事件 */
    connect(subFilesConvertWindow, &SubFilesConvertWindow::windowDestroyed, [this, taskButton](QMdiSubWindow* window) {
        /* 在List中剔除对应窗口，按钮 */
        this->taskButtonList->removeButton(taskButton);
        for (int i = 0; i < this->subWindowList->count(); i++)
        {
            if (this->subWindowList->at(i) == window)
            {
                this->subWindowList->remove(i);
                break;
            }
        }

        /* 判断是否回到主界面 */
        if (this->subWindowList->empty() == true)
        {
            ui->processToolBar->setVisible(false);
            ui->mutiWindowWidget->setVisible(false);
            ui->mainWidget->setVisible(true);
        }
        else
        {
            this->subWindowList->at(0)->showMaximized();
        }

        /* 销毁任务栏按钮 */
        taskButton->deleteLater();
        });
}

void MainWindow::createHardwareDevelopmentSubWindow()
{
    /* 隐藏主界面，显示功能界面 */
    if (ui->mainWidget->isHidden() == false)
        changeToWorkingUI();

    /* 创建音乐界面子窗口 */
    SubAAWindow* subAAWindow = new SubAAWindow(this);  // 创建子窗口
    ui->functionArea->addSubWindow(subAAWindow);  // 添加到工作区
    subAAWindow->showMaximized();  // 最大化显示
    subWindowList->append(subAAWindow); // 加入子窗口列表

    /* 创建任务栏按钮 */
    QPushButton* taskButton = new QPushButton(this);
    taskButton->setIcon(QIcon(":/icon/resources/icons/MaterialSymbolsMediaLinkOutline.svg"));
    taskButton->setCheckable(true);
    taskButton->setChecked(true);
    taskButton->setStyleSheet("QPushButton {border: none; background-color:rgb(231, 234, 248);}QPushButton:hover {background-color:rgb(249, 239, 241);}QPushButton:checked {background-color:grey;}");
    ui->processToolBar->addWidget(taskButton);
    this->taskButtonList->addButton(taskButton);

    /* 状态信息接收 */
    connect(subAAWindow, &SubAAWindow::sendStateInfo, this, &MainWindow::showStateInfo);

    /* 将任务栏按钮和子窗口连接起来 */
    connect(taskButton, &QPushButton::clicked, signalMapper, QOverload<>::of(&QSignalMapper::map));
    signalMapper->setMapping(taskButton, subAAWindow);

    connect(taskButton, &QPushButton::clicked, this, [=]() {
        subAAWindow->showMaximized();
        });

    /* 窗口销毁时的事件 */
    connect(subAAWindow, &SubAAWindow::windowDestroyed, [this, taskButton](QMdiSubWindow* window) {
        /* 在List中剔除对应窗口，按钮 */
        this->taskButtonList->removeButton(taskButton);
        for (int i = 0; i < this->subWindowList->count(); i++)
        {
            if (this->subWindowList->at(i) == window)
            {
                this->subWindowList->remove(i);
                break;
            }
        }

        /* 判断是否回到主界面 */
        if (this->subWindowList->empty() == true)
        {
            ui->processToolBar->setVisible(false);
            ui->mutiWindowWidget->setVisible(false);
            ui->mainWidget->setVisible(true);
        }
        else
        {
            this->subWindowList->at(0)->showMaximized();
        }

        /* 销毁任务栏按钮 */
        taskButton->deleteLater();
        });
}

/* 添加下载任务 */
void MainWindow::addDownloadTask(SingleDownloadFrame *singleDownloadFrame)
{
    singleDownloadFrame->setParent(this);
    downloadListWidget->layout->addWidget(singleDownloadFrame);
    connect(singleDownloadFrame, &SingleDownloadFrame::sendStateInfo, this, &MainWindow::showStateInfo);
    singleDownloadFrame->startDownload();
}

