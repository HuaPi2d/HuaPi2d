#include "ssjjCoreFun.h"
#include "universal/script/simulate/mouseKeyboradControl.h"

// SSJJ_BattleClient_Unity.exe

// 检测当前游戏处于哪个状态
// waitTime: 检测状态前的等待时间，单位为毫秒
// loadingPage: 处于加载界面
// abnormalLoadingPage: 处于异常加载界面，检测到加载窗口，但检测不到进程
// runAwayPage: 处于逃跑界面
// settlementPage: 处于结算界面
// startPage: 处于开始界面
// hallPage: 处于大厅界面，但不包括开始界面
// gamePage: 处于游戏界面
// abnormalGamePage: 处于异常游戏界面，检测到游戏进程，但检测不到窗口
QString checkCurrentState(int waitTime)
{
    QThread::msleep(waitTime);

    cv::Point mousePicPos = findPicInFullScreen(":/pic/script/resources/pic/script/mouse.png");
    if (mousePicPos != cv::Point(-1, -1)) {
        mouseClick(mousePicPos.x, mousePicPos.y);
        QThread::msleep(100);
    }

    // 声明返回对象
    if (findPicInFullScreen(":/pic/script/resources/pic/script/" + resolutionPath + "loading_pic.png") != cv::Point(-1, -1))
    {
        // 处于加载界面
        if (findProcessByName("SSJJ_BattleClient_Unity.exe") != DWORD())
        {
            // 处于正常加载界面
            return "loadingPage";
        }
        else
        {
            // 处于异常加载界面
            return "abnormalLoadingPage";
        }
    }
    else if (findPicInFullScreen(":/pic/script/resources/pic/script/exit_LD.png") != cv::Point(-1, -1))
    {
        // 检测到退出标识
        if (findPicInFullScreen(":/pic/script/resources/pic/script/use_energy.png") != cv::Point(-1, -1))
        {
            // 处于结算界面
            return "settlementPage";
        }
        else
        {
            // 处于逃跑界面
            return "runAwayPage";
        }
    }
    else if (findPicInFullScreen(":/pic/script/resources/pic/script/LD_begin.png") != cv::Point(-1, -1) ||
           findPicInFullScreen(":/pic/script/resources/pic/script/zx_startPushButton.png") != cv::Point(-1, -1))
    {
        // 处于开始界面
        return "startPage";
    }
    else if (findPicInFullScreen(":/pic/script/resources/pic/script/hall.png") != cv::Point(-1, -1))
    {
        // 处于游戏界面，但不包括开始界面
        return "hallPage";
    }
    else if (findPicInFullScreen(":/pic/script/resources/pic/script/zx_settlementExit.png") != cv::Point(-1, -1))
    {
        // 处于主线结算界面
        return "settlementPage";
    }
    else if (findProcessByName("SSJJ_BattleClient_Unity.exe") != DWORD())
    {
        // 检测到游戏进程
        if (findPicInFullScreen(":/pic/script/resources/pic/script/" + resolutionPath + "inGamePlayingPage.png") != cv::Point(-1, -1)
            || findPicInFullScreen(":/pic/script/resources/pic/script/" + resolutionPath + "zx_inGamePlayingPage.png") != cv::Point(-1, -1))
        {
            // 处于游戏界面
            return "gamePage";
        }
        else
        {
            // 处于异常游戏界面
            return "abnormalGamePage";
        }
    }

    return QString();
}

cv::Point findAndClick(QString picPath, int timeLimit, int interval)
{
    cv::Point point = findPicInLimitedTime(picPath, timeLimit, interval);
    QThread::msleep(1000);
    if(point == cv::Point(-1, -1)){
        return point;
    }
    else{
        mouseClick(point.x, point.y);
        QThread::msleep(1000);
        return point;
    }
}

cv::Point findAndClickAndConfirm(QString picPath, int waitTime, int timeLimit, int interval){
    cv::Point res = findAndClick(picPath, timeLimit, interval);
    QThread::msleep(waitTime);
    if(findPicInFullScreen(picPath) != cv::Point(-1, -1)){
        res = findAndClick(picPath, timeLimit, interval);
    }
    return res;
}

bool findAndClickAndCheck(QString picPath, int waitTime, int timeLimit, int interval)
{
    cv::Point res = findAndClick(picPath, timeLimit, interval);
    QThread::msleep(waitTime);
    if (findPicInFullScreen(picPath) != cv::Point(-1, -1)) {
        // 点击后，依然存在图标，说明点击失败
        return false;
    }
    else {
        // 点击后，图标消失，说明点击成功
        return true;
    }
}

