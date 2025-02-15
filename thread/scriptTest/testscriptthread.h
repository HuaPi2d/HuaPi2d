#pragma once

#include <QThread>
#include <QMessageBox>
#include <QString>
#include "ssjjCore/script/scriptcompiler/scriptcompiler.h"

class TestScriptThread  : public QThread
{
	Q_OBJECT

public:
	TestScriptThread(QFileInfo scriptFileInfo, QObject *parent);
	~TestScriptThread();

public: signals:
	void sendWarningMessage(QString title, QString message);
	void sendRunInfo(QString info);

private:
	QFileInfo m_fileInfo;
	QList<FileAttribute> m_fileAttributes;
	ScriptCompiler* m_compiler;
	int m_speed;

protected:
	void run() override;
};
