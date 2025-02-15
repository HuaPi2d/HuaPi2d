#include "markdownlexer.h"

MarkdownLexer::MarkdownLexer(QObject *parent)
	: QsciLexerCustom(parent)
{}

MarkdownLexer::~MarkdownLexer()
{}

const char* MarkdownLexer::language() const
{
	return "Markdown";
}

int MarkdownLexer::styleBitsNeeded() const
{
	return 27;
}

QString MarkdownLexer::description(int style) const
{
	switch (style) {
	case Default:
		return "Default";
	case Header1:
		return "Header1";
	case Header2:
		return "Header2";
	case Header3:
		return "Header3";
	case Header4:
		return "Header4";
	case Header5:
		return "Header5";
	case Header6:
		return "Header6";
	case Monospace:
		return "Monospace";
	case StrikeMono:
		return "StrikeMono";
	case List:
		return "List";
	case Ruler:
		return "Ruler";
	case Bold:
		return "Bold";
	case Italic:
		return "Italic";
	case Strike:
		return "Strike";
	case BoldItalic:
		return "BoldItalic";
	case StrikeBold:
		return "StrikeBold";
	case StrikeItalic:
		return "StrikeItalic";
	case StrikeBoldItalic:
		return "StrikeBoldItalic";
	case Quote:
		return "Quote";
	case BoldQuote:
		return "BoldQuote";
	case ItalicQuote:
		return "ItalicQuote";
	case StrikeQuote:
		return "StrikeQuote";
	case BoldItalicQuote:
		return "BoldItalicQuote";
	case StrikeBoldQuote:
		return "StrikeBoldQuote";
	case StrikeItalicQuote:
		return "StrikeItalicQuote";
	case StrikeBoldItalicQuote:
		return "StrikeBoldItalicQuote";
	case Brackets:
		return "Brackets";
	default:
		return "";
	}
}


