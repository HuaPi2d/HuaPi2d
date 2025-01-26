#pragma once

#include <QObject>
#include <QFile>
#include <QRegularExpression>
#include <QRegularExpressionMatch>

#include "ssjjCore/ssjjCoreFun.h"


// 语句类型
enum StatementType {
	comment = 0,
	function,
};

class ScriptCompiler  : public QObject
{
	Q_OBJECT

public:
	ScriptCompiler(QString script, int userMoveSpeed = 100, QObject *parent = nullptr);
	~ScriptCompiler();

	static ScriptCompiler* getInstance(QString script, int userMoveSpeed = 100, QObject* parent = nullptr);
	static void runScript(QString script, int userMoveSpeed = 120);
	static void parseFunction(QString command);

private:
	// 判断语句类型
	static StatementType judgeStatementType(QString function);
};
