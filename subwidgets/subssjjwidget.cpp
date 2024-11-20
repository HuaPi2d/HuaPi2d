#include "subssjjwidget.h"
#include "ui_subssjjwidget.h"
#include "thread/script/startssjjthread.h"
#include "thread/script/initializegamethread.h"
#include "universal/programController.h"
#include "universal/script/image/findPicPro.h"
#include "QHotkey/qhotkey.h"
#include "thread/weapons/mainWeapons/huguang.h"
#include "thread/weapons/mainWeapons/shenyu.h"
#include "thread/weapons/mainWeapons/limingzhiguang.h"
#include "thread/weapons/mainWeapons/meiying.h"
#include "thread/weapons/mainWeapons/caijue.h"
#include "thread/weapons/secondaryWeapons/feisuo.h"
#include "thread/weapons/secondaryWeapons/nengfang.h"
#include "thread/weapons/secondaryWeapons/fengshen.h"
#include "thread/weapons/meleeWeapons/anshuijing.h"
#include "thread/weapons/meleeWeapons/hantian.h"
#include "thread/weapons/tacticalWeapons/huihe.h"
#include "thread/weapons/throwingWeapons/liangyi.h"
#include "thread/weapons/throwingWeapons/anshi.h"
#include "thread/weapons/throwingWeapons/tianshu.h"
#include "thread/weapons/character/liubi.h"
#include "thread/weapons/character/taikesix.h"
#include "thread/weapons/longQi/hundunbaofa.h"


#include <QSettings>
#include <QFileDialog>
#include <QMessageBox>
#include <QIntValidator>
#include <QComboBox>
#include <QScrollBar>
#include <Qsci/qsciscintilla.h>
#include <qlayout.h>

// 获取生死狙击安装路径
QString getSSJJInstallPath() {
    QSettings settings("HKEY_CURRENT_USER\\Software\\Wooduan\\SSJJ-4399", QSettings::NativeFormat);
    QString ssjjPath = settings.value("InstallPath").toString();
    return ssjjPath;
}

SubSSJJWidget::SubSSJJWidget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::SubSSJJWidget)
{
    ui->setupUi(this);
    widgetList.append(ui->scriptWidget);
    widgetList.append(ui->settingWidget);
    widgetList.append(ui->writeScriptWidget);
    widgetList.append(ui->nodeEditorWidget);
    widgetList.append(ui->bonusWidget);
    /* 搭建脚本编辑界面 */
    createScriptEditor();

    /* 搭建图形编辑器 */
    createNodeEditor();

    /* 脚本框设置 */
    ui->remindTextEdit->setReadOnly(true);
    ui->taskTableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->testPushButton->setVisible(false);
    writeRemindInfo("<p>欢迎使用<b>生死狙击脚本</b>工具</p><p><span style=\"font-size: 13px; color: red;\">使用时注意本栏信息</span></p><p>请开启<span style=\"font-size: 13px; color: red;\"><b>自动登录</b></span></p>");
    ui->launcherPathLineEdit->setReadOnly(true);
    ui->LDScriptPathLineEdit->setReadOnly(true);
    QIntValidator *validator = new QIntValidator(0, 999, this);
    ui->moveSpeedLineEdit->setValidator(validator);
    ui->LDRunTimesLineEdit->setValidator(validator);
    QIntValidator *loadingTimeValidator = new QIntValidator(0, 99, this);
    ui->loadingTimeLineEdit->setValidator(loadingTimeValidator);
    QIntValidator *validatorScriptTime = new QIntValidator(0, 20, this);
    ui->singleScriptTimeLineEdit->setValidator(validatorScriptTime);

    /* 热键设置 */
    QHotkey* F10 = new QHotkey(QKeySequence("F10"),true);
    QObject::connect(F10, &QHotkey::activated, ui->startPushButton, &QPushButton::click);
    QHotkey* F11 = new QHotkey(QKeySequence("F11"),true);
    QObject::connect(F11, &QHotkey::activated, ui->endPushButton, &QPushButton::click);
    QHotkey* Ctrl_F = new QHotkey(QKeySequence("Ctrl+F"),true);
    QObject::connect(Ctrl_F, &QHotkey::activated, ui->singleBonusPushButton, &QPushButton::click);
    QHotkey* Ctrl_M = new QHotkey(QKeySequence("Ctrl+M"),true);
    QObject::connect(Ctrl_M, &QHotkey::activated, ui->contineBonusPushButton, &QPushButton::click);
    QHotkey* Ctrl_T = new QHotkey(QKeySequence("Ctrl+T"),true);
    QObject::connect(Ctrl_T, &QHotkey::activated, ui->stopBounsPushButton, &QPushButton::click);
    runningState = 0;

    /* 连接combox更新信号 */
    connect(ui->mainWeaponComboBox, &QComboBox::currentIndexChanged, this, &SubSSJJWidget::updateCurrentWeaponList);
    connect(ui->secondaryWeaponComboBox, &QComboBox::currentIndexChanged, this, &SubSSJJWidget::updateCurrentWeaponList);
    connect(ui->meleeWeaponComboBox, &QComboBox::currentIndexChanged, this, &SubSSJJWidget::updateCurrentWeaponList);
    connect(ui->throwingWeaponComboBox, &QComboBox::currentIndexChanged, this, &SubSSJJWidget::updateCurrentWeaponList);
    connect(ui->tacticalWeaponComboBox, &QComboBox::currentIndexChanged, this, &SubSSJJWidget::updateCurrentWeaponList);
    connect(ui->characterComboBox, &QComboBox::currentIndexChanged, this, &SubSSJJWidget::updateCurrentWeaponList);
    connect(ui->longQiComboBox, &QComboBox::currentIndexChanged, this, &SubSSJJWidget::updateCurrentWeaponList);

    /* 连接配置文件点击信号 */
    connect(ui->bonusConfigListListView, &QListView::clicked, this, [=](const QModelIndex& index) {
        // 获取点击的文件名
        QString fileName = index.data().toString();

        // 获取运行目录下的 bonusConfig 文件夹路径
        QString configDirPath = QDir(QCoreApplication::applicationDirPath()).filePath("bonusConfig");

        // 拼接文件路径
        QString filePath = QDir(configDirPath).filePath(fileName);

        // 读取文件内容
        loadBounsSettings(filePath);
        });

    /* 设置控件属性 */
    // 列表内元素可拖动
    ui->currentWeaponListWidget->setDragDropMode(QAbstractItemView::InternalMove);

    /* 清除表格 */
    connect(ui->taskTableWidget, &QTableWidget::itemClicked, this, &SubSSJJWidget::clearRow);

    loadSettings();

    writeRemindInfo("<p><span style=\"color: red;\">请确保在每次启动游戏时不会弹出以下图片</p><br>");
    writeRemindInfo("<img src=\"tips/ensure_ratio.png\" width=\"250\" alt=\"提示图片\"><br>");
}