// 重启游戏
SSJJRunState restartSSJJ(QString ssjjInstallPath){
    SSJJRunState ssjjRunState;
    int i = 0;

    TerminateProcessByNameAndCheck("AirLobbyPreloader.exe", 5);
    QThread::msleep(1000);
    TerminateProcessByNameAndCheck("WDlauncher.exe", 5);
    QThread::msleep(1000);
    if (TerminateProcessByNameAndCheck("SSJJ_BattleClient_Unity.exe", 5) == false)
    {
        textToShowInScreen->setValue("SSJJ_BattleClient_Unity Process Terminate Failed");
    }
    else {
        textToShowInScreen->setValue("SSJJ_BattleClient_Unity Process Terminate Success");
    }
    QThread::msleep(1000);
    TerminateProcessByNameAndCheck("MicroClient.exe", 5);
    QThread::msleep(1000);
    if (findProcessByName("MicroClient.exe") != DWORD() /*|| findProcessByName("SSJJ_BattleClient_Unity.exe") != DWORD()*/)
    {
        ssjjRunState.remindText = "<p>发生未知错误，进程销毁失败</p><br>";
        ssjjRunState.errorType = "FatalError";
        ssjjRunState.nextStep = "fatalError";
        return ssjjRunState;
    }

    for(i = 0; i < 30; i++){
        if(findProcessByName("GameMon.des") == DWORD() && findProcessByName("GameGuard.des") == DWORD() && findProcessByName("GameMon64.des") == DWORD()){
            break;
        }
        QThread::msleep(1000);
    }
    if (i == 29)
    {
        ssjjRunState.remindText = "<p>GameGuard 退出异常</p><br>";
        ssjjRunState.errorType = "FatalError";
        ssjjRunState.nextStep = "fatalError";
        return ssjjRunState;
    }

    runProgramAsAdmin(ssjjInstallPath, QStringList());
    QStringList picList;
    picList.append(":/pic/script/resources/pic/script/loaded_cover.png");
    picList.append(":/pic/script/resources/pic/script/first_server.png");
    CompeteRes res = competeFindPic(picList, 60000, 1000);
    if(res.num == 0){
        ssjjRunState.remindText = "<p>请开启<span style=\"color: red;\"><b>自动登录</b></span></p><br>";
        ssjjRunState.errorType = "FatalError";
        ssjjRunState.nextStep = "fatalError";
        return ssjjRunState;
    }
    else if(res.num == -1){
        ssjjRunState.remindText = "<p>发生未知错误，请检查<span style=\"color: red;\"><b>网络连接</b></span></p><br>";
        ssjjRunState.errorType = "Error";
        ssjjRunState.nextStep = "restartSSJJ";
        return ssjjRunState;
    }
    mouseClick(res.point.x, res.point.y);

    /* 寻找是否登录 */
    picList.clear();
    picList.append(":/pic/script/resources/pic/script/close_activity.png");
    picList.append(":/pic/script/resources/pic/script/hall.png");
    picList.append(":/pic/script/resources/pic/script/money.png");
    res = competeFindPic(picList, 60000, 1000);
    if(res.num == -1){
        ssjjRunState.remindText = "<p>发生未知错误，请检查<span style=\"color: red;\"><b>网络连接</b></span></p><br>";
        ssjjRunState.errorType = "Error";
        ssjjRunState.nextStep = "restartSSJJ";
        return ssjjRunState;
    }
    mouseClick(2, 2);
    QThread::msleep(15000);

    /* 判断是否弹出活动界面 */
    res = competeFindPic(picList, 3000, 1000);

    /* 如果有，退出活动界面 */
    if(res.num == 0){
        findAndClick(":/pic/script/resources/pic/script/close_activity.png", 3000);
        QThread::msleep(8000);
        findAndClick(":/pic/script/resources/pic/script/close_activity.png", 3000);
    }
    /* 如果没有，进行任意操作后弹出活动界面 */
    else if(res.num == 1){
        findAndClick(":/pic/script/resources/pic/script/leisure_mode.png", 3000);
        QThread::msleep(5000);
        if(findPicInLimitedTime(":/pic/script/resources/pic/script/leisure_mode.png", 3000) != cv::Point(-1,-1)){
            findAndClick(":/pic/script/resources/pic/script/leisure_mode.png", 3000);
        }

        /* 此时弹出活动界面 */
        findAndClick(":/pic/script/resources/pic/script/close_activity.png", 3000);
        findAndClick(":/pic/script/resources/pic/script/close_activity.png", 10000);
    }

    if(findPicInLimitedTime(":/pic/script/resources/pic/script/hall.png", 6000) != cv::Point(-1, -1))
    {
        ssjjRunState.remindText = "";
        ssjjRunState.errorType = "NoError";
        ssjjRunState.nextStep = "initializeGameScreen";
        return ssjjRunState;
    }
    else{
        ssjjRunState.remindText = "<p>初始化失败</p><br>";
        ssjjRunState.errorType = "Error";
        ssjjRunState.nextStep = "restartSSJJ";
        return ssjjRunState;
    }
}

