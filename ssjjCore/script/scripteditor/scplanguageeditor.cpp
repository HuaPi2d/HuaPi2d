#include "scplanguageeditor.h"

ScpLanguageEditor::ScpLanguageEditor(EditorConfig config, QWidget *parent)
	: LanguageEditor(parent)
{
	// 设置编辑器配置
	m_editorConfig = config;

	// 设置词法分析器
	lexer = new ScpLanguageLexer(config, this);
	lexer->setDefaultColor(QColor(0, 0, 0));
	this->setLexer(lexer);

	// 设置代码提示 api
	apis = new QsciAPIs(lexer);
	apis->load("api/Scriptal-1.0.0.api");
	apis->prepare();

	// 重置编辑器配色
	resetColors();
}

ScpLanguageEditor::~ScpLanguageEditor()
{}

void ScpLanguageEditor::resetColors()
{
	// 设置字体
	QFont font = QFont(m_editorConfig.font.font);
	font.setPointSize(m_editorConfig.font.size);
	this->setFont(font);
	this->setMarginsFont(font);
	lexer->setFont(font);
	// 设置编辑器的配色
	this->setPaper(m_editorConfig.color.background);
	this->setColor(m_editorConfig.color.foreground);
}

void ScpLanguageEditor::setEditorConfig(EditorConfig config)
{
	m_editorConfig = config;
	resetColors();
}