SubSSJJWidget::~SubSSJJWidget()
{
    saveSettings();
    delete ui;
}

void SubSSJJWidget::closeEvent(QCloseEvent *event)
{
    emit widgetClosed();
}

void SubSSJJWidget::saveSettings()
{
    /* 声明对象 */
    QSettings setting(qApp->applicationDirPath() + "/userSettings.ini", QSettings::IniFormat);

    /* 写入配置 */
    setting.beginGroup("SSJJ_Script");
    setting.setValue("ssjjInstallPath", ssjjInstallPath);
    setting.setValue("moveSpeed", ui->moveSpeedLineEdit->text());
    setting.setValue("LDRunTimes", ui->LDRunTimesLineEdit->text());
    setting.setValue("LDScriptPath", ui->LDScriptPathLineEdit->text());
    setting.setValue("singleScriptTime", ui->singleScriptTimeLineEdit->text());
    setting.setValue("loadingTime", ui->loadingTimeLineEdit->text());
    setting.beginGroup("bonusWeapenList");
    setting.setValue("mainWeaponList", ui->mainWeaponComboBox->currentText());
    setting.setValue("secondaryWeaponList", ui->secondaryWeaponComboBox->currentText());
    setting.setValue("meleeWeaponList", ui->meleeWeaponComboBox->currentText());
    setting.setValue("throwingWeaponList", ui->throwingWeaponComboBox->currentText());
    setting.setValue("tacticalWeaponList", ui->tacticalWeaponComboBox->currentText());
    setting.setValue("characterList", ui->characterComboBox->currentText());
    setting.setValue("longQiList", ui->longQiComboBox->currentText());
    setting.setValue("currentPage", widgetList.indexOf(currentWidget));
    QStringList weaponList;
    for (int i = 0; i < ui->currentWeaponListWidget->count(); i++)
    {
        weaponList.append(ui->currentWeaponListWidget->item(i)->text());
    }
    setting.setValue("currentWeaponList", weaponList);
    setting.endGroup();
    setting.endGroup();
}

