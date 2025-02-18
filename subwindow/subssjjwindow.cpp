#include "subssjjwindow.h"

SubSSJJWindow::SubSSJJWindow(QWidget *parent)
    : QMdiSubWindow(parent)
{
    // 设置全局热键
    F9 = new QHotkey(QKeySequence("F9"), true, this);
    F10 = new QHotkey(QKeySequence("F10"), true, this);
    F11 = new QHotkey(QKeySequence("F11"), true, this);

    this->setWindowFlags(Qt::FramelessWindowHint);
    // 创建主界面
    mainWidget = new QWidget(this);
    mainWidget->setContentsMargins(0, 0, 0, 0);
    this->setWidget(mainWidget);

    // 创建垂直布局
    mainLayout = new QVBoxLayout(this);
    mainLayout->setContentsMargins(0, 0, 0, 0);
    mainWidget->setLayout(mainLayout);

    // 添加菜单栏
    menuBar = new QMenuBar(mainWidget);
    mainLayout->addWidget(menuBar);

    // 添加子窗口
    subSSJJWidget = new SubSSJJWidget(mainWidget);
    mainLayout->addWidget(subSSJJWidget);

    // 菜单栏操作
    this->addMenuBarAction();

    connect(subSSJJWidget, &SubSSJJWidget::widgetClosed, this, [=](){
        emit windowDestroyed(this);
        this->deleteLater();
    });
    connect(subSSJJWidget, &SubSSJJWidget::sendStateInfo, this, &SubSSJJWindow::sendStateInfo);
    connect(subSSJJWidget, &SubSSJJWidget::updateMenuBar, this, &SubSSJJWindow::updateMenuBar);
    connect(subSSJJWidget, &SubSSJJWidget::unregiseterHotkey, this, &SubSSJJWindow::unregiseterHotkey);
    connect(subSSJJWidget, &SubSSJJWidget::regiseterHotkey, this, &SubSSJJWindow::regiseterHotkey);
    connect(this, &SubSSJJWindow::getGlobalEditorConfig, subSSJJWidget, &SubSSJJWidget::getGlobalEditorConfig);

    subSSJJWidget->loadSettings();

    // 更新语言
    connect(Language, &GlobalVariableQString::valueChanged, this, [=]() {
        retranslateUi();
        });
    reloadLanguage(Language->value());

    // 开发者选项设置‘
    developerOption();
    
}

SubSSJJWindow::~SubSSJJWindow()
{
}


void SubSSJJWindow::unregiseterHotkey() {
    F9->setRegistered(false);
    F10->setRegistered(false);
    F11->setRegistered(false);
    F9->destroyed();
    F10->destroyed();
    F11->destroyed();
}

void SubSSJJWindow::regiseterHotkey()
{
    F9 = new QHotkey(QKeySequence("F9"), true, this);
    F10 = new QHotkey(QKeySequence("F10"), true, this);
    F11 = new QHotkey(QKeySequence("F11"), true, this);
}