void MarkdownLexer::styleText(int start, int end)
{
	// 获取文本内容
	QString text = editor()->text(start, end);

	// 定义正则表达式
    static const QRegularExpression tokenizer("^######|^#####|^####|^###|^##|---+|====+|~~|"
        "\\s+|[^\\\\]??\\*\\*\\*|[^\\\\]??\\*\\*|"
        "[^\\\\]??___|[^\\\\]??__|_|"
        "[A-Za-z\\d\\p{Script=Han}]+|\\W", QRegularExpression::UseUnicodePropertiesOption);  // 匹配 token A-Za-z\\d\\p{Script=Han}
    static const QRegularExpression notSpace("[^\\s]", QRegularExpression::UseUnicodePropertiesOption);// 匹配任何非空白字符
    static const QRegularExpression linebreak("\\s*(\\n|\\r)+\\s*");                  // 匹配行间空白字符
	static const QRegularExpression header1("====+");
	static const QRegularExpression header2("----+");

	// 处理文本为 token 集
    QRegularExpressionMatchIterator tokenIter = tokenizer.globalMatch(text);

	// 定义字体类型及其标识符
	bool newline = true, quote = false, tag = false, bracket = false, brace = false, isInt;
	int header = 0, length = 0, style = 0;
	enum FontStyle { I1 = 0, B1, BI1, I2, B2, BI2, S, Mono, FontStyleCount };
	static const char* fsIndicator[FontStyleCount] = { "*", "**", "***", "_", "__", "___", "~~", "`" };

	// 文本渲染 lambda 函数
	auto renderText = [&]() {
		// 处理标题文本
		if (header != 0) {
			setStyling(length, header);
		}
		// 处理非标题文本
		else {
			// `` 包裹的文本
			if (style & (1 << Mono)) {
				setStyling(length, style & (1 << S) ? StrikeMono : Monospace);
			}
			// 引用文本
			else if (quote) {
				if (style & (1 << S)) {
					if (style & (1 << I1 | 1 << I2)) setStyling(length, StrikeItalicQuote);
					else if (style & (1 << B1 | 1 << B2)) setStyling(length, StrikeBoldQuote);
					else if (style & (1 << BI1 | 1 << BI2)) setStyling(length, StrikeBoldItalicQuote);
					else setStyling(length, StrikeQuote);
				}
				else {
					if (style & (1 << I1 | 1 << I2)) setStyling(length, ItalicQuote);
					else if (style & (1 << B1 | 1 << B2)) setStyling(length, BoldQuote);
					else if (style & (1 << BI1 | 1 << BI2)) setStyling(length, BoldItalicQuote);
					else setStyling(length, Quote);
				}
			}
			else if (style & (1 << S)) {
				if (style & (1 << I1 | 1 << I2)) setStyling(length, StrikeItalic);
				else if (style & (1 << B1 | 1 << B2)) setStyling(length, StrikeBold);
				else if (style & (1 << BI1 | 1 << BI2)) setStyling(length, StrikeBoldItalic);
				else setStyling(length, Strike);
			}
			else if (style & (1 << I1 | 1 << I2)) {
				setStyling(length, Italic);
			}
			else if (style & (1 << B1 | 1 << B2)) {
				setStyling(length, Bold);
			}
			else if (style & (1 << BI1 | 1 << BI2)) {
				setStyling(length, BoldItalic);
			}
			else {
				setStyling(length, Default);
			}
		}
		length = 0;
		};

	// 开始渲染文本
	startStyling(start);

	QRegularExpressionMatch match, prev;
	qDebug() << "======================";
	while (tokenIter.hasNext())
	{
		prev = match;
		match = tokenIter.next();
		QString token = match.captured(0);
		//qDebug() << "token:" << token;
		//continue;
		// 输出当前长度及当前 token 和下一个 token
		// qDebug() << "current length:" << length << " token:" << token << " next token:" << (tokenIter.hasNext() ? tokenIter.peekNext().captured(0) : "");
#define NEXT_IS_WORDS tokenIter.hasNext() && !notSpace.match(tokenIter.peekNext().captured(0)).hasMatch()
#define NEXT_IS_LINEBREAK tokenIter.hasNext() && linebreak.match(tokenIter.peekNext().captured(0)).hasMatch()
		if (linebreak.match(token).hasMatch()) {
			// 匹配到换行，并且当前文本为标题或引用文本，则渲染文本
			if (header || quote) {
				renderText();
			}
			newline = true;          // 标记为新行
			header = quote = false;  // 清空标题和引用标记
			length += match.capturedLength(0);   // 增加长度
			qDebug() << "linebreak token:" << token;
			continue;
		}
		if (header) {
			length += token.toUtf8().length();
			qDebug() << "header token:" << token;
			continue;
		}
		if (bracket) {
			// 到达右括号，渲染文本
			if (token == "]") {
				setStyling(length+1, Brackets);
				length = 0;
				bracket = false;
				continue;
			}
			// 处理非右括号字符
			else {
				length += token.toUtf8().length();
				continue;
			}
		}
		if (tag) {
			// 到达右尖括号，渲染文本
			if (token == ">") {
				if (length) {
					setStyling(length, Quote);
				}
				setStyling(1, Tag);
				length = 0;
				tag = false;
				continue;
			}
			// 处理非右尖括号字符
			if (tokenIter.hasNext() && tokenIter.peekNext().captured(0) == "=") {
				setStyling(length, Quote);                   // 处理属性值
				setStyling(token.toUtf8().length(), List);   // 处理属性名
				setStyling(1, Default);                      // "="
				length = 0;
				tokenIter.next();
				continue;
			}
			length += token.toUtf8().length();
			continue;
		}
		if (quote) {
			qDebug() << "qoute token:" << token;
		}
		// 新行并且非引用文本
		if (newline && !quote) {
			// 匹配到标题
			if (token == "######" && NEXT_IS_WORDS) {
				renderText();         // 首先渲染上一行文本
				newline = false;
				header = 6;
				qDebug() << "header token:" << token;
			}
			else if (token == "#####" && NEXT_IS_WORDS) {
				renderText();
				newline = false;
				header = 5;
				qDebug() << "header token:" << token;
			}
			else if (token == "####" && NEXT_IS_WORDS) {
				renderText();
				newline = false;
				header = 4;
				qDebug() << "header token:" << token;
			}
			else if (token == "###" && NEXT_IS_WORDS) {
				renderText();
				newline = false;
				header = 3;
				qDebug() << "header token:" << token;
			}
			else if (token == "##" && NEXT_IS_WORDS) {
				renderText();
				newline = false;
				header = 2;
				qDebug() << "header token:" << token;
			}
			else if (token == "#" && NEXT_IS_WORDS) {
				renderText();
				newline = false;
				header = 1;
				qDebug() << "header token:" << token;
			}
			else if (token == ">" && NEXT_IS_WORDS) {
				renderText();
				newline = false;
				quote = true;   // 标记为引用文本
			}
			else if (token == "---" && NEXT_IS_LINEBREAK) {
				renderText();
				newline = false;
				setStyling(3, Ruler);
				qDebug() << "ruler token:" << token;
				continue;
			}
			else if (token == "*" || token == "-" || token == "+" && NEXT_IS_WORDS) {
				renderText();
				newline = false;
				setStyling(1, List);
				qDebug() << "list token:" << token;
				continue;
			}
			else if ((header1.match(token).hasMatch() || header2.match(token).hasMatch()) &&
				tokenIter.hasNext() && linebreak.match(tokenIter.peekNext().captured(0)).hasMatch()) {
				renderText();
				newline = false;
				setStyling(match.capturedLength(0), BoldQuote);
				qDebug() << "boldquote token:" << token;
				break;
				continue;
			}
			else {
				Q_UNUSED(token.toInt(&isInt));
				if (isInt && tokenIter.hasNext() && tokenIter.peekNext().captured(0) == ".") {
					renderText();
					newline = false;
					setStyling(match.capturedLength(0) + 1, List);
					tokenIter.next();
					continue;
				}
			}
		}
		// 处理任何非空白文本
		if (newline && notSpace.match(token).hasMatch()) {
			newline = false;
			qDebug() << "newline non-ws token:" << token;
		}
		// 遇到方括号
		if (token == "[") {
			renderText();
			bracket = true;
		}
		// 遇到[]()链接类型
		else if(token == "(" && !length && prev.captured(0) == "]")
			brace = true;
		else if (token == "<" && !(NEXT_IS_WORDS)) {
			renderText();
			length = 1;
			tag = true;
			while (!(NEXT_IS_WORDS)) {
				token = tokenIter.next().captured(0);
				if (token == ">") {
					++length;
					tag = false;
					break;
				}
				length += token.toUtf8().length();
			}
			setStyling(length, Tag);
			length = 0;
			continue;
		}
		// 未转义标识符号
		else if(!prev.captured(0).endsWith("\\")) {
			for (int fs = I1; fs < FontStyleCount; ++fs) {
				if (token == fsIndicator[fs]) {
					//qDebug() << "stylish token:" << prev.captured(0);
					std::bitset<8> styleBits(style);
					std::bitset<8> fsBits(1 << fs);
					// 尾标识符
					if ((style & (1 << fs))) {
						qDebug() << "stylish token:" << prev.captured(0);
						qDebug() << "tail fsIndicator token:" << token;
						length += match.capturedLength(0);
						renderText();
						length -= match.capturedLength(0);
						style &= ~(1 << fs);
					}
					// 首标识符
					else if (!(style & (1 << fs))) {
						qDebug() << "head fsIndicator token:" << token;
						renderText();
						style |= (1 << fs);
					}
					break;
				}
			}
		}
		length += token.toUtf8().length();
	}
	if (length) 
		renderText();
	qDebug() << "======================";
}

