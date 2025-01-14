#include "scriptcompiler.h"

ScriptCompiler::ScriptCompiler(QString script, int userMoveSpeed, QObject* parent)
	: QObject(parent)
{}

ScriptCompiler::~ScriptCompiler()
{}

ScriptCompiler* ScriptCompiler::getInstance(QString script, int userMoveSpeed, QObject* parent)
{
	static ScriptCompiler* instance = new ScriptCompiler(script, userMoveSpeed, parent);
	return instance;
}

void ScriptCompiler::runScript(QString script, int userMoveSpeed)
{
	int m_userMoveSpeed = userMoveSpeed;
	QFile scriptFile(script);

	// 打开文件
	if (!scriptFile.open(QIODevice::ReadOnly | QIODevice::Text)) {
		qDebug() << "Failed to open script file: " << script;
		return;
	}

	// 逐行读取文件内容
	QTextStream in(&scriptFile);
	while (!in.atEnd()) {
		QString line = in.readLine();
		// 去除行首的空格和注释
		line = line.trimmed();
		if (line.startsWith("#") || line.isEmpty() == true || line.startsWith("//")) {
			continue;
		}
		// 解析指令
		parseCommand(line, m_userMoveSpeed);
	}

	QThread::sleep(100000);
}

void ScriptCompiler::parseCommand(QString command, int userMoveSpeed)
{
	qDebug() << "Parsing command: " << command;
}