void SubSSJJWindow::addMenuBarAction()
{
    /* 文件操作 */ 
    // 创建 QMenu 对象
    fileMenu = new QMenu(tr("文件"), menuBar);
    editMenu = new QMenu(tr("脚本编辑器"), fileMenu);

    runMenu = new QMenu(tr("运行"), menuBar);
    scriptTestMenu = new QMenu(tr("脚本测试"), runMenu);
    bonusMenu = new QMenu(tr("加成"), runMenu);
    runAutoScriptMenu = new QMenu(tr("全自动脚本"), runMenu);

    settingMenu = new QMenu(tr("配置"), menuBar);
    editorSettingsMenu = new QMenu(tr("脚本编辑器配置"), settingMenu);
    autoScriptMenu = new QMenu(tr("全自动脚本配置"), settingMenu);
    scriptFilesMenu = new QMenu(tr("脚本文件"), fileMenu);
    taskListMenu = new QMenu(tr("任务列表数据"), fileMenu);

    // 新建文件
    createNewFileAction = new QAction(tr("新建"), fileMenu);
    createNewFileAction->setShortcut(QKeySequence(Qt::CTRL + Qt::Key_N));
    editMenu->addAction(createNewFileAction);
    connect(createNewFileAction, &QAction::triggered, subSSJJWidget, &SubSSJJWidget::createNewScpFile);
    // 打开文件
    openFileAction = new QAction(tr("打开"), fileMenu);
    openFileAction->setShortcut(QKeySequence(Qt::CTRL + Qt::Key_O));
    editMenu->addAction(openFileAction);
    connect(openFileAction, &QAction::triggered, subSSJJWidget, &SubSSJJWidget::openScpFile);
    // 保存文件
    saveFileAction = new QAction(tr("保存"), fileMenu);
    saveFileAction->setShortcut(QKeySequence(Qt::CTRL + Qt::Key_S));
    editMenu->addAction(saveFileAction);
    connect(saveFileAction, &QAction::triggered, this, &SubSSJJWindow::saveFile);
    // 导入脚本数据至本地数据库
    importScriptalFilesAction = new QAction(tr("导入脚本数据"), scriptFilesMenu);
    scriptFilesMenu->addAction(importScriptalFilesAction);
    connect(importScriptalFilesAction, &QAction::triggered, this, &SubSSJJWindow::importScriptalFiles);
    // 导入任务数据
    importTaskListAction = new QAction(tr("导入任务数据"), taskListMenu);
    taskListMenu->addAction(importTaskListAction);
    connect(importTaskListAction, &QAction::triggered, subSSJJWidget, &SubSSJJWidget::importTaskList);
    //导出任务数据
    exportTaskListAction = new QAction(tr("导出任务数据"), taskListMenu);
    taskListMenu->addAction(exportTaskListAction);
    connect(exportTaskListAction, &QAction::triggered, subSSJJWidget, &SubSSJJWidget::exportTaskList);
    // 录制脚本
    recordScriptAction = new QAction(tr("录制脚本 F9"), scriptTestMenu);
    connect(F9, &QHotkey::activated, recordScriptAction, &QAction::trigger);
    scriptTestMenu->addAction(recordScriptAction);
    connect(recordScriptAction, &QAction::triggered, subSSJJWidget, &SubSSJJWidget::startScriptRecord);
    // 测试代码
    testScriptAction = new QAction(tr("测试 F10"), scriptTestMenu);
    connect(F10, &QHotkey::activated, testScriptAction, &QAction::trigger);
    scriptTestMenu->addAction(testScriptAction);
    connect(testScriptAction, &QAction::triggered, subSSJJWidget, &SubSSJJWidget::testCurrentScript);
    // 终止测试
    stopTestScriptAction = new QAction(tr("终止 F11"), scriptTestMenu);
    connect(F11, &QHotkey::activated, stopTestScriptAction, &QAction::trigger);
    scriptTestMenu->addAction(stopTestScriptAction);
    connect(stopTestScriptAction, &QAction::triggered, subSSJJWidget, &SubSSJJWidget::stopTestScript);
    // 单次加成
    singleBonusAction = new QAction(tr("单次加成 F10"), bonusMenu);
    connect(F10, &QHotkey::activated, singleBonusAction, &QAction::trigger);
    bonusMenu->addAction(singleBonusAction);
    connect(singleBonusAction, &QAction::triggered, subSSJJWidget->ui->singleBonusPushButton, &QPushButton::click);
    // 连续加成
    continuousBonusAction = new QAction(tr("连续加成 F9"), bonusMenu);
    connect(F9, &QHotkey::activated, continuousBonusAction, &QAction::trigger);
    bonusMenu->addAction(continuousBonusAction);
    connect(continuousBonusAction, &QAction::triggered, subSSJJWidget->ui->contineBonusPushButton, &QPushButton::click);
    // 停止加成
    stopBonusAction = new QAction(tr("停止加成 F11"), bonusMenu);
    connect(F11, &QHotkey::activated, stopBonusAction, &QAction::trigger);
    bonusMenu->addAction(stopBonusAction);
    connect(stopBonusAction, &QAction::triggered, subSSJJWidget->ui->stopBounsPushButton, &QPushButton::click);
    // 运行脚本
    startAutoScriptAction = new QAction(tr("运行脚本 F10"), runAutoScriptMenu);
    connect(F10, &QHotkey::activated, startAutoScriptAction, &QAction::trigger);
    runAutoScriptMenu->addAction(startAutoScriptAction);
    connect(startAutoScriptAction, &QAction::triggered, subSSJJWidget->ui->startPushButton, &QPushButton::click);
    // 停止脚本
    stopAutoScriptAction = new QAction(tr("停止脚本 F11"), runAutoScriptMenu);
    connect(F11, &QHotkey::activated, stopAutoScriptAction, &QAction::trigger);
    runAutoScriptMenu->addAction(stopAutoScriptAction);
    connect(stopAutoScriptAction, &QAction::triggered, subSSJJWidget->ui->endPushButton, &QPushButton::click);
    // 脚本编辑器录制相关配置
    scriptRecordOptionAction = new QAction(tr("脚本录制配置"), editorSettingsMenu);
    editorSettingsMenu->addAction(scriptRecordOptionAction);
    connect(scriptRecordOptionAction, &QAction::triggered, subSSJJWidget, &SubSSJJWidget::showScriptRecordOptionDialog);
    // 脚本编辑器外观
    editorSettingsAction = new QAction(tr("外观"), editorSettingsMenu);
    editorSettingsMenu->addAction(editorSettingsAction);
    connect(editorSettingsAction, &QAction::triggered, subSSJJWidget, &SubSSJJWidget::showEditorSettingsDialog);
    // 设置房间密码模式
    passWordModeAction = new QAction(tr("脚本房间密码设置"), autoScriptMenu);
    autoScriptMenu->addAction(passWordModeAction);
    connect(passWordModeAction, &QAction::triggered, this, &SubSSJJWindow::setPasswordMode);

    // 添加菜单栏
    menuBar->addMenu(fileMenu);
    menuBar->addMenu(runMenu);
    menuBar->addMenu(settingMenu);


    fileMenu->addMenu(editMenu);
    fileMenu->addMenu(scriptFilesMenu);
    fileMenu->addMenu(taskListMenu);

    settingMenu->addMenu(editorSettingsMenu);
    settingMenu->addMenu(autoScriptMenu);

    runMenu->addMenu(scriptTestMenu);
    runMenu->addMenu(bonusMenu);
    runMenu->addMenu(runAutoScriptMenu);
}

