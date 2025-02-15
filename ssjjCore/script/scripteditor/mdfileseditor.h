#ifndef MDFILESEDITOR_H
#define MDFILESEDITOR_H


#include "languageeditor.h"
#include "ssjjCore/script/scripteditor/languagelexer/markdownlexer.h"

class MDFilesEditor  : public LanguageEditor
{
public:
	MDFilesEditor(QWidget *parent);
	~MDFilesEditor();

	MarkdownLexer* lexer;
};


#endif // MDFILESEDITOR_H