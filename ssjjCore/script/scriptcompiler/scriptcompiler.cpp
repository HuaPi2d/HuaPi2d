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
	QFileInfo scriptInfo(script);


	int m_userMoveSpeed = userMoveSpeed;
	int m_scriptMoveSpeed;

	// 读取脚本文件属性
	QList<FileAttribute> fileAttributes = readFileAttributes(script);

	// 读取速度
	for (FileAttribute fileAttribute : fileAttributes)
	{
		if (fileAttribute.name == "speed")
		{
			m_scriptMoveSpeed = fileAttribute.value.toInt();
		}
	}

	QFile scriptFile(script);

	/* 文件初步处理*/
	if (!scriptFile.open(QIODevice::ReadOnly | QIODevice::Text)) {
		qDebug() << "Failed to open script file: " << script;
		return;
	}

	// 逐行读取文件内容
	QTextStream cacheIn(&scriptFile);
	QString content = cacheIn.readAll();
	scriptFile.close();
	// 去除全部注释内容
	QRegularExpression lineCommentRegex(R"(#.*)");
	content.replace(lineCommentRegex, "");
	QRegularExpression multiLineCommentRegex(R"((\'\'\'.*?\'\'\'|\"\"\".*?\"\"\"))", QRegularExpression::DotMatchesEverythingOption);
	content.replace(multiLineCommentRegex, "");
	// 保存处理后的脚本内容
	QFileInfo cacheFileInfo(QDir::currentPath() + "/cache/" + scriptInfo.fileName());
	if (!saveStringFile(cacheFileInfo, content))
	{
		qDebug() << "Failed to save cache file: " << cacheFileInfo.filePath();
		return;
	}

	/* 读取缓存文件内容 */
	QFile cacheFile(cacheFileInfo.absoluteFilePath());
	if (!cacheFile.open(QIODevice::ReadOnly | QIODevice::Text)) {
		qDebug() << "Failed to open cache file: " << cacheFile.fileName();
		return;
	}
	QTextStream in(&cacheFile);

	// 解析脚本内容
	while (!in.atEnd()) {
		QString line = in.readLine();
		line = line.trimmed();
		// 判断语句类型
		if (judgeStatementType(line) == StatementType::function)
		{
			parseFunction(line);
		}
	}

	cacheFile.close();
	QThread::msleep(1000000); // 等待1秒，模拟执行时间
	// 清除缓存文件
	QFile::remove(cacheFileInfo.absoluteFilePath());
}

void ScriptCompiler::parseFunction(QString function)
{
	QRegularExpression functionRegex(R"(^(?P<functionName>\w+) # 匹配函数名\
		\s # 匹配空字符 \
		*\((?P<params>.*?)\) # 匹配参数 \)", QRegularExpression::ExtendedPatternSyntaxOption);
	QRegularExpressionMatch functionMatch = functionRegex.match(function);
	QString functionName = functionMatch.captured("functionName");
	QString params = functionMatch.captured("params");
	if (functionName == "move")
	{
		QStringList paramsList = params.split(",");
		if (paramsList.size() == 2)
		{
			QString param1 = paramsList.at(0).trimmed();
			QString param2 = paramsList.at(1).trimmed();
		}
	}
}

StatementType ScriptCompiler::judgeStatementType(QString line)
{
	/* 首先判断语句是否为函数格式 */ 
	QRegularExpression functionRegex(R"(^(?P<functionName>\w+) # 匹配函数名\
		\s # 匹配空字符 \
		*\((?P<params>.*?)\) # 匹配参数 \)", QRegularExpression::ExtendedPatternSyntaxOption);
	QRegularExpressionMatch functionMatch = functionRegex.match(line);
	if (functionMatch.hasMatch())
	{
		return StatementType::function;
	}
}