void SubSSJJWidget::loadSettings()
{
    /* 声明对象 */
    QSettings setting(qApp->applicationDirPath() + "/userSettings.ini", QSettings::IniFormat);

    /* 写入配置 */
    setting.beginGroup("SSJJ_Script");
    if(setting.value("ssjjInstallPath").toString() != "")
    {
        ssjjInstallPath = setting.value("ssjjInstallPath").toString();
        writeRemindInfo("<p>生死狙击程序安装路径:</p><b>" + ssjjInstallPath + "</b><br><br>");
    }
    else
    {
        ssjjInstallPath = getRegDitValue("\\HKEY_CURRENT_USER\\Software\\Wooduan\\SSJJ-4399", "InstallPath") + "\\WDlauncher.exe";
        if( ssjjInstallPath == ""){
            writeRemindInfo("<p><span style=\"font-size: 13px; color: red;\">生死狙击程序安装路径读取失败，请手动添加<b>WDlauncher.exe</b>的路径</span></p><br>");
            writeRemindInfo("<br><img src=\"tips/choose_launcher_tip.png\" width=\"250\" alt=\"提示图片\"><br>");
        }
        else{
            writeRemindInfo("<p>生死狙击程序安装路径:<b>" + ssjjInstallPath + "</b></p><br>");
        }
    }
    if(setting.value("moveSpeed").toString() != ""){
        ui->moveSpeedLineEdit->setText(setting.value("moveSpeed").toString());
    }
    else{
        ui->moveSpeedLineEdit->setText("100");
    }
    ui->launcherPathLineEdit->setText(ssjjInstallPath);
    if(setting.value("LDRunTimes").toString() != ""){
        ui->LDRunTimesLineEdit->setText(setting.value("LDRunTimes").toString());
    }
    else{
        ui->LDRunTimesLineEdit->setText("100");
    }
    ui->LDScriptPathLineEdit->setText(setting.value("LDScriptPath").toString());
    if(setting.value("singleScriptTime").toString() != ""){
        ui->singleScriptTimeLineEdit->setText(setting.value("singleScriptTime").toString());
    }
    else{
        ui->singleScriptTimeLineEdit->setText("5");
    }
    if(setting.value("loadingTime").toString() != ""){
        ui->loadingTimeLineEdit->setText(setting.value("loadingTime").toString());
    }
    else{
        ui->loadingTimeLineEdit->setText("18");
    }
    setting.beginGroup("bonusWeapenList");
     // 加载 combox 内容
    if (setting.value("mainWeaponList").toString() != "") {
        ui->mainWeaponComboBox->setCurrentText(setting.value("mainWeaponList").toString());
    }
    if (setting.value("secondaryWeaponList").toString() != "") {
        ui->secondaryWeaponComboBox->setCurrentText(setting.value("secondaryWeaponList").toString());
    }
    if (setting.value("meleeWeaponList").toString() != "") {
        ui->meleeWeaponComboBox->setCurrentText(setting.value("meleeWeaponList").toString());
    }
    if (setting.value("throwingWeaponList").toString() != "") {
        ui->throwingWeaponComboBox->setCurrentText(setting.value("throwingWeaponList").toString());
    }
    if (setting.value("tacticalWeaponList").toString() != "") {
        ui->tacticalWeaponComboBox->setCurrentText(setting.value("tacticalWeaponList").toString());
    }
    if (setting.value("characterList").toString() != "") {
        ui->characterComboBox->setCurrentText(setting.value("characterList").toString());
    }
    if (setting.value("longQiList").toString() != "")
    {
        ui->longQiComboBox->setCurrentText(setting.value("longQiList").toString());
    }
    if (setting.value("currentWeaponList").toStringList() != QStringList()) {
        for (QString str : setting.value("currentWeaponList").toStringList()) {
            ui->currentWeaponListWidget->addItem(str);
        }
        for (int i = 0; i < 7; i++)
        {
            int currentWeaponNum = ui->currentWeaponListWidget->count();
            for (int j = 0; j < currentWeaponNum + 1; j++) {
                if (i == 0 && j < currentWeaponNum) {
                    if(ui->currentWeaponListWidget->item(j)->text().contains("主武器"))break;
                }
                else if(i == 0 && j >= currentWeaponNum){
                    ui->currentWeaponListWidget->addItem("主武器");
                    break;
                }
                else if(i == 1 && j < currentWeaponNum) {
                    if(ui->currentWeaponListWidget->item(j)->text().contains("副武器"))break;
                }
                else if(i == 1 && j >= currentWeaponNum){
                    ui->currentWeaponListWidget->addItem("副武器");
                    break;
                }
                else if(i == 2 && j < currentWeaponNum) {
                    if(ui->currentWeaponListWidget->item(j)->text().contains("近战武器"))break;
                }
                else if(i == 2 && j >= currentWeaponNum){
                    ui->currentWeaponListWidget->addItem("近战武器");
                    break;
                }
                else if(i == 3 && j < currentWeaponNum) {
                    if(ui->currentWeaponListWidget->item(j)->text().contains("投掷武器"))break;
                }
                else if(i == 3 && j >= currentWeaponNum){
                    ui->currentWeaponListWidget->addItem("投掷武器");
                    break;
                }
                else if(i == 4 && j < currentWeaponNum) {
                    if(ui->currentWeaponListWidget->item(j)->text().contains("战术武器"))break;
                }
                else if(i == 4 && j >= currentWeaponNum){
                    ui->currentWeaponListWidget->addItem("战术武器");
                    break;
                }
                else if(i == 5 && j < currentWeaponNum) {
                    if(ui->currentWeaponListWidget->item(j)->text().contains("角色"))break;
                }
                else if(i == 5 && j >= currentWeaponNum){
                    ui->currentWeaponListWidget->addItem("角色");
                    break;
                }  
                else if(i == 6 && j < currentWeaponNum) {
                    if(ui->currentWeaponListWidget->item(j)->text().contains("龙骑兵"))break;
                }
                else if(i == 6 && j >= currentWeaponNum){
                    qDebug() << "龙骑兵";
                    ui->currentWeaponListWidget->addItem("龙骑兵");
                    break;
                    qDebug() << "龙骑兵";
                }
            }
        }
    }
    else {
        ui->currentWeaponListWidget->addItem("主武器");
        ui->currentWeaponListWidget->addItem("副武器");
        ui->currentWeaponListWidget->addItem("近战武器");
        ui->currentWeaponListWidget->addItem("投掷武器");
        ui->currentWeaponListWidget->addItem("战术武器");
        ui->currentWeaponListWidget->addItem("角色");
    }
    if (setting.value("currentPage").toInt() != -1)
    {
        currentWidget = widgetList[setting.value("currentPage").toInt()];
    }
    else
    {
        currentWidget = ui->scriptWidget;
    }
    updateScreen();
    setting.endGroup();
    setting.endGroup();

    readBonusJsonFiles();
}

void SubSSJJWidget::on_testPushButton_clicked()
{
    StartSSJJThread* startSSJJThread = new StartSSJJThread();
    connect(this, &SubSSJJWidget::sendRestartParams, startSSJJThread, &StartSSJJThread::receiveParams);
    connect(startSSJJThread, &StartSSJJThread::sendStates, this, [=](SSJJRunState res){
        ui->remindTextEdit->insertHtml(res.remindText);
        if(res.errorType == "FatalError"){
            return;
        }
        else if(res.errorType == "Error"){
            ui->remindTextEdit->insertHtml("<p>正在尝试重新启动</p>");
            return;
        }
    });
    emit sendRestartParams(ssjjInstallPath);
    startSSJJThread->start();
}

void SubSSJJWidget::on_closePushButton_clicked()
{
    this->close();
}

