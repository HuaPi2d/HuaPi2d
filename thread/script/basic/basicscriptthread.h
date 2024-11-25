#ifndef BASICSCRIPTTHREAD_H
#define BASICSCRIPTTHREAD_H

#include <QThread>

class BasicScriptThread  : public QThread
{
	Q_OBJECT

public:
	BasicScriptThread(QObject *parent);
	~BasicScriptThread();

public slots:
	void receiveDisplayText(QString text);

public: signals:
	void sendDisplayText(QString text);
};

#endif // BASICSCRIPTTHREAD_H