void SubSSJJWindow::saveFile()
{
    subSSJJWidget->saveFile();
}

void SubSSJJWindow::updateMenuBar(QWidget *currentWidget)
{
    disableMenuAndActions(editMenu);
    disableMenuAndActions(autoScriptMenu);
    disableMenuAndActions(scriptTestMenu);
    disableMenuAndActions(runAutoScriptMenu);
    disableMenuAndActions(bonusMenu);
    disableMenuAndActions(editorSettingsMenu);
    disableMenuAndActions(scriptFilesMenu);
    disableMenuAndActions(taskListMenu);
    if (currentWidget == subSSJJWidget->ui->writeScriptWidget)
    {
        enableMenuAndActions(editMenu);
        enableMenuAndActions(scriptTestMenu);
        enableMenuAndActions(editorSettingsMenu);
    }
    else if (currentWidget == subSSJJWidget->ui->scriptWidget)
    {
        enableMenuAndActions(autoScriptMenu);
        enableMenuAndActions(runAutoScriptMenu);
        enableMenuAndActions(taskListMenu);
        enableMenuAndActions(scriptFilesMenu);

    }
    else if (currentWidget == subSSJJWidget->ui->bonusWidget)
    {
        enableMenuAndActions(bonusMenu);
    }
}

void SubSSJJWindow::setPasswordMode()
{
    PasswordModeDialog* passwordModeDialog = new PasswordModeDialog(this);
    passwordModeDialog->exec();
    if (passwordModeDialog->result() == QDialog::Accepted) {
        passWordMode = passwordModeDialog->getPasswordMode();
    }
}

void SubSSJJWindow::importScriptalFiles()
{
    // 开启对话框，选择某一文件夹
    QDir dir = QFileDialog::getExistingDirectory(this, tr("选择包含脚本文件的*文件夹*"), QDir::currentPath());
    if (!dir.exists()) {
        return;
    }
    subSSJJWidget->readFilesIntoSSJJDatabase(dir);
}

