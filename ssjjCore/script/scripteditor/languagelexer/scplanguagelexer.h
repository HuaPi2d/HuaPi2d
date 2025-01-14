#pragma once

#include <Qsci/qscilexercustom.h>
#include <Qsci/qsciscintilla.h>

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
	// 对字符类型设定样式
	void setupStyles();

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
};