// 添加提示信息
void SubSSJJWidget::writeRemindInfo(QString htmlString)
{
    // 获取当前光标位置
    QTextCursor cursor = ui->remindTextEdit->textCursor();

    // 将光标移动到文档末尾
    cursor.movePosition(QTextCursor::End);

    cursor.insertHtml(htmlString);

    // 滚动到 QTextEdit 的末尾
    QScrollBar *scrollBar = ui->remindTextEdit->verticalScrollBar();
    scrollBar->setValue(scrollBar->maximum());
}

/* 转到设置界面 */
void SubSSJJWidget::on_settingPushButton_clicked()
{
    currentWidget = ui->settingWidget;
    updateScreen();
}

/* 转到脚本界面 */
void SubSSJJWidget::on_scriptPushButton_clicked()
{
    currentWidget = ui->scriptWidget;
    updateScreen();
}

void SubSSJJWidget::on_writePushButton_clicked()
{
    currentWidget = ui->writeScriptWidget;
    updateScreen();
}

void SubSSJJWidget::on_nodeEditorPushButton_clicked()
{
    currentWidget = ui->nodeEditorWidget;
    updateScreen();
}

void SubSSJJWidget::on_bonusPushButton_clicked()
{
    currentWidget = ui->bonusWidget;
    updateScreen();
}

/* 界面转换的更新 */
void SubSSJJWidget::updateScreen()
{
    for(QWidget* m_Widget: widgetList){
        if(m_Widget == currentWidget){
            m_Widget->show();
        }
        else{
            m_Widget->hide();
        }
    }
}

/* 选择启动器路径 */
void SubSSJJWidget::on_chooseLauncherPathPushButton_clicked()
{
    // 设置文件过滤器，只允许选择 .exe 文件
    QString filter = "Executable Files (*.exe)";

    // 打开文件选择对话框
    QString fileName = QFileDialog::getOpenFileName(this, "请选择 WDlauncher.exe",  "", filter);

    // 检查用户是否选择了文件
    if (!fileName.isEmpty()) {
        // 检查选择的文件是否是 WDlauncher.exe
        QFileInfo fileInfo(fileName);
        // qDebug() << fileInfo.fileName();
        if (fileInfo.fileName().contains("WDlauncher.exe")) {
            // 如果是 WDlauncher.exe，执行进一步操作
            QMessageBox::information(this, "成功", "您已成功选择 WDlauncher.exe 文件！");
            ui->launcherPathLineEdit->setText(fileName);
            ssjjInstallPath = fileName;
        } else {
            // 如果不是 WDlauncher.exe，弹出警告提示
            QMessageBox::warning(this, "错误", "请选择 WDlauncher.exe 文件！");
        }
    }
}

/* 判断当前脚本界面是否选择了关卡 */
bool isAnyRadioButtonChecked(QWidget *widget) {
    // 获取所有子对象
    QList<QRadioButton *> radioButtons = widget->findChildren<QRadioButton *>();

    // 遍历所有 QRadioButton，判断是否有被选中的
    for (QRadioButton *radioButton : radioButtons) {
        if (radioButton->isChecked()) {
            return true;  // 找到被选中的按钮
        }
    }

    return false;  // 没有按钮被选中
}

/* 添加脚本任务 */
void SubSSJJWidget::on_addTaskPushButton_clicked()
{
    if(isAnyRadioButtonChecked(ui->tabWidget->currentWidget())){
        ui->taskTableWidget->setRowCount(ui->taskTableWidget->rowCount() + 1);
        currentRow = ui->taskTableWidget->rowCount();
    }
    else{
        writeRemindInfo("<p>请<span style=\"color:red;\"><b>先选择关卡</b></span>后再添加任务</p><br>");
        return;
    }

    if(ui->tabWidget->currentWidget() == ui->LDTab){
        if(ui->LDRunTimesLineEdit->text() == ""){
            writeRemindInfo("<p><span style=\"color:red;\"><b>关卡运行次数</b></span>未指定</p><br>");
            ui->taskTableWidget->removeRow(currentRow - 1);
            return;
        }

        ui->taskTableWidget->setItem(currentRow - 1, 1, new QTableWidgetItem("无"));
        ui->taskTableWidget->setItem(currentRow - 1, 2, new QTableWidgetItem(ui->LDRunTimesLineEdit->text()));
        ui->taskTableWidget->setItem(currentRow - 1, 3, new QTableWidgetItem("0"));
        if(ui->LDScriptPathLineEdit->text() != ""){
            ui->taskTableWidget->setItem(currentRow - 1, 4, new QTableWidgetItem(ui->LDScriptPathLineEdit->text()));
        }
        else{
            ui->taskTableWidget->setItem(currentRow - 1, 4, new QTableWidgetItem("未选择"));
        }

        if(ui->teamPropRadioButton->isChecked() == true){
            ui->taskTableWidget->setItem(currentRow - 1, 0, new QTableWidgetItem("团队道具赛"));
        }
    }
    else if(ui->tabWidget->currentWidget() == ui->ZXTab){

    }
}

/* 选择脚本 */
void SubSSJJWidget::on_chooseLDScriptPathPushButton_clicked()
{
    // 设置文件过滤器，只允许选择 .exe 文件
    QString filter = "Script File (*.scp)";

    // 打开文件选择对话框
    QString fileName = QFileDialog::getOpenFileName(this, "请选择乱斗脚本文件(LD结尾)",  "", filter);

    // 检查用户是否选择了文件
    if (!fileName.isEmpty()) {
        // 检查选择的文件是否是 乱斗脚本
        QFileInfo fileInfo(fileName);
        if (fileInfo.fileName().contains("LD")) {
            // 如果是 乱斗脚本，执行进一步操作
            ui->LDScriptPathLineEdit->setText(fileName);
        } else {
            // 如果不是 乱斗脚本，弹出警告提示
            QMessageBox::information(this, "提示", "选择的可能不是乱斗脚本");
            ui->LDScriptPathLineEdit->setText(fileName);
        }
    }
    else{
        ui->LDScriptPathLineEdit->setText("");
    }
}

