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

	// 将处理后的代码按行分割
	QStringList lines = m_code.split('\n', Qt::SkipEmptyParts);

	// 逐行处理脚本
	for (const QString& line : lines) {
		QString trimmedLine = line.trimmed();
		if (trimmedLine.isEmpty()) {
			continue; // 跳过空行
		}

		// 处理每一行脚本
		processStatement(trimmedLine);
	}
}

QVariant ScriptCompiler::parseFunction(QString functionName, QString params)
{
	try {
		emit sendRunInfo("Running: functionName: " + functionName + " params: " + params);

		// 改进的参数分割处理
		QStringList paramList;
		if (!params.trimmed().isEmpty()) {
			paramList = params.split(",", Qt::SkipEmptyParts);
		}

		// Movement functions
		if (functionName == "sleep_ms") {
			checkParamCount(functionName, paramList, 1);
			ParamType paramType[1] = { ParamType::param_int };
			sleep_ms(parseParamStr(paramList.at(0), paramType[0]).toInt());
			return true;
		}
		else if (functionName == "sleep_s") {
			checkParamCount(functionName, paramList, 1);
			ParamType paramType[1] = { ParamType::param_int };
			sleep_s(parseParamStr(paramList.at(0), paramType[0]).toInt());
			return true;
		}
		else if (functionName == "move_back") {
			checkParamCount(functionName, paramList, 1);
			ParamType paramType[1] = { ParamType::param_int };
			move_back(parseParamStr(paramList.at(0), paramType[0]).toInt());
			return true;
		}
		else if (functionName == "move_forward") {
			checkParamCount(functionName, paramList, 1);
			ParamType paramType[1] = { ParamType::param_int };
			move_forward(parseParamStr(paramList.at(0), paramType[0]).toInt());
			return true;
		}
		else if (functionName == "move_left") {
			checkParamCount(functionName, paramList, 1);
			ParamType paramType[1] = { ParamType::param_int };
			move_left(parseParamStr(paramList.at(0), paramType[0]).toInt());
			return true;
		}
		else if (functionName == "move_right") {
			checkParamCount(functionName, paramList, 1);
			ParamType paramType[1] = { ParamType::param_int };
			move_right(parseParamStr(paramList.at(0), paramType[0]).toInt());
			return true;
		}
		else if (functionName == "move") {
			checkParamCount(functionName, paramList, 2);
			ParamType paramTypes[2] = { ParamType::param_string, ParamType::param_int };
			QString direction = parseParamStr(paramList.at(0), paramTypes[0]).toString();
			int time = parseParamStr(paramList.at(1), paramTypes[1]).toInt();
			QString res = move(direction, time);
			if (!res.isEmpty()) {
				emit sendRunInfo(res);
			}
			return true;
		}
		else if (functionName == "turn_around") {
			checkParamCount(functionName, paramList, 1);
			ParamType paramType[1] = { ParamType::param_int };
			int angle = parseParamStr(paramList.at(0), paramType[0]).toInt();
			QString res = turn_around(angle, true);
			if (!res.isEmpty()) {
				emit sendRunInfo(res);
			}
			return true;
		}
		else if (functionName == "turn_up_down") {
			checkParamCount(functionName, paramList, 1);
			ParamType paramType[1] = { ParamType::param_int };
			int angle = parseParamStr(paramList.at(0), paramType[0]).toInt();
			QString res = turn_up_down(angle, true);
			if (!res.isEmpty()) {
				emit sendRunInfo(res);
			}
			return true;
		}

		// Mouse functions
		else if (functionName == "move_mouse_to") {
			checkParamCount(functionName, paramList, 2);
			ParamType paramTypes[2] = { ParamType::param_int, ParamType::param_int };
			int x = parseParamStr(paramList.at(0), paramTypes[0]).toInt();
			int y = parseParamStr(paramList.at(1), paramTypes[1]).toInt();
			QString res = move_mouse_to(x, y);
			if (!res.isEmpty()) {
				emit sendRunInfo(res);
			}
			return true;
		}
		else if (functionName == "random_move_mouse") {
			checkParamCount(functionName, paramList, 0);
			random_move_mouse();
			return true;
		}

		// Mouse click functions
		else if (functionName == "left_click") {
			handleClickFunction(functionName, paramList,
				[]() { ::left_click(); },  // 无参数版本
				[](int x, int y) { ::left_click(x, y); }  // 带坐标版本
			);
			return true;
		}
		else if (functionName == "left_down") {
			handleClickFunction(functionName, paramList,
				[]() { ::left_down(); },
				[](int x, int y) { ::left_down(x, y); }
			);
			return true;
		}
		else if (functionName == "left_up") {
			handleClickFunction(functionName, paramList,
				[]() { ::left_up(); },
				[](int x, int y) { ::left_up(x, y); }
			);
			return true;
		}
		else if (functionName == "right_click") {
			handleClickFunction(functionName, paramList,
				[]() { ::right_click(); },
				[](int x, int y) { ::right_click(x, y); }
			);
			return true;
		}
		else if (functionName == "right_down") {
			handleClickFunction(functionName, paramList,
				[]() { ::right_down(); },
				[](int x, int y) { ::right_down(x, y); }
			);
			return true;
		}
		else if (functionName == "right_up") {
			handleClickFunction(functionName, paramList,
				[]() { ::right_up(); },
				[](int x, int y) { ::right_up(x, y); }
			);
			return true;
		}
		else if (functionName == "middle_click") {
			handleClickFunction(functionName, paramList,
				[]() { middle_click(); },
				[](int x, int y) { middle_click(x, y); }
			);
			return true;
		}
		else if (functionName == "middle_down") {
			handleClickFunction(functionName, paramList,
				[]() { middle_down(); },
				[](int x, int y) { middle_down(x, y); }
			);
			return true;
		}
		else if (functionName == "middle_up") {
			handleClickFunction(functionName, paramList,
				[]() { middle_up(); },
				[](int x, int y) { middle_up(x, y); }
			);
			return true;
		}

		// Keyboard functions
		else if (functionName == "key_press") {
			checkParamCount(functionName, paramList, 1);
			ParamType paramType[1] = { ParamType::param_string };
			QString key = parseParamStr(paramList.at(0), paramType[0]).toString();
			key_press(key);
			return true;
		}
		else if (functionName == "key_up") {
			checkParamCount(functionName, paramList, 1);
			ParamType paramType[1] = { ParamType::param_string };
			QString key = parseParamStr(paramList.at(0), paramType[0]).toString();
			key_up(key);
			return true;
		}
		else if (functionName == "key_down") {
			checkParamCount(functionName, paramList, 1);
			ParamType paramType[1] = { ParamType::param_string };
			QString key = parseParamStr(paramList.at(0), paramType[0]).toString();
			key_down(key);
			return true;
		}

		// img functions
		else if (functionName == "findpic") {
			checkParamCount(functionName, paramList, 1);
			ParamType paramType[1] = { ParamType::param_string };
			QString imgPath = parseParamStr(paramList.at(0), paramType[0]).toString();
			cv::Point res = findPicInFullScreen(imgPath);
			return QString("(%1, %2)").arg(res.x).arg(res.y);
		}

		// test functions
		else if (functionName == "print") {
			checkParamCount(functionName, paramList, 1);
			ParamType paramType[1] = { ParamType::param_unknown };
			QVariant param = parseParamStr(paramList.at(0), paramType[0]);
			if (param.type() == QMetaType::QString || param.type() == QMetaType::Int) {
				sendRunInfo(">>>" + param.toString());
			}
			else if (param.type() == QMetaType::QPoint) {
				sendRunInfo(">>>" + QString("(%1, %2)").arg(param.toPoint().x()).arg(param.toPoint().y()));
			}
			
			return true;
		}

		throw std::runtime_error(
			QString("Unknown function: %1").arg(functionName).toStdString());
	}
	catch (const std::exception& e) {
		emit sendRunInfo(QString("Error: in %1: %2").arg(functionName).arg(e.what()));
		return false;
	}
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

QVariant ScriptCompiler::processFunctionCall(const QString& line)
{
	QRegularExpression functionRegex("(?<functionName>\\w+)\\s*\\((?<params>.*)\\)", QRegularExpression::UseUnicodePropertiesOption);
	QRegularExpressionMatchIterator functionIter = functionRegex.globalMatch(line);
	QRegularExpressionMatch match;

	while (functionIter.hasNext())
	{
		match = functionIter.next();
		QString functionName = match.captured("functionName");
		QString params = match.captured("params");

		QVariant result = parseFunction(functionName, params);
		if (!result.isValid()) {
			emit sendRunInfo("unknown functionName: " + functionName + " params: " + params);
			return QVariant();
		}
		else if (!result.isNull()) {
			return result;
		}
	}
}

void ScriptCompiler::setLoadTime(int time)
{
	m_loadTime = time;
}

void ScriptCompiler::processStatement(const QString& line)
{
	switch (judgeStatementType(line)) {
	case StatementType::assignment:
		processAssignment(line);
		break;
	case StatementType::function:
		processFunctionCall(line);
		break;
	default:
		emit sendRunInfo("Unknown statement: " + line);
	}
}

void ScriptCompiler::processAssignment(const QString& line)
{
	QRegularExpression assignmentRegex(R"(^(?P<varName>\w+)\s*=\s*(?P<value>.+)$)");
	QRegularExpressionMatch match = assignmentRegex.match(line);

	if (match.hasMatch()) {
		QString varName = match.captured("varName");
		QString valueExpr = match.captured("value");

		// 简单处理：如果右边是函数调用，执行函数并存储结果
		if (judgeStatementType(valueExpr) == StatementType::function) {
			QVariant result = processFunctionCall(valueExpr);
			setVariableStr(varName, result);
		}
		else {
			// 处理简单值赋值
			setVariableStr(varName, QVariant(valueExpr));
		}
	}
}

QVariant ScriptCompiler::parseParamStr(QString paramStr, ParamType type, bool throwError)
{
	paramStr = paramStr.trimmed();

	if (m_variables.contains(paramStr)) {
		return parseVariable(paramStr, type, throwError);
	}
	else {
		Param param = getValueAndType(paramStr);
		if (param.type == type || type == ParamType::param_unknown) {
			if (param.type == ParamType::param_unknown) {
				if (throwError) {
					throw std::runtime_error(
						QString("Unknown parameter type for '%1'").arg(paramStr).toStdString());
				}
				else {
					return QVariant();
				}
			}
			else {
				return param.value;
			}
		}
		else {
			if (throwError) {
				throw std::runtime_error(
					QString("Parameter '%1' is not of type %2")
					.arg(paramStr)
					.arg(getParamTypeString(type))
					.toStdString());
			}
			else {
				return QVariant();
			}
		}
	}
}

Param ScriptCompiler::getValueAndType(QString valueStr)
{
	QRegularExpression extractNumber("^-?\\d+$");
	if (extractNumber.match(valueStr).hasMatch())
	{
		return Param(ParamType::param_int, valueStr);
	}
	QRegularExpression extractString("^\"(?<string>.*?)\"$", QRegularExpression::UseUnicodePropertiesOption);
	if (extractString.match(valueStr).hasMatch())
	{
		return Param(ParamType::param_string, extractString.match(valueStr).captured("string"));
	}
	QRegularExpression extractPoint("(?<x>-?\\d+)\\s*,\\s*(?<y>-?\\d+)");
	if (extractPoint.match(valueStr).hasMatch())
	{
		int x = extractPoint.match(valueStr).captured("x").toInt();
		int y = extractPoint.match(valueStr).captured("y").toInt();
		return Param(ParamType::param_point, QPoint(x, y));
	}
	return Param(ParamType::param_unknown, valueStr);
}

QVariant ScriptCompiler::parseVariable(QString variable, ParamType type, bool throwError)
{
	if (m_variables.contains(variable)) {
		QString varValue = m_variables[variable].toString();
		Param param = getValueAndType(varValue);
		ParamType varType = param.type;
		if (varType == type || type == ParamType::param_unknown) {
			if (varType == ParamType::param_unknown) {
				if (throwError) {
					throw std::runtime_error(
						QString("Unknown variable type for '%1'").arg(variable).toStdString());
				}
				else {
					return QVariant();
				}
			}
			else {
				return param.value;
			}
		}
		else {
			if (throwError) {
				throw std::runtime_error(
					QString("Variable '%1' is not of type %2 (value: %3)")
					.arg(variable)
					.arg(getParamTypeString(type))
					.arg(varValue)
					.toStdString());
			}
			else {
				return QVariant();
			}
		}
	}
	else {
		if (throwError) {
			throw std::runtime_error(
				QString("Unknown variable '%1'").arg(variable).toStdString());
		}
		else {
			return QVariant();
		}
	}
}

QString ScriptCompiler::getParamTypeString(ParamType type)
{
	if (type == ParamType::param_int) {
		return "int";
	}
	else if (type == ParamType::param_string) {
		return "string";
	}
	else if (type == ParamType::param_point) {
		return "point";
	}
	else if (type == ParamType::param_unknown) {
		return "unknown";
	}
	else if (type == ParamType::param_bool) {
		return "bool";
	}
}

// 处理鼠标事件
void ScriptCompiler::handleClickFunction(const QString& funcName,
	const QStringList& params,
	std::function<void()> clickFunc,
	std::function<void(int, int)> clickFuncWithPos)
{
	try {
		if (params.isEmpty()) {
			// 调用无参数版本
			if (clickFunc) {
				clickFunc();
			}
			else {
				throw std::runtime_error(
					QString("%1 without parameters is not supported").arg(funcName).toStdString());
			}
		}
		else if (params.size() == 1) {
			// 调用单参数版本
			ParamType paramType[1] = { ParamType::param_point };
			QPoint point = parseParamStr(params.at(0), paramType[0]).toPoint();
			if (clickFuncWithPos) {
				clickFuncWithPos(point.x(), point.y());
			}
			else {
				throw std::runtime_error(
					QString("%1 with coordinates is not supported").arg(funcName).toStdString());
			}
		}
		else if (params.size() == 2) {
			// 调用带坐标版本
			if (clickFuncWithPos) {
				ParamType paramTypes[2] = { ParamType::param_int, ParamType::param_int };
				int x = parseParamStr(params.at(0), paramTypes[0]).toInt();
				int y = parseParamStr(params.at(1), paramTypes[1]).toInt();
				clickFuncWithPos(x, y);
			}
			else {
				throw std::runtime_error(
					QString("%1 with coordinates is not supported").arg(funcName).toStdString());
			}
		}
		else {
			throw std::runtime_error(
				QString("%1 expects 0 or 2 parameters, got %2")
				.arg(funcName)
				.arg(params.size())
				.toStdString());
		}
	}
	catch (const std::exception& e) {
		emit sendRunInfo(QString("Error in %1: %2").arg(funcName).arg(e.what()));
		throw; // 重新抛出异常，由上层处理
	}
}

// 检查参数数量
void ScriptCompiler::checkParamCount(const QString& funcName, const QStringList& params, int expectedCount)
{
	if (params.size() != expectedCount) {
		throw std::runtime_error(
			QString("%1 expects %2 parameters, got %3")
			.arg(funcName)
			.arg(expectedCount)
			.arg(params.size())
			.toStdString());
	}
}

// 判断语句类型
StatementType ScriptCompiler::judgeStatementType(QString line)
{
	// 检查赋值语句
	QRegularExpression assignmentRegex(R"(^(?P<varName>\w+)\s*=\s*(?P<value>.+)$)");
	QRegularExpressionMatch assignmentMatch = assignmentRegex.match(line);
	if (assignmentMatch.hasMatch()) {
		return StatementType::assignment;
	}

	// 原有的函数判断
	QRegularExpression functionRegex(R"(^(?P<functionName>\w+)\s*\((?P<params>.*?)\))");
	QRegularExpressionMatch functionMatch = functionRegex.match(line);
	if (functionMatch.hasMatch()) {
		return StatementType::function;
	}

	return StatementType::unknown;
}

// 设置变量值
void ScriptCompiler::setVariableStr(QString name, QVariant value)
{
	m_variables[name] = value;
}

// 打印变量
void ScriptCompiler::printVariables()
{
	qDebug() << "Variables: ";
	for (auto it = m_variables.begin(); it!= m_variables.end(); ++it) {
		qDebug() << it.key() << " = " << it.value();
	}
}