void SubSSJJWindow::retranslateUi()
{
    fileMenu->setTitle(QCoreApplication::translate("SubSSJJWindow", "文件", nullptr));
    editMenu->setTitle(QCoreApplication::translate("SubSSJJWindow", "脚本编辑器", nullptr));
    runMenu->setTitle(QCoreApplication::translate("SubSSJJWindow", "运行", nullptr));
    scriptTestMenu->setTitle(QCoreApplication::translate("SubSSJJWindow", "脚本测试", nullptr));
    bonusMenu->setTitle(QCoreApplication::translate("SubSSJJWindow", "加成", nullptr));
    runAutoScriptMenu->setTitle(QCoreApplication::translate("SubSSJJWindow", "全自动脚本", nullptr));
    settingMenu->setTitle(QCoreApplication::translate("SubSSJJWindow", "配置", nullptr));
    editorSettingsMenu->setTitle(QCoreApplication::translate("SubSSJJWindow", "脚本编辑器配置", nullptr));
    autoScriptMenu->setTitle(QCoreApplication::translate("SubSSJJWindow", "全自动脚本配置", nullptr));
    scriptFilesMenu->setTitle(QCoreApplication::translate("SubSSJJWindow", "脚本文件", nullptr));
    taskListMenu->setTitle(QCoreApplication::translate("SubSSJJWindow", "任务列表数据", nullptr));

    createNewFileAction->setText(QCoreApplication::translate("SubSSJJWindow", "新建", nullptr));
    openFileAction->setText(QCoreApplication::translate("SubSSJJWindow", "打开", nullptr));
    saveFileAction->setText(QCoreApplication::translate("SubSSJJWindow", "保存", nullptr));
    recordScriptAction->setText(QCoreApplication::translate("SubSSJJWindow", "录制脚本 F9", nullptr));
    testScriptAction->setText(QCoreApplication::translate("SubSSJJWindow", "测试 F10", nullptr));
    stopTestScriptAction->setText(QCoreApplication::translate("SubSSJJWindow", "终止 F11", nullptr));
    singleBonusAction->setText(QCoreApplication::translate("SubSSJJWindow", "单次加成 F10", nullptr));
    continuousBonusAction->setText(QCoreApplication::translate("SubSSJJWindow", "连续加成 F9", nullptr));
    stopBonusAction->setText(QCoreApplication::translate("SubSSJJWindow", "停止加成 F11", nullptr));
    startAutoScriptAction->setText(QCoreApplication::translate("SubSSJJWindow", "运行脚本 F10", nullptr));
    stopAutoScriptAction->setText(QCoreApplication::translate("SubSSJJWindow", "停止脚本 F11", nullptr));
    editorSettingsAction->setText(QCoreApplication::translate("SubSSJJWindow", "外观", nullptr));
    passWordModeAction->setText(QCoreApplication::translate("SubSSJJWindow", "脚本房间密码设置", nullptr));
    importScriptalFilesAction->setText(QCoreApplication::translate("SubSSJJWindow", "导入脚本数据", nullptr));
    importTaskListAction->setText(QCoreApplication::translate("SubSSJJWindow", "导入任务数据", nullptr));
    exportTaskListAction->setText(QCoreApplication::translate("SubSSJJWindow", "导出任务数据", nullptr));
}

void SubSSJJWindow::developerOption()
{
    if (developerMode == false) {
        editorSettingsAction->setVisible(false);
    }
}

void SubSSJJWindow::disableMenuAndActions(QMenu* menu)
{
    if (!menu) {
        return;
    }

    // 禁用顶级菜单
    menu->setEnabled(false);

    // 遍历并禁用所有子 QAction
    QList<QAction*> actions = menu->actions();
    for (QAction* action : actions) {
        // 禁用 QAction
        action->setEnabled(false);

        // 如果 QAction 是一个子菜单，递归禁用其子 QAction
        QMenu* subMenu = action->menu();
        if (subMenu) {
            disableMenuAndActions(subMenu);
        }
    }
}

void SubSSJJWindow::enableMenuAndActions(QMenu* menu)
{
    if (!menu) {
        return;
    }

    // 启用顶级菜单
    menu->setEnabled(true);

    // 遍历并启用所有子 QAction
    QList<QAction*> actions = menu->actions();
    for (QAction* action : actions) {
        // 启用 QAction
        action->setEnabled(true);

        // 如果 QAction 是一个子菜单，递归启用其子 QAction
        QMenu* subMenu = action->menu();
        if (subMenu) {
            enableMenuAndActions(subMenu);
        }
    }
}
