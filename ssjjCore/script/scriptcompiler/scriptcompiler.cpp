#include "scriptcompiler.h"

ScriptCompiler::ScriptCompiler(QString script, int userMoveSpeed, QObject* parent)
	: QObject(parent)
{
	m_scriptFilePath = script;
	m_userMoveSpeed = userMoveSpeed;
}

ScriptCompiler::~ScriptCompiler()
{}

ScriptCompiler* ScriptCompiler::getInstance(QString script, int userMoveSpeed, QObject* parent)
{
	static ScriptCompiler* instance = new ScriptCompiler(script, userMoveSpeed, parent);
	return instance;
}

void ScriptCompiler::runScript()
{
	readFileContent();
	removeComment();
	if (m_fileInfo.suffix() == "zscp") {
		if (checkCurrentState() != "gamePage") {
			if (m_task.taskType == Task::ZhuXian) {
				enterGame(m_task, m_loadTime);
			}
		}
	}
	else if (m_fileInfo.suffix() == "lscp") {
		if (checkCurrentState() != "gamePage") {
			if (m_task.taskType == Task::LuanDou) {
				enterGame(m_task, m_loadTime);
			}
		}
	}
	extractFunction();
}

bool ScriptCompiler::parseFunction(QString functionName, QString params)
{
	emit sendRunInfo("functionName: " + functionName + " params: " + params);
	QStringList paramList = params.split(",");
	QRegularExpression extractNumber("-?\\d+");
	QRegularExpression extractString("\"(?<string>.*?)\"" , QRegularExpression::UseUnicodePropertiesOption);
	if (functionName == "sleep_ms") {
		int time = extractNumber.match(paramList.at(0)).captured(0).toInt();
		sleep_ms(time);
		return true;
	}
	else if (functionName == "sleep_s") {
		int time = extractNumber.match(paramList.at(0)).captured(0).toInt();
		sleep_s(time);
		return true;
	}
	else if (functionName == "move_back") {
		int time = extractNumber.match(paramList.at(0)).captured(0).toInt();
		move_back(time);
		return true;
	}
	else if (functionName == "move_forward") {
		int time = extractNumber.match(paramList.at(0)).captured(0).toInt();
		move_forward(time);
		return true;
	}
	else if (functionName == "move_left") {
		int time = extractNumber.match(paramList.at(0)).captured(0).toInt();
		move_left(time);
		return true;
	}
	else if (functionName == "move_right") {
		int time = extractNumber.match(paramList.at(0)).captured(0).toInt();
		move_right(time);
		return true;
	}
	else if (functionName == "move") {
		QString direction = extractString.match(paramList.at(0)).captured("string");
		int time = extractNumber.match(paramList.at(1)).captured(0).toInt();
		QString res = move(direction, time);
		if (res != QString()) {
			emit sendRunInfo(res);
		}
		return true;
	}
	else if (functionName == "turn_around") {
		int angle = extractNumber.match(paramList.at(0)).captured(0).toInt();
		QString res = turn_around(angle, true);
		if (res != QString()) {
			emit sendRunInfo(res);
		}
		return true;
	}
	else if (functionName == "turn_up_down") {
		int angle = extractNumber.match(paramList.at(0)).captured(0).toInt();
		QString res = turn_up_down(angle, true);
		if (res != QString()) {
			emit sendRunInfo(res);
		}
		return true;
	}
	else if (functionName == "move_mouse_to") {
		int x = extractString.match(paramList.at(0)).captured("string").toInt();
		int y = extractString.match(paramList.at(1)).captured("string").toInt();
		QString res = move_mouse_to(x, y);
		if (res != QString()) {
			emit sendRunInfo(res);
		}
		return true;
	}
	else if (functionName == "random_move_mouse") {
		random_move_mouse();
		return true;
	}
	else if (functionName == "left_click") {
		if (paramList.count() == 1) {
			left_click();
			return true;
		}
		else {
			int x = extractString.match(paramList.at(0)).captured("string").toInt();
			int y = extractString.match(paramList.at(1)).captured("string").toInt();
			QString res = left_click(x, y);
			if (res != QString()) {
				emit sendRunInfo(res);
			}
			return true;
		}
	}
	else if (functionName == "left_down") {
		if (paramList.count() == 1) {
			left_down();
			return true;
		}
		else {
			int x = extractString.match(paramList.at(0)).captured("string").toInt();
			int y = extractString.match(paramList.at(1)).captured("string").toInt();
			QString res = left_down(x, y);
			if (res != QString()) {
				emit sendRunInfo(res);
			}
			return true;
		}
	}
	else if (functionName == "left_up") {
		if (paramList.count() == 1) {
			left_up();
			return true;
		}
		else {
			int x = extractString.match(paramList.at(0)).captured("string").toInt();
			int y = extractString.match(paramList.at(1)).captured("string").toInt();
			QString res = left_up(x, y);
			if (res != QString()) {
				emit sendRunInfo(res);
			}
			return true;
		}
	}
	else if (functionName == "right_click") {
		if (paramList.count() == 1) {
			right_click();
			return true;
		}
		else {
			int x = extractString.match(paramList.at(0)).captured("string").toInt();
			int y = extractString.match(paramList.at(1)).captured("string").toInt();
			QString res = right_click(x, y);
			if (res != QString()) {
				emit sendRunInfo(res);
			}
			return true;
		}
	}
	else if (functionName == "right_down") {
		if (paramList.count() == 1) {
			right_down();
			return true;
		}
		else {
			int x = extractString.match(paramList.at(0)).captured("string").toInt();
			int y = extractString.match(paramList.at(1)).captured("string").toInt();
			QString res = right_down(x, y);
			if (res != QString()) {
				emit sendRunInfo(res);
			}
			return true;
		}
	}
	else if (functionName == "right_up") {
		if (paramList.count() == 1) {
			right_up();
			return true;
		}
		else {
			int x = extractString.match(paramList.at(0)).captured("string").toInt();
			int y = extractString.match(paramList.at(1)).captured("string").toInt();
			QString res = right_up(x, y);
			if (res != QString()) {
				emit sendRunInfo(res);
			}
			return true;
		}
	}
	else if (functionName == "middle_click") {
		if (paramList.count() == 1) {
			middle_click();
			return true;
		}
		else {
			int x = extractString.match(paramList.at(0)).captured("string").toInt();
			int y = extractString.match(paramList.at(1)).captured("string").toInt();
			QString res = middle_click(x, y);
			if (res != QString()) {
				emit sendRunInfo(res);
			}
			return true;
		}
	}
	else if (functionName == "middle_down") {
		if (paramList.count() == 1) {
			middle_down();
			return true;
		}
		else {
			int x = extractString.match(paramList.at(0)).captured("string").toInt();
			int y = extractString.match(paramList.at(1)).captured("string").toInt();
			QString res = middle_down(x, y);
			if (res != QString()) {
				emit sendRunInfo(res);
			}
			return true;
		}
	}
	else if (functionName == "middle_up") {
		if (paramList.count() == 1) {
			middle_up();
			return true;
		}
		else {
			int x = extractString.match(paramList.at(0)).captured("string").toInt();
			int y = extractString.match(paramList.at(1)).captured("string").toInt();
			QString res = middle_up(x, y);
			if (res != QString()) {
				emit sendRunInfo(res);
			}
			return true;
		}
	}
	else if (functionName == "key_press") {
		QString key = extractString.match(paramList.at(0)).captured("string");
		key_press(key);
		return true;
	}
	else if (functionName == "key_up") {
		QString key = extractString.match(paramList.at(0)).captured("string");
		key_up(key);
		return true;
	}
	else if (functionName == "key_down") {
		QString key = extractString.match(paramList.at(0)).captured("string");
		key_down(key);
		return true;
	}
	return false;
}

