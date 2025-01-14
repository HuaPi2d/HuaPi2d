#pragma once

#include "languageeditor.h"
#include "languagelexer/scplanguagelexer.h"
#include <Qsci/qsciapis.h>

class ScpLanguageEditor  : public LanguageEditor
{
	/*Q_OBJECT*/

public:
	ScpLanguageEditor(QWidget *parent);
	~ScpLanguageEditor();

private:
	ScpLanguageLexer *lexer;
	QsciAPIs *apis;
};
