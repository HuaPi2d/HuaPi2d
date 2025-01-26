#include "scplanguagelexer.h"


ScpLanguageLexer::ScpLanguageLexer(EditorConfig editorConfig, QObject *parent)
	: QsciLexerCustom(parent)
{
	m_editorConfig = editorConfig;
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
	setColor(m_editorConfig.color.text, Default);
	setColor(m_editorConfig.color.keyword, Keyword);
	setColor(m_editorConfig.color.number, Number);
	setColor(m_editorConfig.color.string, String);
	setColor(m_editorConfig.color.operateur, Operator);
	setColor(m_editorConfig.color.function, Function);
	setColor(m_editorConfig.color.variable, Variable);
	setColor(m_editorConfig.color.comment, Comment);
}

