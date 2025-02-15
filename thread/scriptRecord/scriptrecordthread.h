#pragma once

#include <QThread>
#include <QPoint>

#include "ssjjCore/ssjjCoreFun.h"
#include "global/ssjjglobalvar.h"
#include "global/ssjjglobalvar.h"
#include "MacroRecorder/stdafx.h"
#include "QHotkey/qhotkey.h"

class ScriptRecordThread  : public QThread
{
	Q_OBJECT

public:
	ScriptRecordThread(QObject *parent, SingleTask task, int loadingTime = 20);
	~ScriptRecordThread();

private:
	SingleTask m_task;
	int m_loadingTime;
	MacroCompiler compiler;
	Recorder* m_rec;
	QHotkey* F11;
	bool isInRecording;

protected:
	void run() override;

public:
	void receiveLoadingTime(int loadingTime);

public: signals:
	void showRemindText(QString text, QPoint pos, int time, QString labelStyleSheet);
	void unregiseterHotkey();
	void regiseterHotkey();
	void regiseterMouseHotkey();
	void unregiseterMouseHotkey();
	void forceQuitRecording();
};
