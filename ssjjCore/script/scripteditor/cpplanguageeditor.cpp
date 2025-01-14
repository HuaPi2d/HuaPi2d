#include "cpplanguageeditor.h"

CppLanguageEditor::CppLanguageEditor(QWidget *parent)
	: LanguageEditor(parent)
{
	cppLexer = new QsciLexerCPP(this);
	cppLexer->setDefaultColor(QColor(0, 0, 0));
	this->setLexer(cppLexer);
	
	apis = new QsciAPIs(cppLexer);
	apis->prepare();
}

CppLanguageEditor::~CppLanguageEditor()
{}
