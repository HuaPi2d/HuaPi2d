#pragma once

#include <QWidget>
#include <Qsci/qsciscintilla.h>
#include <Qsci/qsciapis.h>
#include <QColor>
#include <QDir>
#include <QKeyEvent>
#include <QList>
#include "universal/file/fileattributes.h"


class LanguageEditor  : public QsciScintilla
{
	/*Q_OBJECT*/

public:
	LanguageEditor(QWidget *parent);
	~LanguageEditor();

protected:
	void keyPressEvent(QKeyEvent* event) override;

private:
	QString m_fileName;
	QString m_savePath;
	QFile* m_file;
	QFileInfo m_fileInfo;
	QList<FileAttribute> fileAttributes;
	
	void insertMatchingCharacter(const QString& openChar, const QString& closeChar);
	bool deleteAction();
	void deleteMatchingCharacter(int pos);
	bool isMatchingCharacter(const char& leftChar, const char& rightChar);
	void readFromFile();
	void createFile();

public slots:
	virtual void receiveFileInfo(const QString& fileName, const QString& savePath, QList<FileAttribute> attributes);
	QString getFileName() const;
	QString getSavePath() const;
	QFileInfo getFileInfo() const;
	void saveFile();
	void receiveFileAttributes(QList<FileAttribute> attributes);
	QList<FileAttribute> getFileAttributes();

public:
	char getCharAt(int pos) const;
	int getCurrentPos();
};
