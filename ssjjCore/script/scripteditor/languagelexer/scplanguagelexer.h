#ifndef SCPLANGUAGELEXER_H
#define SCPLANGUAGELEXER_H

#include <Qsci/qscilexercustom.h>
#include <Qsci/qsciscintilla.h>
#include <QSettings>
#include <QRegularExpression>

#include "struct/editor/EditorConfig.h"


class ScpLanguageLexer  : public QsciLexerCustom
{
public:
	ScpLanguageLexer(QObject *parent);
	~ScpLanguageLexer();

protected:
	// 返回词法分析器的语言名称
	const char *language() const override;
	// 定义支持的样式数量
	int styleBitsNeeded() const override;
	// 返回每种样式的描述
	QString description(int style) const override;
	// 定义语法高亮的逻辑
	void styleText(int start, int end) override;

private:
	// 样式类型枚举
	enum {
		Default = 0,
		Keyword,   // 关键字
		Number,    // 数字
		String,    // 字符串
		Operator,  // 运算符
		Function,  // 函数
		Variable,  // 变量
		Comment,   // 注释
	};

	QStringList keywords;  // 关键字列表
	EditorConfig m_editorConfig;  // 编辑器配置

public slots:
	void resetStyles(EditorConfig editorConfig);
};

#endif // SCPLANGUAGELEXER_H
