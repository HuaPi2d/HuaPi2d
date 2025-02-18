#include "scriptrecordthread.h"

ScriptRecordThread::ScriptRecordThread(QObject *parent, SingleTask task, int loadingTime)
	: QThread(parent)
{
	isInRecording = false;
	m_task = task;
	m_loadingTime = loadingTime;
	m_rec = new Recorder(&compiler, "output.txt");
	emit unregiseterHotkey();
	F11 = new QHotkey(QKeySequence("F11"), true, this);
	connect(F11, &QHotkey::activated, this, [=]() {
		if (isInRecording) {
			m_rec->stopRecording();
			emit regiseterHotkey();
			emit unregiseterMouseHotkey();
			QThread::msleep(1000);
		}
		else {
			emit regiseterHotkey();
			emit forceQuitRecording();
		}
		});
}

ScriptRecordThread::~ScriptRecordThread()
{}

void ScriptRecordThread::run()
{
	while (true) {
		QString state = checkCurrentState();
		if (state == "startPage") {
			break;
		}
		textToShowInScreen->valueChanged("正在等待切换至游戏界面...");
		QThread::msleep(3000);
	}
	SSJJRunState res = enterGame(m_task, m_loadingTime);
	if (res.errorType != "NoError")
	{
		showRemindText("进入游戏失败", QPoint(centerPoint.x() - 150, centerPoint.y() - 150), 3000,
			"QLabel{font-size: 30px; font-family: SimHei; border: 5px; border-style: solid; border-color: rgba(0, 0, 0, 128); ");
		return;
	}
	showRemindText("已开始录制键盘动作", QPoint(centerPoint.x() - 150, centerPoint.y() - 150), 3000,
		"QLabel{font-size: 30px; font-family: SimHei; border: 5px; border-style: solid; border-color: rgba(0, 0, 0, 128); "
		"padding: 5px; color: white; background-color: rgba(255, 0, 0, 128); padding: 10px; border-radius: 5px;}");
	// 开启录制
	emit regiseterMouseHotkey();
	isInRecording = true;
	runHooks();
	m_rec->record();
}

void ScriptRecordThread::receiveLoadingTime(int loadingTime)
{
	m_loadingTime = loadingTime;
}