// 初始化游戏画面
SSJJRunState initiallizeGameScreen(SingleTask task){
    SSJJRunState ssjjRunState;
    QStringList picList;
    CompeteRes res;
    cv::Point point;

    /* 回到大厅 */
    point = findAndClick(":/pic/script/resources/pic/script/hall.png", 6000);
    if(point == cv::Point(-1, -1)){
        ssjjRunState.errorType = "Error";
        ssjjRunState.remindText = "<p>未检测到游戏画面</p><br>";
        ssjjRunState.nextStep = "restartSSJJ";
        return ssjjRunState;
    }
    findAndClick(":/pic/script/resources/pic/script/confirm_exit_channel.png", 3000);
    if(task.taskType == Task::LuanDou) {
        // 乱斗模式
        findAndClickAndConfirm(":/pic/script/resources/pic/script/leisure_mode.png", 3000);
        findAndClick(":/pic/script/resources/pic/script/LD_Icon.png", 8000);
        QThread::msleep(3000);
        picList.clear();
        picList.append(":/pic/script/resources/pic/script/cancel.png");
        picList.append(":/pic/script/resources/pic/script/energy_gray.png");
        picList.append(":/pic/script/resources/pic/script/energy_lighted.png");
        res = competeFindPic(picList, 3000, 1000);
        // 上场战斗未结束
        if(res.num == 0 || res.num == -1){
            picList.clear();
            picList.append(":/pic/script/resources/pic/script/cancel.png");
            picList.append(":/pic/script/resources/pic/script/energy_gray.png");
            picList.append(":/pic/script/resources/pic/script/energy_lighted.png");
            // 等待战斗结束
            for(int i = 0; i < 10; i++){
                findAndClick(":/pic/script/resources/pic/script/cancel.png", 3000);
                QThread::msleep(50000);
                point = findAndClick(":/pic/script/resources/pic/script/hall.png", 3000);
                if(point == cv::Point(-1, -1)){
                    ssjjRunState.errorType = "Error";
                    ssjjRunState.remindText = "<p>未检测到游戏画面</p><br>";
                    ssjjRunState.nextStep = "restartSSJJ";
                    return ssjjRunState;
                }
                findAndClick(":/pic/script/resources/pic/script/confirm_exit_channel.png", 3000);
                findAndClick(":/pic/script/resources/pic/script/leisure_mode.png", 3000);
                findAndClick(":/pic/script/resources/pic/script/LD_Icon.png", 8000);
                QThread::msleep(10000);
                CompeteRes res = competeFindPic(picList);
                if(res.num == -1){
                    ssjjRunState.errorType = "Error";
                    ssjjRunState.remindText = "<p>发生未知错误</p><br>";
                    ssjjRunState.nextStep = "restartSSJJ";
                    return ssjjRunState;
                }
                else if(res.num != 0){
                    break;
                }
            }
            ssjjRunState.errorType = "Error";
            ssjjRunState.remindText = "<p>发生未知错误</p><br>";
            ssjjRunState.nextStep = "restartSSJJ";
        }
        // 点开背水
        picList.clear();
        picList.append(":/pic/script/resources/pic/script/energy_gray.png");
        picList.append(":/pic/script/resources/pic/script/energy_lighted.png");
        CompeteRes res = competeFindPic(picList);
        if(res.num == -1){
            ssjjRunState.errorType = "Error";
            ssjjRunState.remindText = "<p>发生未知错误</p><br>";
            ssjjRunState.nextStep = "restartSSJJ";
            return ssjjRunState;
        }
        else if(res.num == 0)
        {
            mouseClick(res.point.x, res.point.y);
        }
        else
        {
            mouseClick(res.point.x, res.point.y);
            mouseClick(res.point.x, res.point.y);
            mouseClick(res.point.x, res.point.y);
        }
        ssjjRunState.errorType = "NoError";
        ssjjRunState.remindText = "";
        ssjjRunState.nextStep = "enterGame";
        return ssjjRunState;
    }
    else if (task.taskType == Task::ZhuXian) {
        // 主线关卡
        findAndClickAndConfirm(":/pic/script/resources/pic/script/leisure_mode.png", 3000);
        findAndClick(":/pic/script/resources/pic/script/zx_pic.png", 8000);
        QThread::msleep(2000);

        // 读取脚本信息
        QList<FileAttribute> scriptAttributes = readFileAttributes(task.script);
        FileAttribute chapter;
        FileAttribute level;
        for (FileAttribute attribute : scriptAttributes)
        {
            if (attribute.name == "chapter") {
                chapter = attribute;
            }
            if (attribute.name == "level") {
                level = attribute;
            }
        }

        // 转到指定关卡界面
        turnToPage(chapter, level);
        if (clickLevel(level) == false) {
            ssjjRunState.errorType = "Error";
            ssjjRunState.remindText = "";
            ssjjRunState.nextStep = "initializeGameScreen";
            return ssjjRunState;
        }

        ssjjRunState.errorType = "NoError";
        ssjjRunState.remindText = "";
        ssjjRunState.nextStep = "enterGame";
        return ssjjRunState;
    }
    else{
        return ssjjRunState;
    }
}

