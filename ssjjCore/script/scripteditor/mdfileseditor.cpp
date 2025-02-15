#include "mdfileseditor.h"

MDFilesEditor::MDFilesEditor(QWidget *parent)
	: LanguageEditor(parent)
{
	// ��ʼ���﷨������
	lexer = new MarkdownLexer(this);
	lexer->setDefaultColor(QColor(0, 0, 0));
	this->setLexer(lexer);

	// �����źźͲ�
	connect(this->m_signalsHelper, &LanguageEditorSignalsHelper::resetStyles, lexer, &MarkdownLexer::resetStyles);
}

MDFilesEditor::~MDFilesEditor()
{}

