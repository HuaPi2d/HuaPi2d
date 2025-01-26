#include "pythoneditor.h"

PythonEditor::PythonEditor(QWidget *parent)
	: QsciScintilla(parent)
{
	// 设置词法分析器
	textLexer = new QsciLexerPython(this);
	textLexer->setDefaultColor(QColor(0, 0, 0));
	this->setLexer(textLexer);
	// 显示行号
	this->setMarginType(0, QsciScintilla::NumberMargin);
	this->setMarginLineNumbers(0, true);
	this->setMarginWidth(0, 30);
	// 代码提示
	textAPIs = new QsciAPIs(textLexer);
	textAPIs->load("api/Python-3.11.api");
	textAPIs->prepare();

	this->setAutoCompletionSource(QsciScintilla::AcsAll);   // 设置源，自动补全所有地方出现的
	this->setAutoCompletionThreshold(1);  // 设置自动补全的触发字符数
	this->setAutoCompletionCaseSensitivity(true);  // 设置自动补全的大小写敏感
	this->setAutoIndent(true);             // 设置自动缩进
	this->setBraceMatching(QsciScintilla::SloppyBraceMatch);  // 设置括号匹配
	
}

PythonEditor::~PythonEditor()
{}

// 重写部分鼠标事件
// 实现括号，引号匹配输入功能
// 实现括号，引号的联动删除功能
void PythonEditor::keyPressEvent(QKeyEvent * event)
{
	bool isOprate = false;

	switch (event->key())
	{
		case Qt::Key_ParenLeft: // '('
			insertMatchingCharacter("(", ")");
			return;
		case Qt::Key_BracketLeft: // '['
			insertMatchingCharacter("[", "]");
			return;
		case Qt::Key_BraceLeft: // '{'
			insertMatchingCharacter("{", "}");
			return;
		case Qt::Key_QuoteDbl: // '"'
			insertMatchingCharacter("\"", "\"");
			return;
		case Qt::Key_Apostrophe: // '\''
			insertMatchingCharacter("\'", "\'");
			return;
		case Qt::Key_Delete:
		case Qt::Key_Backspace: // 删除括号
			isOprate = deleteAction();
			if (isOprate == true)
				return;
			else
				break;
			
		default:
			break;
	}
	// 对于其他情况，调用父类的方法
	QsciScintilla::keyPressEvent(event);
}

void PythonEditor::insertMatchingCharacter(const QString& openChar, const QString& closeChar)
{
	// 获取当前的光标位置
	int line, index;
	this->getCursorPosition(&line, &index);

	// 插入括号
	this->insert(openChar + closeChar);

	// 将光标移至括号中间
	this->setCursorPosition(line, index + 1);
}

bool PythonEditor::deleteAction()
{
	// 获取当前的光标位置
	int pos = static_cast<int>(SendScintilla(SCI_GETCURRENTPOS));

	// 获取光标左右的符号
	char rightChar = getCharAt(pos);
	char leftChar = pos > 0 ? getCharAt(pos - 1) : '\0';

	if (isMatchingCharacter(leftChar, rightChar))
	{
		deleteMatchingCharacter(pos);
		return true;
	}
	else
	{
		return false;
	}
}

void PythonEditor::deleteMatchingCharacter(int pos)
{
	SendScintilla(SCI_DELETERANGE, pos - 1, 2);
	return;
}

bool PythonEditor::isMatchingCharacter(const char& leftChar, const char& rightChar)
{
	return (leftChar == '(' && rightChar == ')') ||
		(leftChar == '[' && rightChar == ']') ||
		(leftChar == '{' && rightChar == '}') ||
		(leftChar == '\"' && rightChar == '\"') ||
		(leftChar == '\'' && rightChar == '\'');
}

char PythonEditor::getCharAt(int pos) const
{
	return static_cast<char>(SendScintilla(SCI_GETCHARAT, pos));
}

