#include "basicscriptthread.h"

BasicScriptThread::BasicScriptThread(QObject *parent)
	: QThread(parent)
{}

BasicScriptThread::~BasicScriptThread()
{}

void BasicScriptThread::receiveDisplayText(QString text)
{
	emit sendDisplayText(text);
}