SSJJRunState enterGame(SingleTask task, int loadingTimes){
    SSJJRunState ssjjRunState;
    cv::Point point;
    QString state;

    if (task.taskType == Task::LuanDou) {
        // 乱斗模式
        if (task.taskName == "团队道具赛") {
            findAndClickAndConfirm(":/pic/script/resources/pic/script/LD_begin.png", 3000);
            findAndClick(":/pic/script/resources/pic/script/morningRemind.png", 3000);
            QThread::msleep(loadingTimes * 1000);
            for (int i = 0; i < 20; i++) {
                point = findPicInFullScreen(":/pic/script/resources/pic/script/" + resolutionPath + "loading_pic.png");
                DWORD handle = findProcessByName("SSJJ_BattleClient_Unity.exe");
                if (point == cv::Point(-1, -1) && handle != DWORD()) {
                    ssjjRunState.errorType = "NoError";
                    ssjjRunState.remindText = "<p>正常进入游戏</p><br>";
                    ssjjRunState.nextStep = "runScriptFile";
                    return ssjjRunState;
                }
                else if (point == cv::Point(-1, -1) && handle == DWORD()) {
                    // 卡前加载页面
                }
                else if (point != cv::Point(-1, -1) && handle == DWORD()) {
                    // 特殊情况
                    ssjjRunState.errorType = "Error";
                    ssjjRunState.remindText = "<p>特殊情况，进程异常</p><br>";
                    ssjjRunState.nextStep = "restartSSJJ";
                    return ssjjRunState;
                }
                QThread::msleep(1000);
            }

            // 检查逃跑
            TerminateProcessByName("SSJJ_BattleClient_Unity.exe");
            if (findPicInLimitedTime(":/pic/script/resources/pic/script/exit_LD.png") != cv::Point(-1, -1)) {
                findAndClick(":/pic/script/resources/pic/script/exit_LD.png");
                ssjjRunState.errorType = "Remind";
                ssjjRunState.remindText = "<p>出现逃跑提示</p><br>";
                ssjjRunState.nextStep = "enterGame";
                return ssjjRunState;
            }

            ssjjRunState.errorType = "Error";
            ssjjRunState.remindText = "<p><span style=\"color: red;\"><b>长时间停留在加载界面</b></span>，准备重启游戏</p><br>";
            return ssjjRunState;
        }
        else if (task.taskName == "乱境鏖战" || task.taskName == "挑战王者")
        {
            findAndClickAndConfirm(":/pic/script/resources/pic/script/LD_begin.png", 3000);
            if (findAndClickAndCheck(":/pic/script/resources/pic/script/morningRemind.png", 3000) == false)
            {
                // 开启超背
                findAndClick(":/pic/script/resources/pic/script/checkBox.png", 3000);
                ssjjRunState.errorType = "Error";
                ssjjRunState.remindText = "";
                ssjjRunState.nextStep = "enterGame";
                return ssjjRunState;
            }
            state = checkCurrentState(3000);
            if (state == "startPage")
            {
                findAndClickAndConfirm(":/pic/script/resources/pic/script/LD_begin.png", 3000);
                findAndClick(":/pic/script/resources/pic/script/morningRemind.png", 3000);
            }
            QThread::msleep(loadingTimes * 1000 - 3000);
            for (int i = 0; i < 20; i++) {
                state = checkCurrentState(1000);
                textToShowInScreen->setValue(state);
                if (state == "loadingPage") {

                }
                else if (state == "gamePage") {
                    ssjjRunState.errorType = "NoError";
                    ssjjRunState.remindText = "";
                    ssjjRunState.nextStep = "runScriptFile";
                    return ssjjRunState;
                }
                else if (state == "runAwayPage") {
                    findAndClick(":/pic/script/resources/pic/script/exit_LD.png", 3000);
                    ssjjRunState.errorType = "Remind";
                    ssjjRunState.remindText = "<p>出现逃跑提示</p><br>";
                    ssjjRunState.nextStep = "enterGame";
                    return ssjjRunState;
                }
                else if (state == "startPage" || state == "hallPage") {
                    // 卡前加载页面
                    ssjjRunState.errorType = "Error";
                    ssjjRunState.remindText = "";
                    ssjjRunState.nextStep = "enterGame";
                    return ssjjRunState;
                }
                else if (state == "settlementPage")
                {
                    ssjjRunState.errorType = "NoError";
                    ssjjRunState.remindText = "";
                    ssjjRunState.nextStep = "settlement";
                    return ssjjRunState;
                }
            }

            // 结束进程
            if (TerminateProcessByNameAndCheck("SSJJ_BattleClient_Unity.exe") == false) {
                textToShowInScreen->setValue("SSJJ_BattleClient_Unity Process Terminate Failed");
            }
            else {
                textToShowInScreen->setValue("SSJJ_BattleClient_Unity Process Terminate Success");
            }
            state = checkCurrentState(5000);
            if (state == "runAwayPage")
            {
                findAndClick(":/pic/script/resources/pic/script/exit_LD.png", 3000);
                ssjjRunState.errorType = "Remind";
                ssjjRunState.remindText = "<p>出现逃跑提示</p><br>";
                ssjjRunState.nextStep = "enterGame";
                return ssjjRunState;
            }

            ssjjRunState.errorType = "Error";
            ssjjRunState.remindText = "<p><span style=\"color: red;\"><b>发生未知错误，加载超时</b></span>，准备重启游戏</p><br>";
            ssjjRunState.nextStep = "restartSSJJ";
            return ssjjRunState;
        }
    }
    if (task.taskType == Task::ZhuXian) {
        // 选择指定难度
        chooseDifficulty(task.difficulty);

        // 主线关卡
        // 读取脚本信息
        QList<FileAttribute> scriptAttributes = readFileAttributes(task.script);
        FileAttribute chapter;
        FileAttribute level;
        for (FileAttribute attribute : scriptAttributes)
        {
            if (attribute.name == "chapter") {
                chapter = attribute;
            }
            if (attribute.name == "level") {
                level = attribute;
            }
        }

        // 进入关卡
        if (chapter.value == "工业区" || chapter.value == "城市突袭" || chapter.value == "玛雅迷踪" ||
            chapter.value == "沙漠奇兵" || chapter.value == "地狱之城" || chapter.value == "海岛魅影")
        {
            enterZXGame();
            QThread::msleep(loadingTimes * 1000 - 3000);
            for (int i = 0; i < 20; i++) {
                state = checkCurrentState(1000);
                textToShowInScreen->setValue(QString("循环%1").arg(i));
                textToShowInScreen->setValue(state);
                if (state == "loadingPage") {
                }
                else if (state == "gamePage") {
                    ssjjRunState.errorType = "NoError";
                    ssjjRunState.remindText = "";
                    ssjjRunState.nextStep = "runScriptFile";
                    return ssjjRunState;
                }
                else if (state == "startPage" || state == "hallPage") {
                    // 卡前加载页面
                    ssjjRunState.errorType = "Error";
                    ssjjRunState.remindText = "";
                    ssjjRunState.nextStep = "initializeGameScreen";
                    return ssjjRunState;
                }
                else if (state == "settlementPage")
                {
                    ssjjRunState.errorType = "NoError";
                    ssjjRunState.remindText = "";
                    ssjjRunState.nextStep = "settlement";
                    return ssjjRunState;
                }
            }

            // 结束进程
            if (TerminateProcessByNameAndCheck("SSJJ_BattleClient_Unity.exe") == false) {
                textToShowInScreen->setValue("SSJJ_BattleClient_Unity Process Terminate Failed");
            }
            else {
                textToShowInScreen->setValue("SSJJ_BattleClient_Unity Process Terminate Success");
            }

            ssjjRunState.errorType = "Error";
            ssjjRunState.remindText = "<p><span style=\"color: red;\"><b>发生未知错误，加载超时</b></span>，准备重启游戏</p><br>";
            ssjjRunState.nextStep = "restartSSJJ";
            return ssjjRunState;
        }
    }
    else{
        return ssjjRunState;
    }
}

