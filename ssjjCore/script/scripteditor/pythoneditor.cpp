#include "pythoneditor.h"

PythonEditor::PythonEditor(QWidget *parent)
	: QsciScintilla(parent)
{
	// ���ôʷ�������
	textLexer = new QsciLexerPython(this);
	textLexer->setDefaultColor(QColor(0, 0, 0));
	this->setLexer(textLexer);
	// ��ʾ�к�
	this->setMarginType(0, QsciScintilla::NumberMargin);
	this->setMarginLineNumbers(0, true);
	this->setMarginWidth(0, 30);
	// ������ʾ
	textAPIs = new QsciAPIs(textLexer);
	textAPIs->load("api/Python-3.11.api");
	textAPIs->prepare();

	this->setAutoCompletionSource(QsciScintilla::AcsAll);   // ����Դ���Զ���ȫ���еط����ֵ�
	this->setAutoCompletionThreshold(1);  // �����Զ���ȫ�Ĵ����ַ���
	this->setAutoCompletionCaseSensitivity(true);  // �����Զ���ȫ�Ĵ�Сд����
	this->setAutoIndent(true);             // �����Զ�����
	this->setBraceMatching(QsciScintilla::SloppyBraceMatch);  // ��������ƥ��
	
}

PythonEditor::~PythonEditor()
{}

// ��д��������¼�
// ʵ�����ţ�����ƥ�����빦��
// ʵ�����ţ����ŵ�����ɾ������
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
		case Qt::Key_Backspace: // ɾ������
			isOprate = deleteAction();
			if (isOprate == true)
				return;
			else
				break;
			
		default:
			break;
	}
	// ����������������ø���ķ���
	QsciScintilla::keyPressEvent(event);
}

void PythonEditor::insertMatchingCharacter(const QString& openChar, const QString& closeChar)
{
	// ��ȡ��ǰ�Ĺ��λ��
	int line, index;
	this->getCursorPosition(&line, &index);

	// ��������
	this->insert(openChar + closeChar);

	// ��������������м�
	this->setCursorPosition(line, index + 1);
}

bool PythonEditor::deleteAction()
{
	// ��ȡ��ǰ�Ĺ��λ��
	int pos = static_cast<int>(SendScintilla(SCI_GETCURRENTPOS));

	// ��ȡ������ҵķ���
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

