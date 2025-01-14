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

	// ���ļ�
	if (!scriptFile.open(QIODevice::ReadOnly | QIODevice::Text)) {
		qDebug() << "Failed to open script file: " << script;
		return;
	}

	// ���ж�ȡ�ļ�����
	QTextStream in(&scriptFile);
	while (!in.atEnd()) {
		QString line = in.readLine();
		// ȥ�����׵Ŀո��ע��
		line = line.trimmed();
		if (line.startsWith("#") || line.isEmpty() == true || line.startsWith("//")) {
			continue;
		}
		// ����ָ��
		parseCommand(line, m_userMoveSpeed);
	}

	QThread::sleep(100000);
}

void ScriptCompiler::parseCommand(QString command, int userMoveSpeed)
{
	qDebug() << "Parsing command: " << command;
}
