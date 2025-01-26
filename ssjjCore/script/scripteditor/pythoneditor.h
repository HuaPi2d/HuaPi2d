#pragma once

#include <QWidget>
#include <Qsci/qsciscintilla.h>
#include <Qsci/qscilexerpython.h>
#include <Qsci/qsciapis.h>
#include <QColor>
#include <QDir>
#include <QKeyEvent>

class PythonEditor  : public QsciScintilla
{
	/* Q_OBJECT */

public:
	PythonEditor(QWidget *parent);
	~PythonEditor();

protected:
	void keyPressEvent(QKeyEvent* event) override;

private:
	QsciLexerPython *textLexer;
	QsciAPIs *textAPIs;

	void insertMatchingCharacter(const QString& openChar, const QString& closeChar);
	bool deleteAction();
	void deleteMatchingCharacter(int pos);
	bool isMatchingCharacter(const char& leftChar, const char& rightChar);
	char getCharAt(int pos) const;
};
