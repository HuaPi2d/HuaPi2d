#include "globalvariableqstring.h"

GlobalVariableQString::GlobalVariableQString(QObject *parent)
	: QObject(parent)
{}

GlobalVariableQString::~GlobalVariableQString()
{}

GlobalVariableQString* GlobalVariableQString::instance()
{
	static GlobalVariableQString* instance;
	return instance;
}

QString GlobalVariableQString::value() const
{
	return m_value;
}

void GlobalVariableQString::setValue(const QString& value)
{
	m_value = value;
	emit valueChanged(m_value);
}
