#ifndef SCPLANGUAGEEDITOR_H
#define SCPLANGUAGEEDITOR_H

#include <Qsci/qsciapis.h>

#include "languageeditor.h"
#include "ssjjCore/script/scripteditor/languagelexer/scplanguagelexer.h"



class ScpLanguageEditor : public LanguageEditor
{
	/*Q_OBJECT*/

public:
	ScpLanguageEditor(QWidget* parent);
	~ScpLanguageEditor();

private:
	ScpLanguageLexer* lexer;
	QsciAPIs* apis;
	EditorConfig m_editorConfig;
};




#endif // SCPLANGUAGEEDITOR_H