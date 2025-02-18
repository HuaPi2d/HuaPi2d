#include "testscriptthread.h"

TestScriptThread::TestScriptThread(QFileInfo scriptFileInfo, int loadingTime, QObject* parent)
	: QThread(parent)
{
	m_fileInfo = scriptFileInfo;
	m_loadingTime = loadingTime;
	if (m_fileInfo.suffix() == "zscp") {
		m_fileAttributesMap = readFileAttributesMap(m_fileInfo.absoluteFilePath());
		m_speed = m_fileAttributesMap["speed"].toInt();
	}
	else if (m_fileInfo.suffix() == "scp") {
		m_speed = -1;
	}
	m_compiler = new ScriptCompiler(m_fileInfo.absoluteFilePath(), m_speed);
	if (m_fileInfo.suffix() == "zscp") {
		m_compiler->setLoadTime(m_loadingTime);
	}
	connect(m_compiler, &ScriptCompiler::sendRunInfo, this, &TestScriptThread::sendRunInfo);
}

TestScriptThread::~TestScriptThread()
{}

void TestScriptThread::run()
{
	textToShowInScreen->setValue("Test Begin");
	m_compiler->runScript();
	textToShowInScreen->setValue("Test End");
}
