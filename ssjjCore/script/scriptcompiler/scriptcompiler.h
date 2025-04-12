#pragma once

#include <QObject>
#include <QFile>
#include <QRegularExpression>
#include <QRegularExpressionMatch>

#include "struct/ssjj/ssjjstruct.h"
#include "ssjjCore/ssjjCoreFun.h"
#include "ssjjCore/scriptApis/scplanfunction.h"

// 语句类型
enum StatementType {
	comment = 0,
	function,
	assignment,
	unknown,
};

enum ParamType {
	param_int = 0,
	param_string,
	param_bool,
	param_point,
	param_unknown,
};

typedef struct Param {
	ParamType type;
	QVariant value;
};

class ScriptCompiler : public QObject
{
	Q_OBJECT

public:
	ScriptCompiler(QString script, int userMoveSpeed = 120, QObject* parent = nullptr);
	~ScriptCompiler();

	static ScriptCompiler* getInstance(QString script, int userMoveSpeed = 100, QObject* parent = nullptr);
	void runScript();
	QVariant parseFunction(QString functionName, QString params);
	// 读取文件内容
	void readFileContent();
	// 删除注释内容
	void removeComment();
	// 获取加载时间
	void setLoadTime(int time);
	// 处理单行代码
	StatementType judgeStatementType(QString line);
	void processStatement(const QString& line);         // 处理单行语句
	QVariant processFunctionCall(const QString& line);  // 处理函数调用语句
	void processAssignment(const QString& line);        // 处理赋值语句
	// 解析参数
	QVariant parseParamStr(QString param, ParamType type, bool throwError = true);
	// 解析变量
	Param getValueAndType(QString valueStr);
	QVariant parseVariable(QString variable, ParamType type, bool throwError = true);
	// 获取参数类型字符串
	QString getParamTypeString(ParamType type);

	void handleClickFunction(const QString& funcName,
		const QStringList& params,
		std::function<void()> clickFunc,
		std::function<void(int, int)> clickFuncWithPos);

	void checkParamCount(const QString& funcName,
		const QStringList& params,
		int expectedCount);

public: signals:
	void sendRunInfo(QString info);

private:
	QString m_scriptFilePath;
	int m_userMoveSpeed;
	int m_scriptMoveSpeed;
	int m_loadTime;
	QString m_fileContent;
	QString m_code;
	QFileInfo m_fileInfo;
	QMap<QString, QString> m_fileAttributesMap;
	struct SingleTask m_task;
	QMap<QString, QVariant> m_variables; // 存储变量的映射表

	void setVariableStr(QString variable, QVariant value); // 设置变量值

	// 输出变量区的全部内容
	void printVariables();
};