void MarkdownLexer::resetStyles(EditorConfig config)
{
	m_editorConfig = config;

	// 设定特定类型字符的颜色
	QColor bg = m_editorConfig.color.background;
	QColor fg = m_editorConfig.color.foreground;
	QColor mid = QColor((2 * bg.red() + fg.red()) / 3, (2 * bg.green() + fg.green()) / 3, (2 * bg.blue() + fg.blue()) / 3);
	setPaper(bg);
	setColor(fg);
	setColor(fg, Default);
	setColor(m_editorConfig.color.number, List);
	setColor(m_editorConfig.color.operateur, Brackets);
	for (int i = Quote; i <= StrikeBoldItalicQuote; ++i)
		setColor(m_editorConfig.color.string, i);
	setColor(mid, Ruler);

	QFont font = QFont(m_editorConfig.font.font);
	font.setPointSize(m_editorConfig.font.size);
	setFont(font);
	setFont(font, Default);

	QFont bold = font;
	bold.setBold(true);
	setFont(bold, Bold);
	setFont(bold, BoldQuote);

	QFont italic = font;
	italic.setItalic(true);
	setFont(italic, Italic);
	setFont(italic, ItalicQuote);

	QFont boldItalic = font;
	boldItalic.setBold(true);
	boldItalic.setItalic(true);
	setFont(boldItalic, BoldItalic);
	setFont(boldItalic, BoldItalicQuote);

	QFont mono = m_editorConfig.font.font;
	setFont(mono, Monospace);
	setColor(m_editorConfig.color.keyword, Monospace);
	mono.setStrikeOut(true);
	setFont(mono, StrikeMono);
	setColor(m_editorConfig.color.keyword, StrikeMono);

	QFont header = font;
	float factor = 1.4;
	header.setBold(true);
	for (int i = Header1; i <= Header6; ++i) {
		header.setPointSize(qCeil(factor * font.pointSize()));
		setFont(header, i);
		factor -= 0.1;
	}

	setColor(mid, Strike);
	setColor(mid, StrikeItalic);
	setColor(mid, StrikeBold);
	setColor(mid, StrikeBoldItalic);

	fg = m_editorConfig.color.string;
	mid = QColor((2 * bg.red() + fg.red()) / 3, (2 * bg.green() + fg.green()) / 3, (2 * bg.blue() + fg.blue()) / 3);
	setColor(mid, StrikeQuote);
	setColor(mid, StrikeBoldQuote);
	setColor(mid, StrikeItalicQuote);
	setColor(mid, StrikeBoldItalicQuote);
}


