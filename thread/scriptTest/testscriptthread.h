#pragma once

#include <QThread>
#include <QMessageBox>
#include <QString>
#include "ssjjCore/script/scriptcompiler/scriptcompiler.h"

class TestScriptThread  : public QThread
{
	Q_OBJECT

public:
	TestScriptThread(QFileInfo scriptFileInfo, int loadingTime = 20, QObject *parent=nullptr);
	~TestScriptThread();

public: signals:
	void sendWarningMessage(QString title, QString message);
	void sendRunInfo(QString info);

private:
	QFileInfo m_fileInfo;
	QMap<QString, QString> m_fileAttributesMap;
	ScriptCompiler* m_compiler;
	int m_speed;
	int m_loadingTime;

protected:
	void run() override;
};
