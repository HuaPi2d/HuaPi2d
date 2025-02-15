#include "mdfileseditor.h"

MDFilesEditor::MDFilesEditor(QWidget *parent)
	: LanguageEditor(parent)
{
	// 初始化语法分析器
	lexer = new MarkdownLexer(this);
	lexer->setDefaultColor(QColor(0, 0, 0));
	this->setLexer(lexer);

	// 连接信号和槽
	connect(this->m_signalsHelper, &LanguageEditorSignalsHelper::resetStyles, lexer, &MarkdownLexer::resetStyles);
}

MDFilesEditor::~MDFilesEditor()
{}