SSJJRunState runScript(SingleTask task, int speed)
{
    SSJJRunState ssjjRunState;
    cv::Point point;
    QString state;

    QThread::msleep(2000);
    LeftClick();
    if (task.taskType == Task::LuanDou) {
        // 乱斗模式
        if (task.taskName == "乱境鏖战" || task.taskName == "挑战王者") {
            while (true) {
                point = findPicInFullScreen(":/pic/script/resources/pic/script/" + resolutionPath + "waitPeriod.png");
                if (point == cv::Point(-1, -1)) {
                    break;
                }
                QThread::msleep(1000);
            }
            // 按下 Q 键
            keyDown(0x51);
            // 检测是否退出游戏

            while (true) {
                QThread::msleep(3000);
                DWORD handle = findProcessByName("SSJJ_BattleClient_Unity.exe");
                if (handle == DWORD()) {
                    // 松开 Q 键
                    keyRelease(0x51);
                    ssjjRunState.errorType = "NoError";
                    ssjjRunState.remindText = "";
                    ssjjRunState.nextStep = "settlement";
                    return ssjjRunState;
                }
                state = checkCurrentState();
                if (state == "loadingPage" || state == "abnormalLoadingPage") {
                    ssjjRunState.errorType = "Error";
                    ssjjRunState.remindText = "";
                    ssjjRunState.nextStep = "restartSSJJ";
                    return ssjjRunState;
                }
                else if (state == "settlementPage" || state == "abnormalGamePage")
                {
                    ssjjRunState.errorType = "NoError";
                    ssjjRunState.remindText = "";
                    ssjjRunState.nextStep = "settlement";
                    return ssjjRunState;
                }
            }
        }
        else if (task.script == "未选择") {
            // 无脚本，如乱斗
            // 检测是否退出游戏
            while (true) {
                QThread::msleep(3000);
                DWORD handle = findProcessByName("SSJJ_BattleClient_Unity.exe");
                if (handle == DWORD()) {
                    ssjjRunState.errorType = "NoError";
                    ssjjRunState.remindText = "已成功结束游戏";
                    ssjjRunState.nextStep = "settlement";
                    return ssjjRunState;
                }
            }
        }
    }
    else if (task.taskType == Task::ZhuXian) {
        // 主线关卡
        ScriptCompiler scriptCompiler(task.script, speed);
        scriptCompiler.runScript();

        while (true)
        {
            QThread::msleep(3000);
            QString state = checkCurrentState();
            textToShowInScreen->setValue(state);
            if (state == "gamePage")
            {}
            // 开启结算页面
            else if (state == "settlementPage")
            {
                QThread::msleep(3000);
                ssjjRunState.errorType = "NoError";
                ssjjRunState.remindText = "";
                ssjjRunState.nextStep = "settlement";
                return ssjjRunState;
            }
            // 未开启结算界面
            else if (state == "startPage")
            {
                ssjjRunState.errorType = "Success";
                ssjjRunState.remindText = "";
                ssjjRunState.nextStep = "enterGame";
                return ssjjRunState;
            }
        }
    }
    ssjjRunState.errorType = "Error";
    ssjjRunState.remindText = "<p>运行超时，准备重启游戏</p><br>";
    ssjjRunState.nextStep = "restartSSJJ";
    return ssjjRunState;
}

SSJJRunState settlement(SingleTask task)
{
    SSJJRunState ssjjRunState;
    cv::Point point;

    if (task.taskType == Task::LuanDou) {
        // 乱斗模式
        if (task.taskName == "团队道具赛") {
            QThread::msleep(5000);
            for (int i = 0; i < 3; i++) {
                QThread::msleep(3000);
                findAndClick(":/pic/script/resources/pic/script/use_energy.png", 3000);
            }
            findAndClick(":/pic/script/resources/pic/script/exit_LD.png", 3000);
            if (findPicInLimitedTime(":/pic/script/resources/pic/script/LD_begin.png", 5000) != cv::Point(-1, -1)) {
                ssjjRunState.remindText = "";
                ssjjRunState.errorType = "Success";
                ssjjRunState.nextStep = "enterGame";
                return ssjjRunState;
            }
            else {
                ssjjRunState.remindText = "";
                ssjjRunState.errorType = "Error";
                ssjjRunState.nextStep = "restartSSJJ";
                return ssjjRunState;
            }
        }
        else if (task.taskName == "乱境鏖战" || task.taskName == "挑战王者") {
            QThread::msleep(5000);
            for (int i = 0; i < 3; i++) {
                QThread::msleep(3000);
                findAndClick(":/pic/script/resources/pic/script/use_energy.png", 3000);
            }
            findAndClick(":/pic/script/resources/pic/script/exit_LD.png", 3000);
            if (findPicInLimitedTime(":/pic/script/resources/pic/script/LD_begin.png", 5000) != cv::Point(-1, -1)) {
                ssjjRunState.remindText = "";
                ssjjRunState.errorType = "Success";
                ssjjRunState.nextStep = "enterGame";
                return ssjjRunState;
            }
            else {
                ssjjRunState.remindText = "";
                ssjjRunState.errorType = "Error";
                ssjjRunState.nextStep = "restartSSJJ";
                return ssjjRunState;
            }
        }
    }
    else if (task.taskType == Task::ZhuXian) {
        // 主线关卡
        findAndClick(":/pic/script/resources/pic/script/zx_settlementConfirm.png", 3000);
        findAndClick(":/pic/script/resources/pic/script/zx_settlementConfirm.png", 3000);
        findAndClick(":/pic/script/resources/pic/script/zx_settlementExit.png", 3000);
        ssjjRunState.remindText = "";
        ssjjRunState.errorType = "Success";
        ssjjRunState.nextStep = "enterGame";
        return ssjjRunState;
    }
}

