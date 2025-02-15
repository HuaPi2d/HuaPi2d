#include "subssjjwidget.h"
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
#include "thread/weapons/mainWeapons/liexi.h"
#include "thread/weapons/mainWeapons/suixing.h"
#include "thread/weapons/secondaryWeapons/feisuo.h"
#include "thread/weapons/secondaryWeapons/nengfang.h"
#include "thread/weapons/secondaryWeapons/fengshen.h"
#include "thread/weapons/meleeWeapons/anshuijing.h"
#include "thread/weapons/meleeWeapons/hantian.h"
#include "thread/weapons/meleeWeapons/moyunhe.h"
#include "thread/weapons/tacticalWeapons/huihe.h"
#include "thread/weapons/throwingWeapons/liangyi.h"
#include "thread/weapons/throwingWeapons/anshi.h"
#include "thread/weapons/throwingWeapons/tianshu.h"
#include "thread/weapons/character/liubi.h"
#include "thread/weapons/character/lunhua.h"
#include "thread/weapons/character/taikesix.h"
#include "thread/weapons/longQi/hundunbaofa.h"


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

    /* 搭建图形编辑器 */
    createNodeEditor();

    up_arrow = new QHotkey(QKeySequence(Qt::Key_Up), this);
    down_arrow = new QHotkey(QKeySequence(Qt::Key_Down), this);
    left_arrow = new QHotkey(QKeySequence(Qt::Key_Left), this);
    right_arrow = new QHotkey(QKeySequence(Qt::Key_Right), this);
    enter = new QHotkey(QKeySequence("Enter"), this);
    middle = new QHotkey(QKeySequence("/"), this);

    connect(left_arrow, &QHotkey::activated, [=]() {
        qDebug() << "left arrow";
        turn_around(45, false);
        });
    connect(right_arrow, &QHotkey::activated, [=]() {
        turn_around(-45, false);
        });
    connect(up_arrow, &QHotkey::activated, [=]() {
        turn_up_down(30, false);
        });
    connect(down_arrow, &QHotkey::activated, [=]() {
        turn_up_down(-30, false);
        });
    bool isShooting = false;
    connect(enter, &QHotkey::activated, [&]() {
        if (isShooting) {
            isShooting = false;
            LeftDown();
        }
        else {
            isShooting = true;
            LeftUp();
        }
        });
    connect(middle, &QHotkey::activated, [=]() {
        MiddleClick();
        });

    connect(ui->testKeyMouseCheckBox, &QCheckBox::clicked, [=](bool checked) {
        if (checked) {
            regiseterMouseHotkey();
        }
        else {
            unregiseterMouseHotkey();
        }
        });

    // 注销鼠标热键
    ifCanControlMouse = true;
    unregiseterMouseHotkey();

    /* 脚本框设置 */
    ui->remindTextEdit->setReadOnly(true);
    ui->taskTableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);
    writeRemindInfo("<p>" + tr("欢迎使用") + "<b>" + tr("生死狙击脚本") + "</b>" + tr("工具") + "</p><p><span style=\"font-size: 13px; color: red;\">" 
        + tr("使用时注意本栏信息") + "</span></p><p>" + tr("请开启") + "<span style = \"font-size: 13px; color: red;\"><b>" + tr("自动登录") + "</b></span><br><span>游戏内鼠标灵敏度设为40</span><br></p>");
    ui->launcherPathLineEdit->setReadOnly(true);
    ui->LDScriptPathLineEdit->setReadOnly(true);
    QIntValidator *validator = new QIntValidator(0, 999, this);
    ui->moveSpeedLineEdit->setValidator(validator);
    ui->LDRunTimesLineEdit->setValidator(validator);
    ui->zxRunTimesLineEdit->setValidator(validator);
    QIntValidator *loadingTimeValidator = new QIntValidator(0, 99, this);
    ui->loadingTimeLineEdit->setValidator(loadingTimeValidator);
    QIntValidator *validatorScriptTime = new QIntValidator(0, 20, this);
    ui->singleScriptTimeLineEdit->setValidator(validatorScriptTime);

    /* 设置显示在屏幕上的文本label标签样式 */
    outlineStyleSheet = "QLabel{border: 2px solid red; border-radius: 5px; padding: 5px; background-color: white; color: black;}";
    cv::Point cvPoint = getScreenResolution();
    float scale = getScreenScale();
    centerPoint = QPoint(cvPoint.x / 2 / scale, cvPoint.y / 2 / scale);

    /* 热键设置 */
    runningState = 0;

    /* 连接combox更新信号 */
    connect(ui->mainWeaponComboBox, &QComboBox::currentIndexChanged, this, &SubSSJJWidget::updateCurrentWeaponList);
    connect(ui->secondaryWeaponComboBox, &QComboBox::currentIndexChanged, this, &SubSSJJWidget::updateCurrentWeaponList);
    connect(ui->meleeWeaponComboBox, &QComboBox::currentIndexChanged, this, &SubSSJJWidget::updateCurrentWeaponList);
    connect(ui->throwingWeaponComboBox, &QComboBox::currentIndexChanged, this, &SubSSJJWidget::updateCurrentWeaponList);
    connect(ui->tacticalWeaponComboBox, &QComboBox::currentIndexChanged, this, &SubSSJJWidget::updateCurrentWeaponList);
    connect(ui->characterComboBox, &QComboBox::currentIndexChanged, this, &SubSSJJWidget::updateCurrentWeaponList);
    connect(ui->longQiComboBox, &QComboBox::currentIndexChanged, this, &SubSSJJWidget::updateCurrentWeaponList);
    connect(textToShowInScreen, &GlobalVariableQString::valueChanged, this, &SubSSJJWidget::receiveDisplayText);

    /* 分辨率选择信息 */
    connect(ui->radioButton25601440, &QRadioButton::clicked, this, [=]() {resolutionPath = ""; });
    connect(ui->radioButton25601600, &QRadioButton::clicked, this, [=]() {resolutionPath = "2560-1600/"; });
    connect(ui->radioButtonVM, &QRadioButton::clicked, this, [=]() {resolutionPath = "VM/"; });

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

    /* 设置脚本编辑界面 */
    // 设置标签页为可关闭
    ui->scriptalTabWidget->setTabsClosable(true);
    ui->fileAttributesTableWidget->verticalHeader()->setVisible(false);
    ui->fileAttributesTableWidget->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->fileAttributesTableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);
    
    // 连接脚本编辑界面信号和槽
    connect(ui->scriptalTabWidget, &QTabWidget::tabCloseRequested, this, &SubSSJJWidget::closeTab);
    connect(ui->scriptalTabWidget, &QTabWidget::currentChanged, this, &SubSSJJWidget::currentTabChanged);

    // 更新语言
    connect(Language, &GlobalVariableQString::valueChanged, this, [=]() {
        ui->retranslateUi(this);
        });
    reloadLanguage(Language->value());

    // 更新主题
    connect(themeType, &GlobalVariableQString::valueChanged, this, [=]() {
        resetEditorsAppearances();
        });

    /* 主线关卡标签页的Combox的联动更新 */
    connect(ui->zxChapterChooseComboBox, &QComboBox::currentTextChanged, this, [=]() {
        currentChoosedZXChapter = ZXGameData::getChapterByName(ui->zxChapterChooseComboBox->currentText());
        updateZXLevelChooseComboBox();
        });
    connect(ui->zxLevelComboBox, &QComboBox::currentTextChanged, this, [=]() {
        for(ZXLevel level: currentChoosedZXChapter.levels){
            if(level.name == ui->zxLevelComboBox->currentText()){
                currentChoosedZXLevel = level;
                break;
            }
        }
        updateZXDiffcultyChooseComboBox();
        updateZXScriptPathComboBox();
        });

    writeRemindInfo("<p><span style=\"color: red;\">" + tr("请确保在每次启动游戏时不会弹出以下图片") + "</p><br>");
    writeRemindInfo("<img src=\"" + tips_file_path("tips/ensure_ratio.png") + "\" width=\"250\" alt=\"" + tr("提示图片") + "\"><br>");

    // 创建数据库
    ssjjScriptalFilesDatabase = new SSJJScriptalFilesDatabase("ssjjScriptalFiles.db", this);
}

