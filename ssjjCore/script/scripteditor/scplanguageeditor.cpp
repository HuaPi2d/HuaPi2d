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
	apis->load("api/Scriptal-1.0.0.api");
	apis->prepare();

	
}

ScpLanguageEditor::~ScpLanguageEditor()
{
	
}