void turnToPage(FileAttribute chapter, FileAttribute level)
{
    if (chapter.value == "工业区" || chapter.value == "城市突袭" || chapter.value == "玛雅迷踪" || chapter.value == "沙漠奇兵"
        || chapter.value == "地狱之城" || chapter.value == "海岛魅影")
    {
        cv::Point point = findPicInLimitedTime(":/pic/script/resources/pic/script/zx_LeftPage.png", 3000);
        if (chapter.value == "工业区") {
            for (int i = 0; i < 10; i++) {
                mouseClick(point.x, point.y);
                QThread::msleep(100);
            }
            if (level.value == "工业一" || level.value == "工业二" || level.value == "工业三" ||
                level.value == "工业四" || level.value == "工业五" || level.value == "工业六")
            {
                mouseClick(point.x, point.y);
                QThread::msleep(100);
            }
        }
        if (chapter.value == "城市突袭") {
            for (int i = 0; i < 8; i++) {
                mouseClick(point.x, point.y);
                QThread::msleep(100);
            }
            if (level.value == "城市一" || level.value == "城市二" || level.value == "城市三" ||
                level.value == "城市四" || level.value == "城市五" || level.value == "城市六")
            {
                mouseClick(point.x, point.y);
                QThread::msleep(100);
            }
        }
        if (chapter.value == "玛雅迷踪") {
            for (int i = 0; i < 6; i++) {
                mouseClick(point.x, point.y);
                QThread::msleep(100);
            }
            if (level.value == "玛雅一" || level.value == "玛雅二" || level.value == "玛雅三" ||
                level.value == "玛雅四" || level.value == "玛雅五" || level.value == "玛雅六")
            {
                mouseClick(point.x, point.y);
                QThread::msleep(100);
            }
        }
        if (chapter.value == "沙漠奇兵") {
            for (int i = 0; i < 4; i++) {
                mouseClick(point.x, point.y);
                QThread::msleep(100);
            }
            if (level.value == "沙漠一" || level.value == "沙漠二" || level.value == "沙漠三" ||
                level.value == "沙漠四" || level.value == "沙漠五" || level.value == "沙漠六")
            {
                mouseClick(point.x, point.y);
                QThread::msleep(100);
            }
        }
        if (chapter.value == "地狱之城") {
            for (int i = 0; i < 2; i++) {
                mouseClick(point.x, point.y);
                QThread::msleep(100);
            }
            if (level.value == "地狱一" || level.value == "地狱二" || level.value == "地狱三" ||
                level.value == "地狱四" || level.value == "地狱五" || level.value == "地狱六")
            {
                mouseClick(point.x, point.y);
                QThread::msleep(100);
            }
        }
        if (chapter.value == "海岛魅影") {
            if (level.value == "海岛一" || level.value == "海岛二" || level.value == "海岛三" ||
                level.value == "海岛四" || level.value == "海岛五" || level.value == "海岛六" || level.value == "海岛七")
            {
                mouseClick(point.x, point.y);
                QThread::msleep(100);
            }
        }
    }
}

