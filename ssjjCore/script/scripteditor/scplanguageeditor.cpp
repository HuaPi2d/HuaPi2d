#include "scplanguageeditor.h"

ScpLanguageEditor::ScpLanguageEditor(QWidget *parent)
	: LanguageEditor(parent)
{
	// 设置词法分析器
	lexer = new ScpLanguageLexer(this);
	lexer->setDefaultColor(QColor(0, 0, 0));
	this->setLexer(lexer);

	// 设置代码提示 api
	apis = new QsciAPIs(lexer);
	apis->load("api/Scriptal-1.1.1.api");
	apis->prepare();

	// 连接信号和槽
	connect(this->m_signalsHelper, &LanguageEditorSignalsHelper::resetStyles, lexer, &ScpLanguageLexer::resetStyles);
	connect(this->m_signalsHelper, &LanguageEditorSignalsHelper::resetStyles, this, &ScpLanguageEditor::resetStyles);
}

ScpLanguageEditor::~ScpLanguageEditor()
{}


