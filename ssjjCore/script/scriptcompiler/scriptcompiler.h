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
};

class ScriptCompiler : public QObject
{
	Q_OBJECT

public:
	ScriptCompiler(QString script, int userMoveSpeed = 120, QObject* parent = nullptr);
	~ScriptCompiler();

	static ScriptCompiler* getInstance(QString script, int userMoveSpeed = 100, QObject* parent = nullptr);
	void runScript();
	bool parseFunction(QString functionName, QString params);
	// 读取文件内容
	void readFileContent();
	// 删除注释内容
	void removeComment();
	// 提取函数内容
	void extractFunction();

public: signals:
	void sendRunInfo(QString info);

private:
	// 判断语句类型
	StatementType judgeStatementType(QString function);

	QString m_scriptFilePath;
	int m_userMoveSpeed;
	int m_scriptMoveSpeed;
	QString m_fileContent;
	QString m_code;
	QFileInfo m_fileInfo;
	QList<FileAttribute> m_fileAttributes;
	struct SingleTask m_task;
};