bool clickLevel(FileAttribute level)
{
    cv::Point point;
    if (level.value == "工业一")
        point = findPicInLimitedTime(":/pic/script/resources/pic/script/gongye1.jpg", 3000);
    else if (level.value == "工业二")
        point = findPicInLimitedTime(":/pic/script/resources/pic/script/gongye2.jpg", 3000);
    else if (level.value == "工业三")
        point = findPicInLimitedTime(":/pic/script/resources/pic/script/gongye3.jpg", 3000);
    else if (level.value == "工业四")
        point = findPicInLimitedTime(":/pic/script/resources/pic/script/gongye4.jpg", 3000);
    else if (level.value == "工业五")
        point = findPicInLimitedTime(":/pic/script/resources/pic/script/gongye5.jpg", 3000);
    else if (level.value == "工业六")
        point = findPicInLimitedTime(":/pic/script/resources/pic/script/gongye6.jpg", 3000);
    else if (level.value == "工业七")
        point = findPicInLimitedTime(":/pic/script/resources/pic/script/gongye7.jpg", 3000);
    else if (level.value == "工业八")
        point = findPicInLimitedTime(":/pic/script/resources/pic/script/gongye8.jpg", 3000);
    else if (level.value == "工业九")
        point = findPicInLimitedTime(":/pic/script/resources/pic/script/gongye9.jpg", 3000);
    else if (level.value == "工业十")
        point = findPicInLimitedTime(":/pic/script/resources/pic/script/gongye10.jpg", 3000);
    else if (level.value == "双子大厦")
        point = findPicInLimitedTime(":/pic/script/resources/pic/script/gongye11.jpg", 3000);
    else if (level.value == "城市一")
        point = findPicInLimitedTime(":/pic/script/resources/pic/script/chengshi1.jpg", 3000);
    else if (level.value == "城市二")
        point = findPicInLimitedTime(":/pic/script/resources/pic/script/chengshi2.jpg", 3000);
    else if (level.value == "城市三")
        point = findPicInLimitedTime(":/pic/script/resources/pic/script/chengshi3.jpg", 3000);
    else if (level.value == "城市四")
        point = findPicInLimitedTime(":/pic/script/resources/pic/script/chengshi4.jpg", 3000);
    else if (level.value == "城市五")
        point = findPicInLimitedTime(":/pic/script/resources/pic/script/chengshi5.jpg", 3000);
    else if (level.value == "城市六")
        point = findPicInLimitedTime(":/pic/script/resources/pic/script/chengshi6.jpg", 3000);
    else if (level.value == "城市七")
        point = findPicInLimitedTime(":/pic/script/resources/pic/script/chengshi7.jpg", 3000);
    else if (level.value == "城市八")
        point = findPicInLimitedTime(":/pic/script/resources/pic/script/chengshi8.jpg", 3000);
    else if (level.value == "城市九")
        point = findPicInLimitedTime(":/pic/script/resources/pic/script/chengshi9.jpg", 3000);
    else if (level.value == "城市十")
        point = findPicInLimitedTime(":/pic/script/resources/pic/script/chengshi10.jpg", 3000);
    else if (level.value == "星界工厂")
        point = findPicInLimitedTime(":/pic/script/resources/pic/script/chengshi11.jpg", 3000);
    else if (level.value == "玛雅一")
        point = findPicInLimitedTime(":/pic/script/resources/pic/script/maya1.jpg", 3000);
    else if (level.value == "玛雅二")
        point = findPicInLimitedTime(":/pic/script/resources/pic/script/maya2.jpg", 3000);
    else if (level.value == "玛雅三")
        point = findPicInLimitedTime(":/pic/script/resources/pic/script/maya3.jpg", 3000);
    else if (level.value == "玛雅四")
        point = findPicInLimitedTime(":/pic/script/resources/pic/script/maya4.jpg", 3000);
    else if (level.value == "玛雅五")
        point = findPicInLimitedTime(":/pic/script/resources/pic/script/maya5.jpg", 3000);
    else if (level.value == "玛雅六")
        point = findPicInLimitedTime(":/pic/script/resources/pic/script/maya6.jpg", 3000);
    else if (level.value == "玛雅七")
        point = findPicInLimitedTime(":/pic/script/resources/pic/script/maya7.jpg", 3000);
    else if (level.value == "玛雅八")
        point = findPicInLimitedTime(":/pic/script/resources/pic/script/maya8.jpg", 3000);
    else if (level.value == "玛雅九")
        point = findPicInLimitedTime(":/pic/script/resources/pic/script/maya9.jpg", 3000);
    else if (level.value == "玛雅十")
        point = findPicInLimitedTime(":/pic/script/resources/pic/script/maya10.jpg", 3000);
    else if (level.value == "玛雅金字塔")
        point = findPicInLimitedTime(":/pic/script/resources/pic/script/maya11.jpg", 3000);
    else if (level.value == "沙漠一")
        point = findPicInLimitedTime(":/pic/script/resources/pic/script/shamo1.jpg", 3000);
    else if (level.value == "沙漠二")
        point = findPicInLimitedTime(":/pic/script/resources/pic/script/shamo2.jpg", 3000);
    else if (level.value == "沙漠三")
        point = findPicInLimitedTime(":/pic/script/resources/pic/script/shamo3.jpg", 3000);
    else if (level.value == "沙漠四")
        point = findPicInLimitedTime(":/pic/script/resources/pic/script/shamo4.jpg", 3000);
    else if (level.value == "沙漠五")
        point = findPicInLimitedTime(":/pic/script/resources/pic/script/shamo5.jpg", 3000);
    else if (level.value == "沙漠六")
        point = findPicInLimitedTime(":/pic/script/resources/pic/script/shamo6.jpg", 3000);
    else if (level.value == "沙漠七")
        point = findPicInLimitedTime(":/pic/script/resources/pic/script/shamo7.jpg", 3000);
    else if (level.value == "沙漠八")
        point = findPicInLimitedTime(":/pic/script/resources/pic/script/shamo8.jpg", 3000);
    else if (level.value == "沙漠九")
        point = findPicInLimitedTime(":/pic/script/resources/pic/script/shamo9.jpg", 3000);
    else if (level.value == "沙漠十")
        point = findPicInLimitedTime(":/pic/script/resources/pic/script/shamo10.jpg", 3000);
    else if (level.value == "哈苏神庙")
        point = findPicInLimitedTime(":/pic/script/resources/pic/script/shamo11.jpg", 3000);
    else if (level.value == "地狱一")
        point = findPicInLimitedTime(":/pic/script/resources/pic/script/diyu1.jpg", 3000);
    else if (level.value == "地狱二")
        point = findPicInLimitedTime(":/pic/script/resources/pic/script/diyu2.jpg", 3000);
    else if (level.value == "地狱三")
        point = findPicInLimitedTime(":/pic/script/resources/pic/script/diyu3.jpg", 3000);
    else if (level.value == "地狱四")
        point = findPicInLimitedTime(":/pic/script/resources/pic/script/diyu4.jpg", 3000);
    else if (level.value == "地狱五")
        point = findPicInLimitedTime(":/pic/script/resources/pic/script/diyu5.jpg", 3000);
    else if (level.value == "地狱六")
        point = findPicInLimitedTime(":/pic/script/resources/pic/script/diyu6.jpg", 3000);
    else if (level.value == "地狱七")
        point = findPicInLimitedTime(":/pic/script/resources/pic/script/diyu7.jpg", 3000);
    else if (level.value == "地狱八")
        point = findPicInLimitedTime(":/pic/script/resources/pic/script/diyu8.jpg", 3000);
    else if (level.value == "地狱九")
        point = findPicInLimitedTime(":/pic/script/resources/pic/script/diyu9.jpg", 3000);
    else if (level.value == "地狱十")
        point = findPicInLimitedTime(":/pic/script/resources/pic/script/diyu10.jpg", 3000);
    else if (level.value == "匿名者行动基地")
        point = findPicInLimitedTime(":/pic/script/resources/pic/script/diyu11.jpg", 3000);
    else if (level.value == "海岛一")
        point = findPicInLimitedTime(":/pic/script/resources/pic/script/haidao1.jpg", 3000);
    else if (level.value == "海岛二")
        point = findPicInLimitedTime(":/pic/script/resources/pic/script/haidao2.jpg", 3000);
    else if (level.value == "海岛三")
        point = findPicInLimitedTime(":/pic/script/resources/pic/script/haidao3.jpg", 3000);
    else if (level.value == "海岛四")
        point = findPicInLimitedTime(":/pic/script/resources/pic/script/haidao4.jpg", 3000);
    else if (level.value == "海岛五")
        point = findPicInLimitedTime(":/pic/script/resources/pic/script/haidao5.jpg", 3000);
    else if (level.value == "海岛六")
        point = findPicInLimitedTime(":/pic/script/resources/pic/script/haidao6.jpg", 3000);
    else if (level.value == "海岛七")
        point = findPicInLimitedTime(":/pic/script/resources/pic/script/haidao7.jpg", 3000);
    else if (level.value == "海岛八")
        point = findPicInLimitedTime(":/pic/script/resources/pic/script/haidao8.jpg", 3000);
    else if (level.value == "海岛九")
        point = findPicInLimitedTime(":/pic/script/resources/pic/script/haidao9.jpg", 3000);
    else if (level.value == "海岛十")
        point = findPicInLimitedTime(":/pic/script/resources/pic/script/haidao10.jpg", 3000);
    else if (level.value == "海岛十一")
        point = findPicInLimitedTime(":/pic/script/resources/pic/script/haidao11.jpg", 3000);
    else if (level.value == "海岛十二")
        point = findPicInLimitedTime(":/pic/script/resources/pic/script/haidao12.jpg", 3000);
    else if (level.value == "F.N.R城堡废墟")
        point = findPicInLimitedTime(":/pic/script/resources/pic/script/haidao13.jpg", 3000);
    
    // 点击坐标
    if (point != cv::Point(-1, -1)) {
        mouseClick(point.x, point.y);
        return true;
    }
    else {
        return false;
    }
}