/* 开始运行脚本按钮按下 */
void SubSSJJWidget::on_startPushButton_clicked()
{
    // 重置任务次数
    for(int i = 0; i < ui->taskTableWidget->rowCount(); i++){
        ui->taskTableWidget->setItem(i, 3, new QTableWidgetItem(QString::number(0)));
    }

    if(runningState == 1){
        writeRemindInfo("<p><span style=\"color:red;\"><b>脚本正在运行中，请勿重复点击</b></span></p><br>");
        return;
    }
    runningState = 1;
    writeRemindInfo("<p><span style=\"color:lightgreen;\"><b>脚本开始运行</b></span></p><br>");

    // 脚本逻辑
    /* 创建脚本线程 */
    ssjjMainThread = new SSJJMainThread(this);
    /* 接收提示信息 */
    connect(ssjjMainThread, &SSJJMainThread::sendRemindInfo, this, &SubSSJJWidget::receiveRemindInfo);
    /* 发送单个任务 */
    connect(this, &SubSSJJWidget::sendSingleTask, ssjjMainThread, &SSJJMainThread::receiveTask);
    /* 接收任务执行结果 */
    connect(ssjjMainThread, &SSJJMainThread::singleTaskFinished, this, &SubSSJJWidget::sendNextTask);
    connect(ssjjMainThread, &SSJJMainThread::sendFatalError, this, &SubSSJJWidget::receiveFatalError);
    /* 发送任务 */
    sendNextTask(SSJJRunState());
    /* 开始执行 */
    ssjjMainThread->start();
}

void SubSSJJWidget::on_endPushButton_clicked()
{
    if(runningState == 0){
        writeRemindInfo("<p><span style=\"color:red;\"><b>当前没有脚本运行</b></span></p><br>");
        return;
    }

    // 强制停止逻辑
    if(ssjjMainThread->isFinished() == false){
        ssjjMainThread->stopThread();
        ssjjMainThread->terminate();
    }

    runningState = 0;
    writeRemindInfo("<p><span style=\"color:lightgreen;\"><b>脚本运行已结束</b></span></p><br>");
}

/* 执行单次加成任务 */
void SubSSJJWidget::on_singleBonusPushButton_clicked()
{
    ui->singleBonusPushButton->setEnabled(false);
    ui->contineBonusPushButton->setEnabled(false);
    
    /* 创建加成主线程 */
    weaponBonusThread = new WeaponBonusThread(this);

    /* 获取加成武器线程列表,并传入主线程 */
    getBounsWeaponList();

    /* 设置加成类型为单次加成 */
    weaponBonusThread->setBonusType(0);

    /* 接收线程执行情况信息 */
    // 成功结束
    connect(weaponBonusThread, &QThread::finished, this, [=]() {
        ui->singleBonusPushButton->setEnabled(true);
        ui->contineBonusPushButton->setEnabled(true);
        });
    // 线程意外终止
    connect(weaponBonusThread, &QThread::destroyed, this, [=]() {
        ui->singleBonusPushButton->setEnabled(true);
        ui->contineBonusPushButton->setEnabled(true);
        });

    /* 开启主线程 */
    weaponBonusThread->start();
}

/* 连续加成任务 */
void SubSSJJWidget::on_contineBonusPushButton_clicked()
{
    ui->contineBonusPushButton->setEnabled(false);
    ui->singleBonusPushButton->setEnabled(false);

    /* 创建加成主线程 */
    weaponBonusThread = new WeaponBonusThread(this);

    /* 获取加成武器线程列表,并传入主线程 */
    getBounsWeaponList();

    /* 设置加成类型为连续加成 */
    weaponBonusThread->setBonusType(1);

    /* 是否需要随机移动 */
    if (ui->randomMoveCheckBox->isChecked() == true)
    {
        weaponBonusThread->setIsRandomMove(true);
    }

    /* 是否发送消息 */
    if (ui->sendMessageCheckBox->isChecked() == true)
    {
        weaponBonusThread->setIsSendMessage(true);
    }

    /* 接收线程执行情况信息 */
    // 成功结束
    connect(weaponBonusThread, &QThread::finished, this, [=]() {
        ui->contineBonusPushButton->setEnabled(true);
        ui->singleBonusPushButton->setEnabled(true);
        });
    // 线程意外终止
    connect(weaponBonusThread, &QThread::destroyed, this, [=]() {
        ui->contineBonusPushButton->setEnabled(true);
        ui->singleBonusPushButton->setEnabled(true);
        });

    /* 开启主线程 */
    weaponBonusThread->start();
}

void SubSSJJWidget::on_stopBounsPushButton_clicked()
{
    if (weaponBonusThread != NULL)
    {
        if(weaponBonusThread->isFinished() == false)
        {
            weaponBonusThread->terminate();
        }
    }
}

void SubSSJJWidget::getSingleTask()
{
    for(int i = 0; i < ui->taskTableWidget->rowCount(); i++){
        if(ui->taskTableWidget->item(i, 3)->text().toInt() < ui->taskTableWidget->item(i, 2)->text().toInt()){
            currentTask.taskName = ui->taskTableWidget->item(i, 0)->text();
            currentTask.difficulty = ui->taskTableWidget->item(i, 1)->text();
            currentTask.script = ui->taskTableWidget->item(i, 4)->text();
            return;
        }
    }

    // 运行结束
    runningState = 0;
}

