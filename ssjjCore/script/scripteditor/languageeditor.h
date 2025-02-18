#pragma once

#include <QWidget>
#include <Qsci/qsciscintilla.h>
#include <Qsci/qsciapis.h>
#include <QColor>
#include <QDir>
#include <QKeyEvent>
#include <QList>
#include <QSettings>
#include <QMessageBox>
#include <QRegularExpression>

#include "universal/file/fileattributes.h"
#include "languageeditorsignalshelper.h"


class LanguageEditor  : public QsciScintilla
{
	/*Q_OBJECT*/

public:
	LanguageEditor(QWidget *parent);
	~LanguageEditor();

	LanguageEditorSignalsHelper* m_signalsHelper;

protected:
	void keyPressEvent(QKeyEvent* event) override;

private:
	QString m_fileName;
	QString m_savePath;
	QFile* m_file;
	QFileInfo m_fileInfo;
	QList<FileAttribute> fileAttributes;
	QMap<QString, QString> fileAttributeMap;
	
	void insertMatchingCharacter(const QString& openChar, const QString& closeChar);
	bool deleteAction();
	void deleteMatchingCharacter(int pos);
	bool isMatchingCharacter(const char& leftChar, const char& rightChar);
	void readFromFile();
	void createFile();

public slots:
	virtual void receiveFileInfo(const QString& fileName, const QString& savePath, const QMap<QString, QString>& attributes);
	virtual void receiveFileInfo(const QString& fileName, const QString& savePath);
	virtual void receiveFileInfo(const QString& resourcesFilePath);
	void setFileAttributes(QList<FileAttribute> attributes);
	void setFileAttributesMap(const QMap<QString, QString>& attributes);
	void setFileAttribute(const QString& key, const QString& value);
	QString getFileName() const;
	QString getSavePath() const;
	QFileInfo getFileInfo() const;
	bool renameFile(const QString& newFileName);
	void saveFile();
	QMap<QString, QString> getFileAttributesMap();
	void resetStyles();

public:
	char getCharAt(int pos) const;
	int getCurrentPos();
};
