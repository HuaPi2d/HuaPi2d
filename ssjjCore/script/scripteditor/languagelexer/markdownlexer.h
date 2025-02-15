#pragma once

#include <Qsci/qscilexercustom.h>
#include <QRegularExpression>
#include <Qsci/qsciscintilla.h>
#include <bitset>

#include "struct/editor/EditorConfig.h"


class MarkdownLexer  : public QsciLexerCustom
{
public:
	MarkdownLexer(QObject *parent);
	~MarkdownLexer();

public slots:
	void resetStyles(EditorConfig config);

protected:
	// 返回词法分析器的语言名称
	const char *language() const override;
	// 定义支持的样式数量
	int styleBitsNeeded() const override;
	// 返回每种样式的描述
	QString description(int style) const override;
	// 定义语法高亮逻辑
	void styleText(int start, int end) override;

private:
	EditorConfig m_editorConfig;

	enum {
		Default = 0, // 默认样式
		Header1, Header2, Header3, Header4, Header5, Header6, // 标题
		Monospace, // 等宽字体
		StrikeMono, // 删除线字体
		List, // 列表
		Ruler, // 分割线
		Bold, Italic, Strike, BoldItalic, StrikeBold, StrikeItalic, StrikeBoldItalic,
		Quote, // 引用
		BoldQuote, ItalicQuote, StrikeQuote, BoldItalicQuote, StrikeBoldQuote, StrikeItalicQuote, StrikeBoldItalicQuote, 
		Brackets, // 方括号
		Brace = Quote, // 花括号
		Tag = Brackets // 标签
	};
};