void SubSSJJWidget::receiveRemindInfo(QString remindInfo)
{
    writeRemindInfo(remindInfo);
}

/* 发送下一个任务 */
void SubSSJJWidget::sendNextTask(SSJJRunState res)
{
    /* 执行成功更新表格 */
    if(res.errorType == "Success"){
        for(int i = 0; i < ui->taskTableWidget->rowCount(); i++){
            if(ui->taskTableWidget->item(i, 3)->text().toInt() < ui->taskTableWidget->item(i, 2)->text().toInt()){
                ui->taskTableWidget->setItem(i, 3, new QTableWidgetItem(QString::number(ui->taskTableWidget->item(i, 3)->text().toInt() + 1)));
            }
        }
    }

    getSingleTask();

    if(runningState == 1){
        emit sendSingleTask(currentTask, ssjjInstallPath, ui->moveSpeedLineEdit->text().toInt(), ui->singleScriptTimeLineEdit->text().toInt(), ui->loadingTimeLineEdit->text().toInt());
    }
    else{
        // 强制停止逻辑
        if(ssjjMainThread->isFinished() == false){
            if(ssjjMainThread->currentThread != new QThread()){
                ssjjMainThread->stopThread();
            }
            ssjjMainThread->terminate();
        }
        writeRemindInfo("<p><span style=\"color:lightgreen;\"><b>全部任务已执行完毕</b></span></p><br>");
        writeRemindInfo("<p><span style=\"color:lightgreen;\"><b>脚本运行已结束</b></span></p><br>");
    }
}

void SubSSJJWidget::receiveFatalError()
{
    on_endPushButton_clicked();
}

void SubSSJJWidget::clearRow(QTableWidgetItem *item)
{
    int row = item->row();  // 获取当前点击的行
    ui->taskTableWidget->removeRow(row);  // 删除该行
}

void SubSSJJWidget::createScriptEditor()
{
    QLayout* qsciLayout = new QVBoxLayout;
    scriptEditor = new QsciScintilla(this);
    ui->qsciWidget->setLayout(qsciLayout);
    qsciLayout->addWidget(scriptEditor);
}

void SubSSJJWidget::createNodeEditor()
{
    QGraphicsScene* graphicsScene;

}

/* 更新武器列表 */
void SubSSJJWidget::updateCurrentWeaponList()
{
    for (int i = 0; i < ui->currentWeaponListWidget->count(); i++) 
    {
        if (ui->currentWeaponListWidget->item(i)->text().contains("主武器"))
        {
            ui->currentWeaponListWidget->item(i)->setText("主武器-" + ui->mainWeaponComboBox->currentText());
        }
        if (ui->currentWeaponListWidget->item(i)->text().contains("副武器"))
        {
            ui->currentWeaponListWidget->item(i)->setText("副武器-" + ui->secondaryWeaponComboBox->currentText());
        }
        if (ui->currentWeaponListWidget->item(i)->text().contains("近战武器"))
        {
            ui->currentWeaponListWidget->item(i)->setText("近战武器-" + ui->meleeWeaponComboBox->currentText());
        }
        if (ui->currentWeaponListWidget->item(i)->text().contains("投掷武器"))
        {
            ui->currentWeaponListWidget->item(i)->setText("投掷武器-" + ui->throwingWeaponComboBox->currentText());
        }
        if (ui->currentWeaponListWidget->item(i)->text().contains("战术武器"))
        {
            ui->currentWeaponListWidget->item(i)->setText("战术武器-" + ui->tacticalWeaponComboBox->currentText());
        }
        if (ui->currentWeaponListWidget->item(i)->text().contains("角色"))
        {
            ui->currentWeaponListWidget->item(i)->setText("角色-" + ui->characterComboBox->currentText());
        }
        if (ui->currentWeaponListWidget->item(i)->text().contains("龙骑兵"))
        {
            ui->currentWeaponListWidget->item(i)->setText("龙骑兵-" + ui->longQiComboBox->currentText());
        }
    }
}

