#include "testscriptthread.h"

TestScriptThread::TestScriptThread(QFileInfo scriptFileInfo, QObject* parent)
	: QThread(parent)
{
	m_fileInfo = scriptFileInfo;
	if (m_fileInfo.suffix() == "zscp") {
		m_fileAttributes = readFileAttributes(m_fileInfo.absoluteFilePath());
		for (FileAttribute fileAttribute : m_fileAttributes)
		{
			if (fileAttribute.name == "speed")
			{
				if (fileAttribute.value != "")
				{
					m_speed = fileAttribute.value.toInt();
				}
				else {
					// 弹出警告，提示输入速度
					emit sendWarningMessage("提示", "请在右侧属性栏设置脚本录制速度！");
					return;
				}
			}
		}
	}
	else if (m_fileInfo.suffix() == "scp") {
		m_speed = -1;
	}
	m_compiler = new ScriptCompiler(m_fileInfo.absoluteFilePath(), m_speed);
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
