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

private:
	QFileInfo m_fileInfo;

protected:
	void run() override;
};