/* 获取加成武器线程列表 */
void SubSSJJWidget::getBounsWeaponList()
{
    for (int i = 0; i < ui->currentWeaponListWidget->count(); i++)
    {
        if (ui->currentWeaponListWidget->item(i)->text().contains("主武器"))
        {
            // 主武器
            if (ui->mainWeaponComboBox->currentText() == "弧光")
            {
                // 弧光
                weaponBonusThread->addBonusWeapon(new HuGuang(weaponBonusThread));
            }
            if (ui->mainWeaponComboBox->currentText() == "神谕")
            {
                // 神谕
                weaponBonusThread->addBonusWeapon(new ShenYu(weaponBonusThread));
            }
            if (ui->mainWeaponComboBox->currentText() == "黎明之光")
            {
                // 黎明之光
                weaponBonusThread->addBonusWeapon(new LiMingZhiGuang(weaponBonusThread));
            }
            if(ui->mainWeaponComboBox->currentText() == "魅影")
            {
                // 魅影
                weaponBonusThread->addBonusWeapon(new MeiYing(weaponBonusThread));
            }
            if(ui->mainWeaponComboBox->currentText() == "裁决")
            {
                // 裁决
                weaponBonusThread->addBonusWeapon(new CaiJue(weaponBonusThread));
            }
        }
        if (ui->currentWeaponListWidget->item(i)->text().contains("副武器"))
        {
            // 副武器
            if (ui->secondaryWeaponComboBox->currentText() == "飞梭")
            {
                // 飞梭
                weaponBonusThread->addBonusWeapon(new FeiSuo(weaponBonusThread));
            }
            if (ui->secondaryWeaponComboBox->currentText() == "能防")
            {
                // 能防
                weaponBonusThread->addBonusWeapon(new NengFang(weaponBonusThread));
            }
            if (ui->secondaryWeaponComboBox->currentText() == "风神")
            {
                // 风神
                weaponBonusThread->addBonusWeapon(new FengShen(weaponBonusThread));
            }
        }
        if (ui->currentWeaponListWidget->item(i)->text().contains("近战武器"))
        {
            // 近战武器
            if (ui->meleeWeaponComboBox->currentText() == "黯水晶")
            {
                // 黯水晶
                weaponBonusThread->addBonusWeapon(new AnShuiJing(weaponBonusThread));
            }
            if (ui->meleeWeaponComboBox->currentText() == "撼天")
            {
                // 撼天
                weaponBonusThread->addBonusWeapon(new HanTian(weaponBonusThread));
            }
        }
        if (ui->currentWeaponListWidget->item(i)->text().contains("投掷武器"))
        {
            // 投掷武器
            if (ui->throwingWeaponComboBox->currentText() == "两仪")
            {
                // 两仪
                weaponBonusThread->addBonusWeapon(new LiangYi(weaponBonusThread));
            }
            if (ui->throwingWeaponComboBox->currentText() == "暗蚀")
            {
                // 暗蚀
                weaponBonusThread->addBonusWeapon(new AnShi(weaponBonusThread));
            }
            if(ui->throwingWeaponComboBox->currentText() == "天枢")
            {
                // 天枢
                weaponBonusThread->addBonusWeapon(new TianShu(weaponBonusThread));
            }
        }
        if (ui->currentWeaponListWidget->item(i)->text().contains("战术武器"))
        {
            // 战术武器
            if (ui->tacticalWeaponComboBox->currentText() == "灰核")
            {
                // 灰核
                weaponBonusThread->addBonusWeapon(new HuiHe(weaponBonusThread));
            }
        }
        if (ui->currentWeaponListWidget->item(i)->text().contains("角色"))
        {
            // 角色
            if (ui->characterComboBox->currentText() == "六臂魔童（无中键）")
            {
                // 六臂魔童（无中键）
                weaponBonusThread->addBonusWeapon(new LiuBi(weaponBonusThread));
            }
            if (ui->characterComboBox->currentText() == "泰克斯（有X）")
            {
                // 泰克斯（有X）
                weaponBonusThread->addBonusWeapon(new TaiKeSiX(weaponBonusThread));
            }
        }
        if (ui->currentWeaponListWidget->item(i)->text().contains("龙骑兵"))
        {
            // 龙骑兵
            if (ui->longQiComboBox->currentText() == "混沌爆发")
            {
                // 混沌爆发
                weaponBonusThread->addBonusWeapon(new HunDunBaoFa(weaponBonusThread));
            }
        }
    }
}

/* 保存加成设置 */
void SubSSJJWidget::saveBounsSettings(QString filePath)
{
    QJsonObject jsonObject;

    for (int i = 0; i < ui->currentWeaponListWidget->count(); i++) {
        QString itemText = ui->currentWeaponListWidget->item(i)->text();

        if (itemText.contains("主武器")) {
            // 主武器
            QJsonObject mainWeapon;
            mainWeapon["name"] = ui->mainWeaponComboBox->currentText();
            mainWeapon["order"] = i;
            jsonObject["mainWeapon"] = mainWeapon;
        }
        else if (itemText.contains("副武器")) {
            // 副武器
            QJsonObject secondaryWeapon;
            secondaryWeapon["name"] = ui->secondaryWeaponComboBox->currentText();
            secondaryWeapon["order"] = i;
            jsonObject["secondaryWeapon"] = secondaryWeapon;
        }
        else if (itemText.contains("近战武器")) {
            // 近战武器
            QJsonObject meleeWeapon;
            meleeWeapon["name"] = ui->meleeWeaponComboBox->currentText();
            meleeWeapon["order"] = i;
            jsonObject["meleeWeapon"] = meleeWeapon;
        }
        else if (itemText.contains("投掷武器")) {
            // 投掷武器
            QJsonObject throwingWeapon;
            throwingWeapon["name"] = ui->throwingWeaponComboBox->currentText();
            throwingWeapon["order"] = i;
            jsonObject["throwingWeapon"] = throwingWeapon;
        }
        else if (itemText.contains("战术武器")) {
            // 战术武器
            QJsonObject tacticalWeapon;
            tacticalWeapon["name"] = ui->tacticalWeaponComboBox->currentText();
            tacticalWeapon["order"] = i;
            jsonObject["tacticalWeapon"] = tacticalWeapon;
        }
        else if (itemText.contains("角色")) {
            // 角色
            QJsonObject character;
            character["name"] = ui->characterComboBox->currentText();
            character["order"] = i;
            jsonObject["character"] = character;
        }
        else if (itemText.contains("龙骑兵")) {
            // 龙骑兵
            QJsonObject longQi;
            longQi["name"] = ui->longQiComboBox->currentText();
            longQi["order"] = i;
            jsonObject["longQi"] = longQi;
        }
    }

    QJsonDocument jsonDoc(jsonObject);
    QFile file(filePath);
    if (file.open(QIODevice::WriteOnly)) {
        file.write(jsonDoc.toJson());
        file.close();
    }
}

