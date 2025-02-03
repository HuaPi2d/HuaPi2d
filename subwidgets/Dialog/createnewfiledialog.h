#ifndef CREATENEWFILEDIALOG_H
#define CREATENEWFILEDIALOG_H

#include <QDialog>
#include <QString>
#include <QList>
#include <QDir>
#include <QComboBox>
#include <QModelIndex>
#include <QMessageBox>
#include <QStringList>

#include "ui_createnewfiledialog.h"
#include "ssjjCore/zx/zxlevels.h"
#include "universal/file/fileattributes.h"
#include "global/mainglobalvar.h"

namespace Ui { class CreateNewFileDialogClass; };

typedef struct FileType
{
	QString description;
	QString suffix;
	QList<FileType> childTypes;
} FileType;

extern FileType scriptalFileType;
extern FileType emptyScriptalFileType;
extern FileType luanDouScriptalFileType;
extern FileType zhuXianScriptalFileType;

#define SCP scriptalFileType
#define ESCP emptyScriptalFileType
#define LSCP luanDouScriptalFileType
#define ZSCP zhuXianScriptalFileType

class CreateNewFileDialog : public QDialog
{
	Q_OBJECT

public:
	CreateNewFileDialog(QWidget* parent, QList<FileType> fileTypes, QString defaultSavePath = "scriptal/");
	~CreateNewFileDialog();

	QString getFileName();
	QString getSavePath();
	QList<FileAttribute> getFileAttribute();

private:
	Ui::CreateNewFileDialogClass *ui;

	FileType selectedFileType;
	FileType selectedChildFileType;
	QList<ZXLevel> zxLevels;
	ZXLevel zxLevel;
	QString parentDir;
};


#endif // CREATENEWFILEDIALOG_H