SubSSJJWidget::~SubSSJJWidget()
{
    // 终止线程
    if(checkThreadRunningState(ssjjMainThread) == 1)
        forceQuitThread(ssjjMainThread);
    if(checkThreadRunningState(testScriptThread) == 1)
        forceQuitThread(testScriptThread);
    saveSettings();
    delete ui;
}

void SubSSJJWidget::closeEvent(QCloseEvent *event)
{
    emit widgetClosed();
}

// 保存配置
void SubSSJJWidget::saveSettings()
{
    /* 声明对象 */
    UsersSettings settings(qApp->applicationDirPath() + "/Settings/ssjjSettings.ini");

    /* 写入配置 */
    settings.beginGroup("SSJJ_Script");
    settings.setValue("ssjjInstallPath", ssjjInstallPath);
    settings.setValue("moveSpeed", ui->moveSpeedLineEdit->text());
    settings.setValue("LDRunTimes", ui->LDRunTimesLineEdit->text());
    settings.setValue("LDScriptPath", ui->LDScriptPathLineEdit->text());
    settings.setValue("singleScriptTime", ui->singleScriptTimeLineEdit->text());
    settings.setValue("loadingTime", ui->loadingTimeLineEdit->text());
    settings.setValue("ifResetTimes", ui->ifResetTimesCheckBox->isChecked());
    settings.setValue("passWordMode", passWordMode);
    settings.setValue("enterGamePassWord", enterGamePassWord);
    settings.setValue("tabWidgetPage", ui->tabWidget->currentIndex());

    settings.beginGroup("bonusWeapenList");
    settings.setValue("mainWeaponList", ui->mainWeaponComboBox->currentText());
    settings.setValue("secondaryWeaponList", ui->secondaryWeaponComboBox->currentText());
    settings.setValue("meleeWeaponList", ui->meleeWeaponComboBox->currentText());
    settings.setValue("throwingWeaponList", ui->throwingWeaponComboBox->currentText());
    settings.setValue("tacticalWeaponList", ui->tacticalWeaponComboBox->currentText());
    settings.setValue("characterList", ui->characterComboBox->currentText());
    settings.setValue("longQiList", ui->longQiComboBox->currentText());
    settings.setValue("currentPage", widgetList.indexOf(currentWidget));
    settings.setValue("resolutionPath", this->resolutionPath);
    QStringList weaponList;
    for (int i = 0; i < ui->currentWeaponListWidget->count(); i++)
    {
        weaponList.append(ui->currentWeaponListWidget->item(i)->text());
    }
    settings.setValue("currentWeaponList", weaponList);
    settings.endGroup();

    settings.beginGroup("scriptEditor");
    QString fileName;
    QString savePath;
    QStringList openFiles;
    for (ScpLanguageEditor* editor : scpLanguageEditors)
    {
        fileName = editor->getFileName();
        savePath = editor->getSavePath();
        openFiles.append(savePath + "/" + fileName);
    }
    settings.setValue("openFiles", openFiles);
    settings.setValue("currentScriptEditorTabIndex", ui->scriptalTabWidget->currentIndex());
    settings.setValue("ifShowRecordTeachingDialog", ifShowRecordTeachingDialog);
    settings.endGroup();

    // 保存主线关卡设置
    settings.beginGroup("zxSetttings");
    settings.setValue("chapter", ui->zxChapterChooseComboBox->currentText());
    settings.setValue("level", ui->zxLevelComboBox->currentText());
    settings.setValue("difficulty", ui->diffcultyChooseComboBox->currentText());
    settings.setValue("runTimes", ui->zxRunTimesLineEdit->text());
    settings.setValue("scriptPath", ui->zxChapterChooseComboBox->currentText());
    settings.endGroup();

    settings.endGroup();


    // 任务数据
    exportToXlsx(ui->taskTableWidget, qApp->applicationDirPath() + "/Settings/taskList.xlsx");

    if (1) {
        /* 声明对象 */
        UsersSettings editorConfigSettings(qApp->applicationDirPath() + "/Settings/editorConfig.ini");

        /* 写入编辑器配置 */
        editorConfigSettings.beginGroup("scpEditorConfig_dark_dark");
        editorConfigSettings.setValue("font", scpEditorConfig_dark.font.font.toString());
        editorConfigSettings.setValue("fontSize", scpEditorConfig_dark.font.size);
        editorConfigSettings.beginGroup("color");
        editorConfigSettings.setValue("background", scpEditorConfig_dark.color.background.name());
        editorConfigSettings.setValue("text", scpEditorConfig_dark.color.text.name());
        editorConfigSettings.setValue("keyword", scpEditorConfig_dark.color.keyword.name());
        editorConfigSettings.setValue("number", scpEditorConfig_dark.color.number.name());
        editorConfigSettings.setValue("string", scpEditorConfig_dark.color.string.name());
        editorConfigSettings.setValue("operateur", scpEditorConfig_dark.color.operateur.name());
        editorConfigSettings.setValue("function", scpEditorConfig_dark.color.function.name());
        editorConfigSettings.setValue("variable", scpEditorConfig_dark.color.variable.name());
        editorConfigSettings.setValue("comment", scpEditorConfig_dark.color.comment.name());
        editorConfigSettings.setValue("caret_line_color", scpEditorConfig_dark.color.caret_line_color.name());
        editorConfigSettings.setValue("line_number_color", scpEditorConfig_dark.margin.line_number_color.name());
        editorConfigSettings.setValue("background_color", scpEditorConfig_dark.margin.background_color.name());
        editorConfigSettings.endGroup();
        editorConfigSettings.endGroup();

        editorConfigSettings.beginGroup("scpEditorConfig_light_light");
        editorConfigSettings.setValue("font", scpEditorConfig_light.font.font.toString());
        editorConfigSettings.setValue("fontSize", scpEditorConfig_light.font.size);
        editorConfigSettings.beginGroup("color");
        editorConfigSettings.setValue("background", scpEditorConfig_light.color.background.name());
        editorConfigSettings.setValue("text", scpEditorConfig_light.color.text.name());
        editorConfigSettings.setValue("keyword", scpEditorConfig_light.color.keyword.name());
        editorConfigSettings.setValue("number", scpEditorConfig_light.color.number.name());
        editorConfigSettings.setValue("string", scpEditorConfig_light.color.string.name());
        editorConfigSettings.setValue("operateur", scpEditorConfig_light.color.operateur.name());
        editorConfigSettings.setValue("function", scpEditorConfig_light.color.function.name());
        editorConfigSettings.setValue("variable", scpEditorConfig_light.color.variable.name());
        editorConfigSettings.setValue("comment", scpEditorConfig_light.color.comment.name());
        editorConfigSettings.setValue("caret_line_color", scpEditorConfig_light.color.caret_line_color.name());
        editorConfigSettings.setValue("line_number_color", scpEditorConfig_light.margin.line_number_color.name());
        editorConfigSettings.setValue("background_color", scpEditorConfig_light.margin.background_color.name());
        editorConfigSettings.endGroup();
        editorConfigSettings.endGroup();
    }
}

