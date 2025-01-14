#include "languageeditor.h"

LanguageEditor::LanguageEditor(QWidget *parent)
	: QsciScintilla(parent)
{
	fileAttributes = QList<FileAttribute>();

	// 显示行号
	this->setMarginType(0, QsciScintilla::NumberMargin);
	this->setMarginLineNumbers(0, true);
	this->setMarginWidth(0, 30);

	// 优化设置
	this->setAutoCompletionSource(QsciScintilla::AcsAll);   // 设置源，自动补全所有地方出现的
	this->setAutoCompletionThreshold(1);  // 设置自动补全的触发字符数
	this->setAutoCompletionCaseSensitivity(true);  // 设置自动补全的大小写敏感
	this->setAutoIndent(true);             // 设置自动缩进
	this->setBraceMatching(QsciScintilla::SloppyBraceMatch);  // 设置括号匹配
	//this->setTabWidth(4);           // 设置 tab 宽度为四个空格
}

LanguageEditor::~LanguageEditor()
{
	// 保存文件
	if(m_fileName.isEmpty() || m_savePath.isEmpty())
		return;
	saveFile();
	delete m_file;
}

void LanguageEditor::keyPressEvent(QKeyEvent * event)
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
	case Qt::Key_Less:        // '<'
		insertMatchingCharacter("<", ">");
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

void LanguageEditor::insertMatchingCharacter(const QString & openChar, const QString & closeChar)
{
	// 获取当前的光标位置
	int line, index;
	this->getCursorPosition(&line, &index);

	// 插入括号
	this->insert(openChar + closeChar);

	// 将光标移至括号中间
	this->setCursorPosition(line, index + 1);
}

bool LanguageEditor::deleteAction()
{
	// 获取当前的光标位置
	int pos = getCurrentPos();

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

void LanguageEditor::deleteMatchingCharacter(int pos)
{
	SendScintilla(SCI_DELETERANGE, pos - 1, 2);
	return;
}

bool LanguageEditor::isMatchingCharacter(const char& leftChar, const char& rightChar)
{
	return (leftChar == '(' && rightChar == ')') ||
		(leftChar == '[' && rightChar == ']') ||
		(leftChar == '{' && rightChar == '}') ||
		(leftChar == '\"' && rightChar == '\"') ||
		(leftChar == '\'' && rightChar == '\'') ||
		(leftChar == '<' && rightChar == '>');
}

void LanguageEditor::readFromFile()
{
	if (m_file->open(QIODevice::ReadOnly | QIODevice::Text))
	{
		QTextStream in(m_file);
		this->setText(in.readAll());
		m_file->close();
	}
}

void LanguageEditor::createFile()
{
	// 检查是否存在文件存储路径，不存在则创建
	if (!QDir(m_savePath).exists())
	{
		QDir().mkpath(m_savePath);
	}

	// 创建文件
	m_file = new QFile(m_savePath + "/" + m_fileName);
	if (m_file->open(QIODevice::WriteOnly | QIODevice::Text))
	{
		m_file->close();
	}
}

void LanguageEditor::saveFile()
{
	// 检查是否存在文件存储路径，不存在则创建
	if (!QDir(m_savePath).exists())
	{
		QDir().mkpath(m_savePath);
	}

	// 保存文件
	if (m_file->open(QIODevice::WriteOnly | QIODevice::Text))
	{
		QTextStream out(m_file);
		out << this->text();
		m_file->close();
	}

	// 写入文件属性
	writeFileAttributes(m_fileInfo.absoluteFilePath(), fileAttributes);
}

void LanguageEditor::receiveFileAttributes(QList<FileAttribute> attributes)
{
	fileAttributes = attributes;
}

QList<FileAttribute> LanguageEditor::getFileAttributes()
{
	return fileAttributes;
}

char LanguageEditor::getCharAt(int pos) const
{
	return static_cast<char>(SendScintilla(SCI_GETCHARAT, pos));
}

int LanguageEditor::getCurrentPos()
{
	return static_cast<int>(SendScintilla(SCI_GETCURRENTPOS));
}

void LanguageEditor::receiveFileInfo(const QString& fileName, const QString& savePath, QList<FileAttribute> attributes)
{
	m_fileName = fileName;
	m_savePath = savePath;
	m_file = new QFile(m_savePath + "/" + m_fileName);
	m_fileInfo = QFileInfo(m_savePath + "/" + m_fileName);

	// 检测文件是否存在，不存在则创建，存在则读取内容
	if (m_file->exists())
	{
		readFromFile();
	}
	else
	{
		createFile();
	}

	fileAttributes = attributes;
}

QString LanguageEditor::getFileName() const
{
	return m_fileName;
}

QString LanguageEditor::getSavePath() const
{
	return m_savePath;
}

QFileInfo LanguageEditor::getFileInfo() const
{
	return QFileInfo(m_savePath + "/" + m_fileName);
}


