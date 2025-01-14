#include "testscriptthread.h"

TestScriptThread::TestScriptThread(QFileInfo scriptFileInfo, QObject* parent)
	: QThread(parent)
{
	m_fileInfo = scriptFileInfo;
}

TestScriptThread::~TestScriptThread()
{}

void TestScriptThread::run()
{
	if (m_fileInfo.suffix() == "zscp")
	{
		int speed;
		QList<FileAttribute> fileAttributes = readFileAttributes(m_fileInfo.absoluteFilePath());
		for (FileAttribute fileAttribute : fileAttributes)
		{
			if (fileAttribute.name == "speed")
			{
				if (fileAttribute.value != "")
				{
					speed = fileAttribute.value.toInt();
				}
				else {
					// 弹出警告，提示输入速度
					emit sendWarningMessage("提示", "请在右侧属性栏设置脚本录制速度！");
	                return;
				}
			}
		}
		textToShowInScreen->setValue("Test Begin");
		ScriptCompiler::runScript(m_fileInfo.absoluteFilePath(), speed);
		textToShowInScreen->setValue("Test End");
	}
}