// 读取配置
void SubSSJJWidget::loadSettings()
{
    if (1) {
        // 全局编辑器配置
    /* 声明对象 */
        UsersSettings editorConfigSettings(qApp->applicationDirPath() + "/Settings/editorConfig.ini");

        /* 读取编辑器配置 */
        //// 编辑器配置
        editorConfigSettings.beginGroup("scpEditorConfig_dark");
        scpEditorConfig_dark.font.font = editorConfigSettings.value("font", QFont("Consolas")).value<QFont>();
        scpEditorConfig_dark.font.size = editorConfigSettings.value("fontSize", 12).toInt();
        editorConfigSettings.beginGroup("color");
        scpEditorConfig_dark.color.background = QColor(editorConfigSettings.value("background", "#2b2b2b").toString());
        scpEditorConfig_dark.color.foreground = QColor(editorConfigSettings.value("foreground", "#FAFAFA").toString());
        scpEditorConfig_dark.color.text = QColor(editorConfigSettings.value("text", "#FFFFFF").toString());
        scpEditorConfig_dark.color.keyword = QColor(editorConfigSettings.value("keyword", "#cb7731").toString());
        scpEditorConfig_dark.color.number = QColor(editorConfigSettings.value("number", "#1793D0").toString());
        scpEditorConfig_dark.color.string = QColor(editorConfigSettings.value("string", "#6a8759").toString());
        scpEditorConfig_dark.color.operateur = QColor(editorConfigSettings.value("operateur", "#FF8F00").toString());
        scpEditorConfig_dark.color.function = QColor(editorConfigSettings.value("function", "#FF3DB5").toString());
        scpEditorConfig_dark.color.variable = QColor(editorConfigSettings.value("variable", "#a9b7c6").toString());
        scpEditorConfig_dark.color.comment = QColor(editorConfigSettings.value("comment", "#3ef112").toString());
        scpEditorConfig_dark.color.caret_line_color = QColor(editorConfigSettings.value("caret_line_color", "#323232").toString());
        scpEditorConfig_dark.margin.line_number_color = QColor(editorConfigSettings.value("line_number_color", "#8a8a8a").toString());
        scpEditorConfig_dark.margin.background_color = QColor(editorConfigSettings.value("background_color", "#313335").toString());
        editorConfigSettings.endGroup();
        editorConfigSettings.endGroup();

        editorConfigSettings.beginGroup("scpEditorConfig_light");
        scpEditorConfig_light.font.font = editorConfigSettings.value("font", QFont("Consolas")).value<QFont>();
        scpEditorConfig_light.font.size = editorConfigSettings.value("fontSize", 12).toInt();
        editorConfigSettings.beginGroup("color");
        scpEditorConfig_light.color.background = QColor(editorConfigSettings.value("background", "#FFFFFF").toString());
        scpEditorConfig_light.color.foreground = QColor(editorConfigSettings.value("foreground", "#000000").toString());
        scpEditorConfig_light.color.text = QColor(editorConfigSettings.value("text", "#000000").toString());
        scpEditorConfig_light.color.keyword = QColor(editorConfigSettings.value("keyword", "#0000FF").toString());
        scpEditorConfig_light.color.number = QColor(editorConfigSettings.value("number", "#008000").toString());
        scpEditorConfig_light.color.string = QColor(editorConfigSettings.value("string", "#008080").toString());
        scpEditorConfig_light.color.operateur = QColor(editorConfigSettings.value("operateur", "#800080").toString());
        scpEditorConfig_light.color.function = QColor(editorConfigSettings.value("function", "#0000FF").toString());
        scpEditorConfig_light.color.variable = QColor(editorConfigSettings.value("variable", "#000000").toString());
        scpEditorConfig_light.color.comment = QColor(editorConfigSettings.value("comment", "#808000").toString());
        scpEditorConfig_light.color.caret_line_color = QColor(editorConfigSettings.value("caret_line_color", "#FFFFFF").toString());
        scpEditorConfig_light.margin.line_number_color = QColor(editorConfigSettings.value("line_number_color", "#808080").toString());
        scpEditorConfig_light.margin.background_color = QColor(editorConfigSettings.value("background_color", "#FFFFFF").toString());
        editorConfigSettings.endGroup();
        editorConfigSettings.endGroup();
    }


    /* 声明对象 */
    UsersSettings settings(qApp->applicationDirPath() + "/Settings/ssjjSettings.ini");

    /* 写入配置 */
    settings.beginGroup("SSJJ_Script");
    // 生死狙击程序安装路径
    if(settings.value("ssjjInstallPath").toString() != "")
    {
        ssjjInstallPath = settings.value("ssjjInstallPath").toString();
        writeRemindInfo("<p>" + tr("生死狙击程序安装路径:") + "</p><b>" + ssjjInstallPath + "</b><br><br>");
    }
    else
    {
        ssjjInstallPath = getRegDitValue("\\HKEY_CURRENT_USER\\Software\\Wooduan\\SSJJ-4399", "InstallPath") + "\\WDlauncher.exe";
        if( ssjjInstallPath == ""){
            writeRemindInfo("<p><span style=\"font-size: 13px; color: red;\">" + tr("生死狙击程序安装路径读取失败，请手动添加") + "<b>WDlauncher.exe</b>" + tr("的路径") + "</span></p><br>");
            writeRemindInfo("<br><img src=\"" + tips_file_path("tips/choose_launcher_tip.png") + "\" width=\"250\" alt=\"" + tr("提示图片") + "\"><br>");
        }
        else{
            writeRemindInfo("<p>" + tr("生死狙击程序安装路径:") + "<b>" + ssjjInstallPath + "</b></p><br>");
        }
    }
    ui->launcherPathLineEdit->setText(ssjjInstallPath);
    // 移动速度
    ui->moveSpeedLineEdit->setText(settings.value("moveSpeed", "100").toString());
    // 乱斗次数
    ui->LDRunTimesLineEdit->setText(settings.value("LDRunTimes", "100").toString());
    // 乱斗脚本路径
    ui->LDScriptPathLineEdit->setText(settings.value("LDScriptPath").toString());
    // 单次脚本超时时长
    ui->singleScriptTimeLineEdit->setText(settings.value("singleScriptTime", "5").toString());
    // 加载时间
    ui->loadingTimeLineEdit->setText(settings.value("loadingTime", "11").toString());
    // 是否重置次数
    ui->ifResetTimesCheckBox->setChecked(settings.value("ifResetTimes", false).toBool());
    // 房间密码模式
    passWordMode = settings.value("passWordMode", 1).toInt();
    // 房间密码
    enterGamePassWord = settings.value("enterGamePassWord", "HuaPi2D").toString();
    // tabWidget
    ui->tabWidget->setCurrentIndex(settings.value("tabWidgetPage", "0").toInt());

    settings.beginGroup("bonusWeapenList");
    // 加载武器 combox 内容
    ui->mainWeaponComboBox->setCurrentText(settings.value("mainWeaponList").toString());
    ui->secondaryWeaponComboBox->setCurrentText(settings.value("secondaryWeaponList").toString());
    ui->meleeWeaponComboBox->setCurrentText(settings.value("meleeWeaponList").toString());
    ui->throwingWeaponComboBox->setCurrentText(settings.value("throwingWeaponList").toString());
    ui->tacticalWeaponComboBox->setCurrentText(settings.value("tacticalWeaponList").toString());
    ui->characterComboBox->setCurrentText(settings.value("characterList").toString());
    ui->longQiComboBox->setCurrentText(settings.value("longQiList").toString());
    if (settings.value("currentWeaponList").toStringList() != QStringList()) {
        for (QString str : settings.value("currentWeaponList").toStringList()) {
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
    // 转到上次关闭时界面
    currentWidget = widgetList[settings.value("currentPage", "0").toInt()];
    updateScreen();
    // 选中分辨率
    this->resolutionPath = settings.value("resolutionPath").toString();
    if (this->resolutionPath == "")
        ui->radioButton25601440->setChecked(true);
    else if (this->resolutionPath == "2560-1600/")
        ui->radioButton25601600->setChecked(true);
    else if (this->resolutionPath == "VM/")
        ui->radioButtonVM->setChecked(true);
    settings.endGroup();

    settings.beginGroup("scriptEditor");
    if (settings.value("openFiles").toStringList() != QStringList()) {
        for (QString filePath : settings.value("openFiles").toStringList()) {
            QFileInfo fileInfo(filePath);
            // 判断文件是否被删除或改名
            QFile file(fileInfo.absoluteFilePath());
            if (!file.exists()) {
                QMessageBox::StandardButton button = QMessageBox::warning(this, tr("错误"),
                    file.fileName() + tr("文件不在") + filePath + tr("路径下，是否重新选择文件所在位置？"), QMessageBox::Yes | QMessageBox::No, QMessageBox::Yes);
                if (button == QMessageBox::Yes)
                {
                    // 选择新文件，文件类型与当前文件相同
                    QString newFilePath = QFileDialog::getOpenFileName(this, tr("选择文件"), QDir::currentPath(), tr("脚本文件(*.scp);;乱斗脚本(*.lscp);;主线脚本(*.zscp)"));
                    if (newFilePath.isEmpty()) {
                        return;
                    }
                    else {
                        QFileInfo newfileInfo(newFilePath);
                        this->creatNewScriptEditorTab(newfileInfo.fileName(), newfileInfo.path(), readFileAttributes(newfileInfo.absoluteFilePath()));
                    }
                }
                else {
                    return;
                }
            }
            this->creatNewScriptEditorTab(fileInfo.fileName(), fileInfo.path(), readFileAttributes(fileInfo.absoluteFilePath()));
        }
    }
    ui->scriptalTabWidget->setCurrentIndex(settings.value("currentScriptEditorTabIndex", "0").toInt());
    ifShowRecordTeachingDialog = settings.value("ifShowRecordTeachingDialog", true).toBool();
    settings.endGroup();

    // 读取主线关卡设置
    settings.beginGroup("zxSetttings");
    // 读取章节置 zxChapterChooseComboBox 内
    for (ZXChapter chapter : ZXGameData::getChapters())
        ui->zxChapterChooseComboBox->addItem(chapter.name);
    // 章节设置
    if (settings.value("chapter").toString() != "") 
        ui->zxChapterChooseComboBox->setCurrentText(settings.value("chapter").toString());
    else 
        ui->zxChapterChooseComboBox->setCurrentIndex(0);
    currentChoosedZXChapter = ZXGameData::getChapterByName(ui->zxChapterChooseComboBox->currentText());
    updateZXLevelChooseComboBox();
    // 读取关卡置 zxLevelComboBox 内
    if (settings.value("level").toString() != "") 
        ui->zxLevelComboBox->setCurrentText(settings.value("level").toString());
    else 
        ui->zxLevelComboBox->setCurrentIndex(0);
    for (ZXLevel level : currentChoosedZXChapter.levels)
    {
        if (level.name == ui->zxLevelComboBox->currentText())
        {
            currentChoosedZXLevel = level;
            break;
        }
    }
    updateZXDiffcultyChooseComboBox();
    // 读取关卡难度至 diffcultyChooseComboBox 内
    if (settings.value("difficulty").toString() != "") 
        ui->diffcultyChooseComboBox->setCurrentText(settings.value("difficulty").toString());
    else 
        ui->diffcultyChooseComboBox->setCurrentIndex(0);
    if (settings.value("runTimes").toString() != "") 
        ui->zxRunTimesLineEdit->setText(settings.value("runTimes").toString());
    else 
        ui->zxRunTimesLineEdit->setText("3");

    // 读取当前数据库脚本文件至 zxScriptPathComboBox 内
    updateZXScriptPathComboBox();
    if (settings.value("scriptPath").toString() != "") 
        ui->zxScriptPathComboBox->setCurrentText(settings.value("scriptPath").toString());
    else
        ui->zxScriptPathComboBox->setCurrentIndex(0);
    
    settings.endGroup();
    settings.endGroup();

    importFromXlsx(ui->taskTableWidget, qApp->applicationDirPath() + "/Settings/taskList.xlsx");

    readBonusJsonFiles();
    hideSomeItems();
}

// 隐藏某些控件
void SubSSJJWidget::hideSomeItems()
{
    if (developerMode == false)
    {
        ui->testPushButton->hide();
        ui->terminateUnityPushButton->hide();
        ui->nodeEditorPushButton->hide();
        ui->localMusicPushButton->hide();
        ui->terminateWDLaucherPushButton->hide();
        ui->terminateMicroClientPushButton->hide();
    }
}

void SubSSJJWidget::on_testPushButton_clicked()
{
    //unregiseterDirectionHotkey();
    convertRecordResult();
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

void SubSSJJWidget::on_terminateUnityPushButton_clicked()
{
    if (TerminateProcessByNameAndCheck("SSJJ_BattleClient_Unity.exe"))
    {
        writeRemindInfo("<p>" + tr("已销毁SSJJ_BattleClient_Unity.exe进程") + "</p><br>");
    }
    else
    {
        writeRemindInfo("<p>" + tr("销毁SSJJ_BattleClient_Unity.exe进程失败") + "</p><br>");
    }
}

void SubSSJJWidget::on_terminateMicroClientPushButton_clicked()
{
    if (TerminateProcessByNameAndCheck("MicroClient.exe"))
    {
        writeRemindInfo("<p>" + tr("已销毁MicroClient.exe进程") + "</p><br>");
    }
    else
    {
        writeRemindInfo("<p>" + tr("销毁MicroClient.exe进程失败") + "</p><br>");
    }
}

void SubSSJJWidget::on_terminateWDLaucherPushButton_clicked()
{
    if (TerminateProcessByNameAndCheck("WDlauncher.exe"))
    {
        writeRemindInfo("<p>" + tr("已销毁WDlauncher.exe进程") + "</p><br>");
    }
    else
    {
        writeRemindInfo("<p>" + tr("销毁WDlauncher.exe进程失败") + "</p><br>");
    }
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

    emit updateMenuBar(currentWidget);
}

/* 选择启动器路径 */
void SubSSJJWidget::on_chooseLauncherPathPushButton_clicked()
{
    // 设置文件过滤器，只允许选择 .exe 文件
    QString filter = tr("Executable Files (*.exe)");

    // 打开文件选择对话框
    QString fileName = QFileDialog::getOpenFileName(this, tr("请选择 WDlauncher.exe"),  "", filter);

    // 检查用户是否选择了文件
    if (!fileName.isEmpty()) {
        // 检查选择的文件是否是 WDlauncher.exe
        QFileInfo fileInfo(fileName);
        // qDebug() << fileInfo.fileName();
        if (fileInfo.fileName().contains("WDlauncher.exe")) {
            // 如果是 WDlauncher.exe，执行进一步操作
            QMessageBox::information(this, tr("成功"), tr("您已成功选择 WDlauncher.exe 文件！"));
            ui->launcherPathLineEdit->setText(fileName);
            ssjjInstallPath = fileName;
        } else {
            // 如果不是 WDlauncher.exe，弹出警告提示
            QMessageBox::warning(this, tr("错误"), tr("请选择 WDlauncher.exe 文件！"));
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
    // 乱斗模式
    if (ui->tabWidget->currentWidget() == ui->LDTab) {
        if (isAnyRadioButtonChecked(ui->tabWidget->currentWidget())) {
            ui->taskTableWidget->setRowCount(ui->taskTableWidget->rowCount() + 1);
            currentRow = ui->taskTableWidget->rowCount();
        }
        else {
            writeRemindInfo("<p>" + tr("请") + "<span style = \"color:red;\"><b>" + tr("先选择关卡") + "</b></span>" + tr("后再添加任务") + "</p><br>");
            return;
        }

        if (ui->LDRunTimesLineEdit->text() == "") {
            writeRemindInfo("<p><span style=\"color:red;\"><b>" + tr("关卡运行次数") + "</b></span>" + tr("未指定") + "</p><br>");
            ui->taskTableWidget->removeRow(currentRow - 1);
            return;
        }

        ui->taskTableWidget->setItem(currentRow - 1, 1, new QTableWidgetItem(tr("无")));
        ui->taskTableWidget->setItem(currentRow - 1, 2, new QTableWidgetItem(ui->LDRunTimesLineEdit->text()));
        ui->taskTableWidget->setItem(currentRow - 1, 3, new QTableWidgetItem("0"));
        if (ui->LDScriptPathLineEdit->text() != "") {
            ui->taskTableWidget->setItem(currentRow - 1, 4, new QTableWidgetItem(ui->LDScriptPathLineEdit->text()));
        }
        else {
            ui->taskTableWidget->setItem(currentRow - 1, 4, new QTableWidgetItem("未选择"));
        }

        if (ui->teamPropRadioButton->isChecked() == true) {
            ui->taskTableWidget->setItem(currentRow - 1, 0, new QTableWidgetItem("团队道具赛"));
        }
        else if (ui->luanJingRadioButton->isChecked() == true) {
            ui->taskTableWidget->setItem(currentRow - 1, 0, new QTableWidgetItem("乱境鏖战"));
        }
        else if (ui->challenHeroRadioButton->isChecked() == true) {
            ui->taskTableWidget->setItem(currentRow - 1, 0, new QTableWidgetItem("挑战王者"));
        }
        ui->taskTableWidget->setItem(currentRow - 1, 5, new QTableWidgetItem("王者乱斗"));
    }
    // 主线关卡
    else if (ui->tabWidget->currentWidget() == ui->ZXTab) {
        if (ui->zxRunTimesLineEdit->text() == "")
        {
            writeRemindInfo("<p><span style=\"color:red;\"><b>" + tr("关卡运行次数") + "</b></span>" + tr("未指定") + "</p><br>");
            return;
        }
        if (ui->zxScriptPathComboBox->currentText() == "")
        {
            writeRemindInfo("<p><span style=\"color:red;\"><b>" + tr("脚本路径") + "</b></span>" + tr("未指定") + "</p><br>");
            return;
        }

        // 添加新的一行
        ui->taskTableWidget->setRowCount(ui->taskTableWidget->rowCount() + 1);
        currentRow = ui->taskTableWidget->rowCount();

        // 添加信息
        ui->taskTableWidget->setItem(currentRow - 1, 0, new QTableWidgetItem(currentChoosedZXLevel.name));
        ui->taskTableWidget->setItem(currentRow - 1, 1, new QTableWidgetItem(ui->diffcultyChooseComboBox->currentText()));
        ui->taskTableWidget->setItem(currentRow - 1, 2, new QTableWidgetItem(ui->zxRunTimesLineEdit->text()));
        ui->taskTableWidget->setItem(currentRow - 1, 3, new QTableWidgetItem("0"));
        if (ui->zxScriptPathComboBox->currentText().contains(qApp->applicationDirPath().replace("/x64/Release", "")))
        {
            ui->taskTableWidget->setItem(currentRow - 1, 4, new QTableWidgetItem(ui->zxScriptPathComboBox->currentText().replace(qApp->applicationDirPath().replace("/x64/Release", "") + "/", "")));
        }
        else {
            ui->taskTableWidget->setItem(currentRow - 1, 4, new QTableWidgetItem(ui->zxScriptPathComboBox->currentText()));
        }
        ui->taskTableWidget->setItem(currentRow - 1, 5, new QTableWidgetItem("主线关卡"));
    }
}

/* 选择脚本 */
void SubSSJJWidget::on_chooseLDScriptPathPushButton_clicked()
{
    // 设置文件过滤器，只允许选择 .scp或.lscp 文件
    QString filter = tr("乱斗脚本 (*.lscp);; 脚本文件 (*.scp)");

    // 打开文件选择对话框
    QString fileName = QFileDialog::getOpenFileName(this, tr("请选择乱斗脚本文件"), "", filter);

    // 检查用户是否选择了文件
    if (!fileName.isEmpty()) {
        ui->LDScriptPathLineEdit->setText(fileName);
    }
    else{
        ui->LDScriptPathLineEdit->setText("");
    }
}

void SubSSJJWidget::on_chooseZXScriptPathPushButton_clicked()
{
    // 打开文件选择对话框
    QString fileName = QFileDialog::getOpenFileName(this, tr("请选择主线关卡脚本文件"), "", tr("主线脚本 (*.zscp);; 脚本文件 (*.scp)"));
    if (!fileName.isEmpty()) {
        // 查看文件是否在 zxScriptPathComboBox 中
        if (ui->zxScriptPathComboBox->findText(fileName) != -1) 
        {
            ui->zxScriptPathComboBox->setCurrentText(fileName);
        }
        else 
        {
            ui->zxScriptPathComboBox->addItem(fileName);
            ui->zxScriptPathComboBox->setCurrentText(fileName);
            // 保存文件至数据库
            ssjjScriptalFilesDatabase->readFileIntoDatabase(QFileInfo(fileName));
        }
    }
}

/* 开始运行脚本按钮按下 */
void SubSSJJWidget::on_startPushButton_clicked()
{
    // 检查是否有脚本任务
    if(ui->taskTableWidget->rowCount() == 0){
        writeRemindInfo("<p><span style=\"color:red;\"><b>" + tr("请先添加脚本任务") + "</b></span></p><br>");
        return;
    }

    // 重置任务次数
    if (ui->ifResetTimesCheckBox->isChecked() == true)
    {
        for (int i = 0; i < ui->taskTableWidget->rowCount(); i++) {
            ui->taskTableWidget->setItem(i, 3, new QTableWidgetItem(QString::number(0)));
        }
    }

    qDebug() << "开始运行脚本";
    if(checkThreadState() == 1) {
        writeRemindInfo("<p><span style=\"color:red;\"><b>" + tr("脚本正在运行中，请勿重复点击") + "</b></span></p><br>");
        return;
    }
    writeRemindInfo("<p><span style=\"color:lightgreen;\"><b>" + tr("脚本开始运行") + "</b></span></p><br>");

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
    connect(ssjjMainThread, &SSJJMainThread::sendDisplayText, this, &SubSSJJWidget::receiveDisplayText);
    /* 定义全局变量 */
    ssjjMainThread->receiveResolutionPath(this->resolutionPath);
    /* 开始执行 */
    ssjjMainThread->start();
    /* 发送任务 */
    sendNextTask(SSJJRunState());
}

void SubSSJJWidget::on_endPushButton_clicked()
{
    // 检查是否有脚本正在运行
    if(checkThreadRunningState(ssjjMainThread) != 1) {
        writeRemindInfo("<p><span style=\"colorred;\"><b>" + tr("当前没有脚本运行") + "</b></span></p><br>");
        return;
    }

    // 强制停止逻辑
    if(ssjjMainThread->isFinished() == false){
        forceQuitSSJJThread();
    }

    writeRemindInfo("<p><span style=\"color:lightgreen;\"><b>" + tr("脚本运行已结束") + "</b></span></p><br>");
}

void SubSSJJWidget::on_scriptRecordPushButton_clicked()
{
    startScriptRecord();
}

void SubSSJJWidget::forceQuitSSJJThread()
{
    ssjjMainThread->stopThread();
    ssjjMainThread->terminate();
    ssjjMainThread->deleteLater();
    ssjjMainThread = nullptr;
}

void SubSSJJWidget::receiveDisplayText(QString text)
{
    showTextOnScreen(text, QPoint(50, 50), 1000, "");
}

void SubSSJJWidget::showTextOnScreen(QString text, QPoint pos, int time, QString labelStyleSheet)
{
    ShowTextInScreenWidget *textShowWidget = new ShowTextInScreenWidget(this, text, pos, time, labelStyleSheet);
    textShowWidget->showTextInScreen();
}

/* 执行单次加成任务 */
void SubSSJJWidget::on_singleBonusPushButton_clicked()
{
    if (checkThreadState()){
        return;
    }
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
    else
    {
        weaponBonusThread->setIsRandomMove(false);
    }

    /* 是否发送消息 */
    if (ui->sendMessageCheckBox->isChecked() == true)
    {
        weaponBonusThread->setIsSendMessage(true);
    }
    else
    {
        weaponBonusThread->setIsSendMessage(false);
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

/* 获取单个任务 */
void SubSSJJWidget::getSingleTask()
{
    for(int i = 0; i < ui->taskTableWidget->rowCount(); i++){
        if(ui->taskTableWidget->item(i, 3)->text().toInt() < ui->taskTableWidget->item(i, 2)->text().toInt()){
            currentTask.taskName = ui->taskTableWidget->item(i, 0)->text();
            currentTask.difficulty = ui->taskTableWidget->item(i, 1)->text();
            currentTask.script = ui->taskTableWidget->item(i, 4)->text();
            
            if (ui->taskTableWidget->item(i, 5)->text() == "王者乱斗")
            {
                currentTask.taskType = Task::LuanDou;
            }
            else if (ui->taskTableWidget->item(i, 5)->text() == "主线关卡")
            {
                currentTask.taskType = Task::ZhuXian;
            }

            // 检查脚本文件是否存在
            if (currentTask.script != tr("未选择") && !QFile::exists(currentTask.script))
            {
                if (!QFile::exists(qApp->applicationDirPath().replace("/x64/Release", "") + "/" + currentTask.script)) {
                    writeRemindInfo("<p><span style=\"color:red;\"><b>" + currentTask.script + tr("脚本文件不存在") + "</b></span></p><br>");
                    // 清除该行
                    ui->taskTableWidget->removeRow(i);
                    i--;
                    continue;
                }
                else {
                    currentTask.script = qApp->applicationDirPath().replace("/x64/Release", "") + "/" + currentTask.script;
                }
            }
            else
            {
                return;
            }
        }
    }

    // 运行结束
    ssjjMainThread->stopThread();
    ssjjMainThread->terminate();
    ssjjMainThread->deleteLater();
    ssjjMainThread = nullptr;
}

void SubSSJJWidget::receiveRemindInfo(QString remindInfo)
{
    writeRemindInfo(remindInfo);
}

/* 发送下一个任务 */
void SubSSJJWidget::sendNextTask(SSJJRunState res)
{
    /* 执行成功 */
    if(res.errorType == "Success"){
        for(int i = 0; i < ui->taskTableWidget->rowCount(); i++){
            if(ui->taskTableWidget->item(i, 3)->text().toInt() < ui->taskTableWidget->item(i, 2)->text().toInt()){
                /* 显示信息 */
                writeRemindInfo("<p>" + currentTask.taskName + "-" + QString::number(ui->taskTableWidget->item(i, 3)->text().toInt() + 1) + " " + getYYYYMMDDHHMMSS() + "</p><br>");
                /* 更新表格 */
                ui->taskTableWidget->setItem(i, 3, new QTableWidgetItem(QString::number(ui->taskTableWidget->item(i, 3)->text().toInt() + 1)));
                break;
            }
        }
    }
    else if (res.errorType == "FatalError") {
        emit on_endPushButton_clicked();
        return;
    }

    // 获取下一个任务
    getSingleTask();

    if(checkThreadRunningState(ssjjMainThread) == 1){
        emit sendSingleTask(currentTask, ssjjInstallPath, ui->moveSpeedLineEdit->text().toInt(), ui->singleScriptTimeLineEdit->text().toInt(), ui->loadingTimeLineEdit->text().toInt());
    }
    else{
        writeRemindInfo("<p><span style=\"color:lightgreen;\"><b>" + tr("全部任务已执行完毕") + "</b></span></p><br>");
        writeRemindInfo("<p><span style=\"color:lightgreen;\"><b>" + tr("脚本运行已结束") + "</b></span></p><br>");
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
            if (ui->mainWeaponComboBox->currentText() == "裂隙行者")
            {
                // 裂隙行者
                weaponBonusThread->addBonusWeapon(new LieXi(weaponBonusThread));
            }
            if (ui->mainWeaponComboBox->currentText() == "碎星")
            {
                // 碎星
                weaponBonusThread->addBonusWeapon(new SuiXing(weaponBonusThread));
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
            if (ui->meleeWeaponComboBox->currentText() == "墨韵·合")
            {
                // 墨韵·合
                weaponBonusThread->addBonusWeapon(new MoYunHe(weaponBonusThread));
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
            if (ui->characterComboBox->currentText() == "轮滑")
            {
                // 轮滑
                weaponBonusThread->addBonusWeapon(new LunHua(weaponBonusThread));
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
        qWarning() << tr("Unable to open file for reading:") << filePath;
        return;
    }

    QByteArray fileData = file.readAll();
    file.close();

    QJsonDocument jsonDoc(QJsonDocument::fromJson(fileData));
    if (jsonDoc.isNull() || !jsonDoc.isObject()) {
        qWarning() << tr("Invalid JSON file format:") << filePath;
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

void SubSSJJWidget::closeTab(int index)
{
    if (index != 0) {
        // 获取当前标签页的编辑器
        ScpLanguageEditor* editor = scpLanguageEditors.at(index - 1);
        // 从编辑器列表中删除
        scpLanguageEditors.removeOne(editor);
        if (editor) {
            // 在这里可以执行关闭前的其他操作，比如保存文件等
            editor->deleteLater();  // 删除编辑器对象
        }

        // 移除标签页
        ui->scriptalTabWidget->removeTab(index);

        // 如果标签页数量为 1，则显示欢迎页
        if (ui->scriptalTabWidget->count() == 1) {
            ui->scriptalTabWidget->setTabVisible(0, true);
        }
    }
    else {
        // 关闭欢迎页，显示第一个标签页
        ui->scriptalTabWidget->setTabVisible(0, false);
    }
}

void SubSSJJWidget::updateZXLevelChooseComboBox()
{
    ui->zxLevelComboBox->clear();
    for (ZXLevel level : currentChoosedZXChapter.levels) {
        ui->zxLevelComboBox->addItem(level.name);
    }
    ui->zxLevelComboBox->setCurrentIndex(0);
    currentChoosedZXLevel = currentChoosedZXChapter.levels.at(0);
}

void SubSSJJWidget::updateZXDiffcultyChooseComboBox()
{
    int currentIndex = ui->diffcultyChooseComboBox->currentIndex();
    QString currentDifficulty = ui->diffcultyChooseComboBox->currentText();
    ui->diffcultyChooseComboBox->clear();
    for (DifficultyMode mode : currentChoosedZXLevel.modes)
    {
        ui->diffcultyChooseComboBox->addItem(mode.name);
    }
    if (ui->diffcultyChooseComboBox->count() > currentIndex)
        ui->diffcultyChooseComboBox->setCurrentIndex(currentIndex);
    else
        ui->diffcultyChooseComboBox->setCurrentText(currentDifficulty);
}

void SubSSJJWidget::updateZXScriptPathComboBox()
{
    ui->zxScriptPathComboBox->clear();
    QStringList scriptPaths = ssjjScriptalFilesDatabase->getFilesFromDatabase(currentChoosedZXLevel.name);
    for (QString scriptPath : scriptPaths)
    {
        ui->zxScriptPathComboBox->addItem(scriptPath);
    }
}

void SubSSJJWidget::currentTabChanged(int index)
{
    if (index > 0) {
        // 获取当前标签页的编辑器
        ScpLanguageEditor* editor = scpLanguageEditors.at(index - 1);
        // 获取文件编辑器打开文件的文件属性
        QList<FileAttribute> fileAttributes = editor->getFileAttributes();
        // 更新 fileAttributesTableWidget 内容
        ui->fileAttributesTableWidget->clearContents();
        ui->fileAttributesTableWidget->setRowCount(0);
        for (FileAttribute attribute : fileAttributes) {
            int row = ui->fileAttributesTableWidget->rowCount();
            ui->fileAttributesTableWidget->insertRow(row);
            if (attribute.name == "speed") {
                ui->fileAttributesTableWidget->setItem(row, 0, new QTableWidgetItem(attribute.name));
                QLineEdit* speedLineEdit = new QLineEdit();
                speedLineEdit->setText(attribute.value);
                QValidator* speedValidator = new QIntValidator(0, 999, this);
                speedLineEdit->setValidator(speedValidator);
                ui->fileAttributesTableWidget->setCellWidget(row, 1, speedLineEdit);
                connect(speedLineEdit, &QLineEdit::textChanged, [=]() {
                    QList<FileAttribute> fileAttributes = editor->getFileAttributes();
                    for (FileAttribute& attribute : fileAttributes) {
                        if (attribute.name == "speed") {
                            attribute.value = speedLineEdit->text();
                            break;
                        }
                    }
                    editor->receiveFileAttributes(fileAttributes);
                });
            }
            else {
                ui->fileAttributesTableWidget->setItem(row, 0, new QTableWidgetItem(attribute.name));
                ui->fileAttributesTableWidget->setItem(row, 1, new QTableWidgetItem(attribute.value));
            }
            
        }
    }
    else if (index == 0) {
        // 清空 fileAttributesTableWidget 内容
        ui->fileAttributesTableWidget->clearContents();
        ui->fileAttributesTableWidget->setRowCount(0);
    }
}

void SubSSJJWidget::getCurrentScriptEditor()
{
    if (ui->scriptalTabWidget->currentIndex() > 0) {
        // 找到当前标签页
        currentScriptEditor = scpLanguageEditors.at(ui->scriptalTabWidget->currentIndex() - 1);
    }
    else {
        currentScriptEditor = nullptr;
    }
}

void SubSSJJWidget::receiveWarningMessage(QString title, QString message)
{
    QMessageBox::warning(this, title, message);
}

bool SubSSJJWidget::checkThreadState()
{
    if (checkThreadRunningState(testScriptThread) == 1 || checkThreadRunningState(scriptRecordThread) == 1
        || checkThreadRunningState(ssjjMainThread) == 1 || checkThreadRunningState(weaponBonusThread) == 1)
    {
        textToShowInScreen->setValue(tr("当前有脚本正在运行"));
        return true;
    }
    else {
        return false;
    }
}

void SubSSJJWidget::convertRecordResult()
{
    getCurrentScriptEditor();
    // 读取录制结果文件
    QFile recordResultFile("output.txt");
    if (!recordResultFile.open(QIODevice::ReadOnly | QIODevice::Text)) {
        qWarning() << tr("Unable to open file for reading:") << "output.txt";
        return;
    }
    QTextStream in(&recordResultFile);
    QString result = in.readAll();
    recordResultFile.close();
    // 转换为脚本语言
    currentScriptEditor->insert(convertRecordToScp(result));
}

void SubSSJJWidget::regiseterMouseHotkey()
{
    qDebug() << "注册鼠标热键";
    // 当 ifCanControlMouse 为 true 时，说明鼠标热键已经被注册，不需要再次注册
    if (!ifCanControlMouse) {
        up_arrow->setRegistered(true);
        down_arrow->setRegistered(true);
        left_arrow->setRegistered(true);
        right_arrow->setRegistered(true);
        enter->setRegistered(true);
        middle->setRegistered(true);
        ifCanControlMouse = true;
        ui->testKeyMouseCheckBox->setChecked(true);
    }
}

void SubSSJJWidget::unregiseterMouseHotkey()
{
    // 当 ifCanControlMouse 为 false 时，说明鼠标热键已经被释放，不需要再次释放
    if (ifCanControlMouse) {
        up_arrow->setRegistered(false);
        down_arrow->setRegistered(false);
        left_arrow->setRegistered(false);
        right_arrow->setRegistered(false);
        enter->setRegistered(false);
        middle->setRegistered(false);
        ifCanControlMouse = false;
        ui->testKeyMouseCheckBox->setChecked(false);
    }
}

// 更改编辑器配色
void SubSSJJWidget::showEditorSettingsDialog()
{
    EditorSettingsDialog dialog(scpEditorConfig_dark, this);
    connect(&dialog, &EditorSettingsDialog::applySettings, this, [=](EditorConfig config) {
        scpEditorConfig_dark = config;
        resetEditorsAppearances();
        });

    dialog.exec();
    if (dialog.result() == QDialog::Accepted) {
        scpEditorConfig_dark = dialog.getEditorConfig();
        resetEditorsAppearances();
    }
    else if (dialog.result() == QDialog::Rejected) {
        return;
    }
}

void SubSSJJWidget::resetEditorsAppearances()
{
    for (ScpLanguageEditor* editor : scpLanguageEditors)
    {
        if(themeType->value() == "dark")
            editor->m_signalsHelper->resetStyles(scpEditorConfig_dark);
        else if(themeType->value() == "light")
            editor->m_signalsHelper->resetStyles(scpEditorConfig_light);
    }
}

void SubSSJJWidget::getGlobalEditorConfig(EditorConfig editorConfig)
{
    globalEditorConfig = editorConfig;
}

void SubSSJJWidget::startScriptRecord()
{
    if (checkThreadState())
    {
        return;
    }
    // 获取脚本类型
    int type;
    getCurrentScriptEditor();
    if (currentScriptEditor == nullptr) {
        QMessageBox::warning(this, tr("警告"), tr("请先打开一个脚本文件"));
        return;  // 当前标签页无效，直接返回
    }
    currentScriptEditor->saveFile();
    if (currentScriptEditor->getFileInfo().suffix() == "scp") {
        type = 0;
    }
    else if (currentScriptEditor->getFileInfo().suffix() == "zscp") {
        type = 1;
    }
    else if (currentScriptEditor->getFileInfo().suffix() == "lscp") {
        type = 2;
    }
    else {
        return;
    }
    // 显示教学对话框
    bool startRecord = true;
    if (ifShowRecordTeachingDialog == true) {
        RecordTeachingDialog* recordTeachingDialog = new RecordTeachingDialog(this);
        recordTeachingDialog->receiveRecordType(type);
        if (recordTeachingDialog->exec() == QDialog::Rejected) {
            startRecord = false;
        }
        ifShowRecordTeachingDialog = !recordTeachingDialog->getIfShowNextTime();
        recordTeachingDialog->deleteLater();
    }
    // 开始录制
    if (startRecord) {
        QFile output("output.txt");
        output.remove();
        if (checkThreadState()) {
            return;
        }
        if (type == 1) {
            showTextOnScreen("请确保当前画面处于关卡界面", QPoint(500, 50), 5000, 
                "QLabel{border: 2px solid red; border-radius: 5px; padding: 5px; background-color: white; color: black;}");
            QList<FileAttribute> fileAttributes = currentScriptEditor->getFileAttributes();
            SingleTask task;
            for (FileAttribute attribute : fileAttributes) {
                if (attribute.name == "difficulty") {
                    task.difficulty = attribute.value;
                }
                if (attribute.name == "level") {
                    task.taskName = attribute.value;
                }
            }
            task.taskType = Task::ZhuXian;
            task.script = currentScriptEditor->getFileInfo().absoluteFilePath();
            scriptRecordThread = new ScriptRecordThread(this, task);
            scriptRecordThread->receiveLoadingTime(ui->loadingTimeLineEdit->text().toInt());
            connect(scriptRecordThread, &ScriptRecordThread::showRemindText, this, &SubSSJJWidget::showTextOnScreen);
            connect(scriptRecordThread, &ScriptRecordThread::unregiseterHotkey, this, &SubSSJJWidget::unregiseterHotkey);
            connect(scriptRecordThread, &ScriptRecordThread::regiseterHotkey, this, &SubSSJJWidget::regiseterHotkey);
            connect(scriptRecordThread, &ScriptRecordThread::regiseterMouseHotkey, this, &SubSSJJWidget::regiseterMouseHotkey);
            connect(scriptRecordThread, &ScriptRecordThread::unregiseterMouseHotkey, this, &SubSSJJWidget::unregiseterMouseHotkey);
            connect(scriptRecordThread, &ScriptRecordThread::forceQuitRecording, this, [=]() {
                forceQuitThread(scriptRecordThread);
                });
            connect(scriptRecordThread, &ScriptRecordThread::finished, this, [=]() {
                receiveDisplayText("录制完成");
                scriptRecordThread->deleteLater();

                // 开始处理录制结果
                convertRecordResult();
                });
            scriptRecordThread->start();
        }
    }
}

void SubSSJJWidget::showScriptRecordOptionDialog()
{
    ScriptRecordOptionDialog scriptRecordOptionDialog(this);
    scriptRecordOptionDialog.setIfShowTeachingDialogOption(ifShowRecordTeachingDialog);
    if (scriptRecordOptionDialog.exec() == QDialog::Accepted)
    {
        ifShowRecordTeachingDialog = scriptRecordOptionDialog.ifShowTeachingDialogOption();
    }
}

void SubSSJJWidget::importTaskList()
{
    importFromXlsx(ui->taskTableWidget);
}

void SubSSJJWidget::exportTaskList()
{
    exportToXlsx(ui->taskTableWidget);
}

// 创建新的文件编辑标签页
void SubSSJJWidget::creatNewScriptEditorTab(QString fileName, QString filePath, QList<FileAttribute> fileAttributes)
{
    // 新建编辑器对象
    ScpLanguageEditor* newEditor = new ScpLanguageEditor(this);
    // 判断文件是否已经打开
    for (ScpLanguageEditor* editor : scpLanguageEditors)
    {
        if (editor->getFileName() == fileName && editor->getSavePath() == filePath)
        {
            // 文件已经打开，直接转到该标签页
            ui->scriptalTabWidget->setCurrentWidget(editor);
            // 删除newEditor 对象
            newEditor->deleteLater();
            return;
        }
        else if (editor->getFileName() == fileName && editor->getSavePath() != filePath)
        {
            // 文件名相同，但路径不同，在原标签页上添加路径信息
            int index = ui->scriptalTabWidget->indexOf(editor);
            ui->scriptalTabWidget->setTabText(index, fileName + " (" + filePath + ")");
            // 新建标签页
            ui->scriptalTabWidget->addTab(newEditor, fileName + " (" + filePath + ")");
            // 创建新的标签页
            newEditor->receiveFileInfo(fileName, filePath, fileAttributes);
            scpLanguageEditors.append(newEditor);
            // 转到新建标签页
            ui->scriptalTabWidget->setCurrentWidget(newEditor);
            return;
        }
    }
    // 创建新的标签页
    ui->scriptalTabWidget->addTab(newEditor, fileName);
    newEditor->receiveFileInfo(fileName, filePath, fileAttributes);
    scpLanguageEditors.append(newEditor);

    // 转到新建标签页
    ui->scriptalTabWidget->setCurrentWidget(newEditor);

    // 隐藏欢迎页
    ui->scriptalTabWidget->setTabVisible(0, false);

    // 重设编辑器配色
    resetEditorsAppearances();
}

void SubSSJJWidget::readFilesIntoSSJJDatabase(QDir dir)
{
    ssjjScriptalFilesDatabase->readFilesIntoDatabase(dir);
    updateZXScriptPathComboBox();
}

void SubSSJJWidget::saveFile()
{
    // 找到当前标签页
    getCurrentScriptEditor();
    if (currentScriptEditor == nullptr) {
        return;  // 当前标签页无效，直接返回
    }
    // 保存文件
    currentScriptEditor->saveFile();
    // 将文件相关信息写入数据库
    ssjjScriptalFilesDatabase->readFileIntoDatabase(currentScriptEditor->getFileInfo());
    // 更新 zxScriptPathComboBox 内容
    updateZXScriptPathComboBox();
}

void SubSSJJWidget::testCurrentScript()
{
    ui->runInfoListWidget->clear();
    if (checkThreadState())
    {
        return;
    }

    getCurrentScriptEditor();
    if (currentScriptEditor == nullptr) {
        textToShowInScreen->setValue(tr("当前标签页无效"));
        return;  // 当前标签页无效，直接返回
    }
     
    currentScriptEditor->saveFile();
    testScriptThread = new TestScriptThread(currentScriptEditor->getFileInfo(), this);
    connect(testScriptThread, &TestScriptThread::sendWarningMessage, this, &SubSSJJWidget::receiveWarningMessage);
    connect(testScriptThread, &TestScriptThread::sendRunInfo, this, [=](QString info) {
        ui->runInfoListWidget->addItem(info);
        });

    testScriptThread->start();
    connect(testScriptThread, &TestScriptThread::finished, [=]() {testScriptThread->deleteLater(); });
}

void SubSSJJWidget::stopTestScript()
{
    int state = checkThreadRunningState(testScriptThread);
    if (state == 1)
    {
        forceQuitThread(testScriptThread);
    }
    state = checkThreadRunningState(scriptRecordThread);
    if (state == 1) {
        forceQuitThread(scriptRecordThread);
    }
}
