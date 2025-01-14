#pragma once

#include "languageeditor.h"
#include <Qsci/qscilexercpp.h>
#include <Qsci/qsciapis.h>


class CppLanguageEditor  : public LanguageEditor
{
public:
	CppLanguageEditor(QWidget *parent);
	~CppLanguageEditor();

private:
	QsciLexerCPP *cppLexer;
	QsciAPIs *apis;
};
