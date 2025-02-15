#include "scplanguagelexer.h"


ScpLanguageLexer::ScpLanguageLexer(QObject *parent)
	: QsciLexerCustom(parent)
{
}

ScpLanguageLexer::~ScpLanguageLexer()
{}

const char* ScpLanguageLexer::language() const
{
	return "Scriptal";
}

int ScpLanguageLexer::styleBitsNeeded() const
{
	return 7;
}

QString ScpLanguageLexer::description(int style) const
{
	switch (style) {
		case Default:
			return "Default";
		case Keyword:
			return "Keyword";
		case Number:
			return "Number";
		case String:
			return "String";
		case Operator:
			return "Operator";
		case Function:
			return "Function";
		case Variable:
			return "Variable";
		case Comment:
			return "Comment";
	}
	return QString();
}

void ScpLanguageLexer::styleText(int start, int end)
{
	// 获取文本内容
	QString text = editor()->text();
	static const QRegularExpression tokenizer("(?<!\\\\)\"\"\"|(?<!\\\\)'''|#|" // 注释
		"(?<!\\\\)\"|(?<!\\\\)'|"  // 字符串
		"\\s+|"  // 空字符串
		"-\\d+|" // 数字
		"\\w+|"  // 文本，变量
		"\\W"  // 操作符
		, QRegularExpression::UseUnicodePropertiesOption);
	static const QRegularExpression linebreak("\\s*(\\n|\\r)+\\s*");
	static const QRegularExpression keyword("\\b(if|else|elif|for|while|break|continue|return|def|class|import|from|as|try|except|finally|with|assert|del|global|nonlocal|lambda|yield|and|or|not|in|is|not|None|True|False)\\b");
	static const QRegularExpression variable("\\b[a-zA-Z_][a-zA-Z0-9_]*\\b");
	QRegularExpressionMatchIterator tokenIter = tokenizer.globalMatch(text);
	QRegularExpressionMatch match, prev;

	// 标志符
	bool isSharpComment = false, isInt;
	int style = 0, length = 0;
	enum StringType { Comment1 = 0, Comment2, String1, String2, StringTypeCount };
	static const char* StringTypeIndicator[StringTypeCount] = { "\"\"\"" , "'''"  , "\""  , "'" };

	auto renderText = [&]() {
		if (style & (1 << Comment1 | 1 << Comment2) || isSharpComment) {
			setStyling(length, Comment);
		}
		else if (style & (1 << String1 | 1 << String2)) {
			setStyling(length, String);
		}
		else {
			setStyling(length, Default);
		}
		length = 0;
		};
	
	// 开启文本渲染
	startStyling(0);

	// 遍历文本内容，进行渲染
	while (tokenIter.hasNext())
	{
		prev = match;
		match = tokenIter.next();
		QString token = match.captured(0);

		if (linebreak.match(token).hasMatch()) {
			renderText();
			isSharpComment = false;
			length += token.toUtf8().length();
			continue;
		}
		if (!style && token == "#") {
			renderText(); length = 0;
			isSharpComment = true;
			length += token.toUtf8().length();
			continue;
		}
		if (!style && !isSharpComment) {
			if (token == ",") {
				renderText(); length = 0;
				setStyling(match.capturedLength(), Operator);
				continue;
			}
			else if (token == "(" || token == ")" || token == "=") {
				renderText(); length = 0;
				setStyling(match.capturedLength(), Variable);
				continue;
			}
			else if (keyword.match(token).hasMatch()) {
				renderText(); length = 0;
				setStyling(match.capturedLength(), Keyword);
				continue;
			}
			else if (variable.match(token).hasMatch()) {
				renderText(); length = 0;
				setStyling(match.capturedLength(), Variable);
				continue;
			}
			Q_UNUSED(token.toInt(&isInt));
			if (isInt) {
				renderText(); length = 0;
				setStyling(match.capturedLength(), Number);
				continue;
			}
		}
		if (!isSharpComment) {
			for (int i = 0; i < StringTypeCount; i++) {
				if (token == StringTypeIndicator[i]) {
					if (style & (1 << i)) {
						length += match.capturedLength();
						renderText();
						length -= match.capturedLength();
						style &= ~(1 << i);
					}
					else {
						renderText();
						style |= 1 << i;
					}
					break;
				}
			}
		}

		length += token.toUtf8().length();
	}
	if (length) {
		renderText();
	}
}

void ScpLanguageLexer::resetStyles(EditorConfig editorConfig)
{
	m_editorConfig = editorConfig;

	// 设置字体
	QFont font = QFont(m_editorConfig.font.font);
	font.setPointSize(m_editorConfig.font.size);
	setFont(font);

	// 设定特定类型字符的颜色
	setPaper(m_editorConfig.color.background);
	setColor(m_editorConfig.color.foreground);
	setColor(m_editorConfig.color.text, Default);
	setColor(m_editorConfig.color.keyword, Keyword);
	setColor(m_editorConfig.color.number, Number);
	setColor(m_editorConfig.color.string, String);
	setColor(m_editorConfig.color.operateur, Operator);
	setColor(m_editorConfig.color.function, Function);
	setColor(m_editorConfig.color.variable, Variable);
	setColor(m_editorConfig.color.comment, Comment);
}