// 选择难度
void chooseDifficulty(QString difficulty)
{
    if (checkCurrentState() != "startPage") {
        QThread::msleep(3000);
    }
    if (difficulty == "挑战" || difficulty == "普通" || difficulty == "困难" || difficulty == "噩梦" || difficulty == "极速"
        || difficulty == "专家一" || difficulty == "专家二" || difficulty == "专家三" || difficulty == "专家四"
        || difficulty == "专家五" || difficulty == "专家六")
    {
        findAndClick(":/pic/script/resources/pic/script/difficulty_combox1_arrow.png", 3000);
        if (difficulty == "挑战")
            findAndClick(":/pic/script/resources/pic/script/difficulty_TiaoZhan.png", 3000);
        else if (difficulty == "普通")
            findAndClick(":/pic/script/resources/pic/script/difficulty_Normal.png", 3000);
        else if (difficulty == "困难")
            findAndClick(":/pic/script/resources/pic/script/difficulty_Hard.png", 3000);
        else if (difficulty == "噩梦")
            findAndClick(":/pic/script/resources/pic/script/difficulty_NightMare.png", 3000);
        else if (difficulty == "极速")
            findAndClick(":/pic/script/resources/pic/script/difficulty_Jisu.png", 3000);
        else 
        {
            findAndClick(":/pic/script/resources/pic/script/difficulty_Expert.png", 3000);
            findAndClick(":/pic/script/resources/pic/script/difficulty_combox2_arrow.png", 3000);
            if (difficulty == "专家一")
                findAndClick(":/pic/script/resources/pic/script/difficulty_Expert1.png", 3000);
            else if (difficulty == "专家二")
                findAndClick(":/pic/script/resources/pic/script/difficulty_Expert2.png", 3000);
            else if (difficulty == "专家三")
                findAndClick(":/pic/script/resources/pic/script/difficulty_Expert3.png", 3000);
            else if (difficulty == "专家四")
                findAndClick(":/pic/script/resources/pic/script/difficulty_Expert4.png", 3000);
            else if (difficulty == "专家五")
                findAndClick(":/pic/script/resources/pic/script/difficulty_Expert5.png", 3000);
            else 
            {
                cv::Point point = findPicInLimitedTime(":/pic/script/resources/pic/script/difficulty_Expert_ScrollBar.png", 3000);
                MoveMouse(point.x, point.y);
                QThread::msleep(100);
                LeftDown();
                QThread::msleep(100);
                MoveMouse(point.x, point.y + 100);
                QThread::msleep(100);
                LeftUp();
                QThread::msleep(100);
                findAndClick(":/pic/script/resources/pic/script/difficulty_Expert6.png", 3000);
            }
        }
    }
}

// 进入主线游戏关卡
void enterZXGame()
{
    if (passWordMode == 1 || passWordMode == 2) {
        findAndClick(":/pic/script/resources/pic/script/zx_passWordPushButton.png", 3000);
        findAndClick(":/pic/script/resources/pic/script/zx_passWordLineEdit.png", 3000);
        if (passWordMode == 1) {
            inputString(enterGamePassWord);
        }
        else {
            inputString(getYYYYMMDDHHMMSS());
        }
        findAndClick(":/pic/script/resources/pic/script/zx_passWordStartPushButton.png", 3000);
    }
    else {
        findAndClick(":/pic/script/resources/pic/script/zx_startPushButton.png", 3000);
    }
}