void SubSSJJWidget::loadBounsSettings(QString filePath)
{
    QFile file(filePath);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        qWarning() << "Unable to open file for reading:" << filePath;
        return;
    }

    QByteArray fileData = file.readAll();
    file.close();

    QJsonDocument jsonDoc(QJsonDocument::fromJson(fileData));
    if (jsonDoc.isNull() || !jsonDoc.isObject()) {
        qWarning() << "Invalid JSON file format:" << filePath;
        return;
    }

    QJsonObject jsonObject = jsonDoc.object();
    struct WeaponInfo {
        QString type;
        QString name;
        int order;
    };

    QList<WeaponInfo> weaponList;

    // Retrieve each weapon type and store its data in the list with order
    if (jsonObject.contains("mainWeapon")) {
        QJsonObject mainWeapon = jsonObject["mainWeapon"].toObject();
        ui->mainWeaponComboBox->setCurrentText(mainWeapon["name"].toString());
        weaponList.append({ "主武器", mainWeapon["name"].toString(), mainWeapon["order"].toInt() });
    }
    if (jsonObject.contains("secondaryWeapon")) {
        QJsonObject secondaryWeapon = jsonObject["secondaryWeapon"].toObject();
        ui->secondaryWeaponComboBox->setCurrentText(secondaryWeapon["name"].toString());
        weaponList.append({ "副武器", secondaryWeapon["name"].toString(), secondaryWeapon["order"].toInt() });
    }
    if (jsonObject.contains("meleeWeapon")) {
        QJsonObject meleeWeapon = jsonObject["meleeWeapon"].toObject();
        ui->meleeWeaponComboBox->setCurrentText(meleeWeapon["name"].toString());
        weaponList.append({ "近战武器", meleeWeapon["name"].toString(), meleeWeapon["order"].toInt() });
    }
    if (jsonObject.contains("throwingWeapon")) {
        QJsonObject throwingWeapon = jsonObject["throwingWeapon"].toObject();
        ui->throwingWeaponComboBox->setCurrentText(throwingWeapon["name"].toString());
        weaponList.append({ "投掷武器", throwingWeapon["name"].toString(), throwingWeapon["order"].toInt() });
    }
    if (jsonObject.contains("tacticalWeapon")) {
        QJsonObject tacticalWeapon = jsonObject["tacticalWeapon"].toObject();
        ui->tacticalWeaponComboBox->setCurrentText(tacticalWeapon["name"].toString());
        weaponList.append({ "战术武器", tacticalWeapon["name"].toString(), tacticalWeapon["order"].toInt() });
    }
    if (jsonObject.contains("character")) {
        QJsonObject character = jsonObject["character"].toObject();
        ui->characterComboBox->setCurrentText(character["name"].toString());
        weaponList.append({ "角色", character["name"].toString(), character["order"].toInt() });
    }
    if (jsonObject.contains("longQi")) {
        QJsonObject longQi = jsonObject["longQi"].toObject();
        ui->longQiComboBox->setCurrentText(longQi["name"].toString());
        weaponList.append({ "龙骑兵", longQi["name"].toString(), longQi["order"].toInt() });
    }

    // Sort weapons by order
    std::sort(weaponList.begin(), weaponList.end(), [](const WeaponInfo& a, const WeaponInfo& b) {
        return a.order < b.order;
        });

    // Clear current list widget and add items in sorted order
    ui->currentWeaponListWidget->clear();
    for (const WeaponInfo& weapon : weaponList) {
        QString displayText = QString("%1-%2").arg(weapon.type, weapon.name);
        ui->currentWeaponListWidget->addItem(displayText);
    }
}

void SubSSJJWidget::on_saveBonusConfigPushButton_clicked()
{
    // 确定保存文件夹路径（程序当前运行目录下的 bonusConfig 文件夹）
    QString saveDir = QDir(QCoreApplication::applicationDirPath()).filePath("bonusConfig");

    // 如果文件夹不存在，创建文件夹
    QDir dir;
    if (!dir.exists(saveDir)) {
        dir.mkpath(saveDir);
    }

    // 打开文件对话框，获取保存文件名（不含路径）
    QString fileName = QFileDialog::getSaveFileName(
        this,
        tr("Save Bonus Configuration"),
        saveDir + "/bonus_config.json",  // 默认保存路径和文件名
        tr("JSON Files (*.json)")  // 文件类型
    );

    // 检查用户是否选择了文件名
    if (fileName.isEmpty()) {
        return;  // 用户取消选择，直接返回
    }

    // 如果文件名没有 .json 扩展名，则添加
    if (!fileName.endsWith(".json", Qt::CaseInsensitive)) {
        fileName += ".json";
    }

    // 调用保存函数，将数据保存为 JSON 格式
    saveBounsSettings(fileName);

    // 输出保存路径
    qDebug() << "File saved to:" << fileName;

    // 更新列表内容
    readBonusJsonFiles();
}

void SubSSJJWidget::readBonusJsonFiles()
{
    // 获取当前运行目录下的 bonusConfig 文件夹路径
    QString configDirPath = QDir(QCoreApplication::applicationDirPath()).filePath("bonusConfig");

    // 确认文件夹存在
    QDir configDir(configDirPath);
    if (!configDir.exists(configDirPath)) {
        configDir.mkpath(configDirPath);
    }

    // 获取 bonusConfig 文件夹下所有 .json 文件名
    QStringList jsonFiles = configDir.entryList(QStringList() << "*.json", QDir::Files);

    // 将文件名列表显示到 bonusConfigListListView 中
    auto* model = new QStringListModel(this);
    model->setStringList(jsonFiles);
    ui->bonusConfigListListView->setModel(model);
}
