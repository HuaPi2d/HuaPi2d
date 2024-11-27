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
    else if (findPicInFullScreen(":/pic/script/resources/pic/script/LD_begin.png") != cv::Point(-1, -1))
    {
        // 处于开始界面
        return "startPage";
    }
    else if (findPicInFullScreen(":/pic/script/resources/pic/script/hall.png") != cv::Point(-1, -1))
    {
        // 处于游戏界面，但不包括开始界面
        return "hallPage";
    }
    else if (findProcessByName("SSJJ_BattleClient_Unity.exe") != DWORD())
    {
        // 检测到游戏进程
        if (findPicInFullScreen(":/pic/script/resources/pic/script/" + resolutionPath + "inGamePlayingPage.png") != cv::Point(-1, -1))
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
    QThread::msleep(8000);

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

SSJJRunState initiallizeGameScreen(QString taskName){
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
    if(taskName == "团队道具赛" || taskName == "乱境鏖战"){
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
    else{
        return ssjjRunState;
    }
}

SSJJRunState enterGame(SingleTask task, int loadingTimes){
    SSJJRunState ssjjRunState;
    cv::Point point;
    QString state;

    if(task.taskName == "团队道具赛"){
        findAndClickAndConfirm(":/pic/script/resources/pic/script/LD_begin.png", 3000);
        findAndClick(":/pic/script/resources/pic/script/morningRemind.png", 3000);
        QThread::msleep(loadingTimes * 1000);
        for(int i = 0; i < 20; i++){
            point = findPicInFullScreen(":/pic/script/resources/pic/script/" + resolutionPath + "loading_pic.png");
            DWORD handle = findProcessByName("SSJJ_BattleClient_Unity.exe");
            if(point == cv::Point(-1, -1) && handle != DWORD()){
                ssjjRunState.errorType = "NoError";
                ssjjRunState.remindText = "<p>正常进入游戏</p><br>";
                ssjjRunState.nextStep = "runScriptFile";
                return ssjjRunState;
            }
            else if(point == cv::Point(-1, -1) && handle == DWORD()){
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
        if(findPicInLimitedTime(":/pic/script/resources/pic/script/exit_LD.png") != cv::Point(-1, -1)){
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
    else if (task.taskName == "乱境鏖战")
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
        if(TerminateProcessByNameAndCheck("SSJJ_BattleClient_Unity.exe") == false){
            textToShowInScreen->setValue("SSJJ_BattleClient_Unity Process Terminate Failed");
        }
        else{
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
    if (task.taskName == "乱境鏖战"){
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
    else if(task.script == "未选择"){
        // 无脚本，如乱斗
        // 检测是否退出游戏
        while(true){
            QThread::msleep(3000);
            DWORD handle = findProcessByName("SSJJ_BattleClient_Unity.exe");
            if(handle == DWORD()){
                ssjjRunState.errorType = "NoError";
                ssjjRunState.remindText = "已成功结束游戏";
                ssjjRunState.nextStep = "settlement";
                return ssjjRunState;
            }
        }
    }
    ssjjRunState.errorType = "Error";
    ssjjRunState.remindText = "<p>运行超时，准备重启游戏</p><br>";
    ssjjRunState.nextStep = "restartSSJJ";
    return ssjjRunState;
}

SSJJRunState settlement(QString taskName)
{
    SSJJRunState ssjjRunState;
    cv::Point point;

    if(taskName == "团队道具赛"){
        QThread::msleep(5000);
        for(int i = 0; i < 3; i++){
            QThread::msleep(3000);
            findAndClick(":/pic/script/resources/pic/script/use_energy.png", 3000);
        }
        findAndClick(":/pic/script/resources/pic/script/exit_LD.png", 3000);
        if(findPicInLimitedTime(":/pic/script/resources/pic/script/LD_begin.png", 5000) != cv::Point(-1, -1)){
            ssjjRunState.remindText = "";
            ssjjRunState.errorType = "Success";
            ssjjRunState.nextStep = "enterGame";
            return ssjjRunState;
        }
        else{
            ssjjRunState.remindText = "";
            ssjjRunState.errorType = "Error";
            ssjjRunState.nextStep = "restartSSJJ";
            return ssjjRunState;
        }
    }
    else if(taskName == "乱境鏖战"){
        QThread::msleep(5000);
        for(int i = 0; i < 3; i++){
            QThread::msleep(3000);
            findAndClick(":/pic/script/resources/pic/script/use_energy.png", 3000);
        }
        findAndClick(":/pic/script/resources/pic/script/exit_LD.png", 3000);
        if(findPicInLimitedTime(":/pic/script/resources/pic/script/LD_begin.png", 5000) != cv::Point(-1, -1)){
            ssjjRunState.remindText = "";
            ssjjRunState.errorType = "Success";
            ssjjRunState.nextStep = "enterGame";
            return ssjjRunState;
        }
        else{
            ssjjRunState.remindText = "";
            ssjjRunState.errorType = "Error";
            ssjjRunState.nextStep = "restartSSJJ";
            return ssjjRunState;
        }
    }
}