void ScriptCompiler::readFileContent()
{
	// 读取脚本文件属性
	m_fileAttributesMap = readFileAttributesMap(m_scriptFilePath);
	m_fileInfo = QFileInfo(m_scriptFilePath);
	if (m_fileInfo.suffix() == "zscp") {
		m_task.taskType = Task::ZhuXian;
	}
	else if (m_fileInfo.suffix() == "lscp") {
		m_task.taskType = Task::LuanDou;
	}

	// 读取速度
	m_task.script = m_scriptFilePath;
	m_scriptMoveSpeed = m_fileAttributesMap["speed"].toInt();
	m_task.taskName = m_fileAttributesMap["level"];
	m_task.difficulty = m_fileAttributesMap["difficulty"];

	QFile scriptFile(m_scriptFilePath);

	/* 文件初步处理*/
	if (!scriptFile.open(QIODevice::ReadOnly | QIODevice::Text)) {
		qDebug() << "Failed to open script file: " << m_scriptFilePath;
		return;
	}

	// 逐行读取文件内容
	QTextStream cacheIn(&scriptFile);
	m_fileContent = cacheIn.readAll();
}

void ScriptCompiler::removeComment()
{
	m_code = "";
	// token 提取器
	static const QRegularExpression tokenizer("(?<!\\\\)\"\"\"|(?<!\\\\)'''|#|" // 注释
		"(?<!\\\\)\"|(?<!\\\\)'|"  // 字符串
		"\\s+|"  // 空字符串
		"\\w+|"  // 文本，变量
		"\\W"  // 操作符
		, QRegularExpression::UseUnicodePropertiesOption);
	static const QRegularExpression linebreak("\\s*(\\n|\\r)+\\s*");

	// 声明正则表达式对象
	QRegularExpressionMatchIterator tokenIter = tokenizer.globalMatch(m_fileContent);
	QRegularExpressionMatch match;

	// 标志符
	bool isSharpComment = false, isIndicator = false;
	int style = 0;
	enum StringType { Comment1 = 0, Comment2, StringTypeCount };
	static const char* StringTypeIndicator[StringTypeCount] = { "\"\"\"" , "'''" };

	while (tokenIter.hasNext())
	{
		match = tokenIter.next();
		QString token = match.captured(0);

		if (linebreak.match(token).hasMatch()) {
			isSharpComment = false;
		}
		if (!style && token == "#") {
			isSharpComment = true;
			continue;
		}
		if (!isSharpComment) {
			for (int i = 0; i < StringTypeCount; i++) {
				if (token == StringTypeIndicator[i]) {
					isIndicator = true;
					if (style & (1 << i)) {
						style &= ~(1 << i);
					}
					else {
						style |= 1 << i;
					}
					break;
				}
			}
			if (isIndicator) {
				isIndicator = false;
				continue;
			}
		}
		if (!style && !isSharpComment) {
			m_code = m_code + token;
		}
	}
}

void ScriptCompiler::extractFunction()
{
	QRegularExpression functionRegex("(?<functionName>\\w+)\\s*\\((?<params>.*)\\)", QRegularExpression::UseUnicodePropertiesOption);
	QRegularExpressionMatchIterator functionIter = functionRegex.globalMatch(m_code);
	QRegularExpressionMatch match;
	while (functionIter.hasNext())
	{
		match = functionIter.next();
		QString functionName = match.captured("functionName");
		QString params = match.captured("params");
		if (!parseFunction(functionName, params)) {
			emit sendRunInfo("unknown functionName: " + functionName + " params: " + params);
		}
	}
}

void ScriptCompiler::setLoadTime(int time)
{
	m_loadTime = time;
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
