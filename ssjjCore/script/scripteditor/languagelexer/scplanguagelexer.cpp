#include "scplanguagelexer.h"

ScpLanguageLexer::ScpLanguageLexer(QObject *parent)
	: QsciLexerCustom(parent)
{
	this->setupStyles();
}

ScpLanguageLexer::~ScpLanguageLexer()
{}

const char* ScpLanguageLexer::language() const
{
	return "Scriptal";
}

int ScpLanguageLexer::styleBitsNeeded() const
{
	return 7;
}

QString ScpLanguageLexer::description(int style) const
{
	switch (style) {
		case Default:
			return "Default";
		case Keyword:
			return "Keyword";
		case Number:
			return "Number";
		case String:
			return "String";
		case Operator:
			return "Operator";
		case Function:
			return "Function";
		case Variable:
			return "Variable";
		case Comment:
			return "Comment";
	}
	return QString();
}

void ScpLanguageLexer::styleText(int start, int end)
{
	// 获取文本内容
	QString text = editor()->text(start, end);
}

void ScpLanguageLexer::setupStyles()
{
	// 设定特定类型字符的颜色
	setColor(QColor("#000000"), Default);
	setColor(QColor("#0000FF"), Keyword);
	setColor(QColor("#FF0000"), Number);
	setColor(QColor("#008000"), String);
	setColor(QColor("#FF00FF"), Operator);
	setColor(QColor("#800080"), Function);
	setColor(QColor("#000080"), Variable);
	setColor(QColor("#808080"), Comment);

	// 设定字体
	// 省略
}

