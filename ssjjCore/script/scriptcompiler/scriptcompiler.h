#pragma once

#include <QObject>
#include <QFile>

#include "ssjjCore/ssjjCoreFun.h"

class ScriptCompiler  : public QObject
{
	Q_OBJECT

public:
	ScriptCompiler(QString script, int userMoveSpeed = 100, QObject *parent = nullptr);
	~ScriptCompiler();

	static ScriptCompiler* getInstance(QString script, int userMoveSpeed = 100, QObject* parent = nullptr);
	static void runScript(QString script, int userMoveSpeed = 120);
	static void parseCommand(QString command, int userMoveSpeed);

private:

};
