#pragma once

#include <QObject>
#include <QDebug>
#include <QString>


class GlobalVariableQString  : public QObject
{
	Q_OBJECT
	Q_PROPERTY(QString value READ value WRITE setValue NOTIFY valueChanged)

public:
	GlobalVariableQString(QObject *parent);
	~GlobalVariableQString();

	static GlobalVariableQString* instance();
	QString value() const;
	void setValue(const QString& value);

signals:
	void valueChanged(const QString& value);

private:
	QString m_value;
};
