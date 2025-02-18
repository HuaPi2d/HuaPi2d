#pragma once

#include <QDialog>
#include <QComboBox>
#include <QMap>

#include "ui_modifyfileattributesdialog.h"
#include "ssjjCore/zx/zxlevels.h"
#include "universal/file/fileattributes.h"

QT_BEGIN_NAMESPACE
namespace Ui { class ModifyFileAttributesDialogClass; };
QT_END_NAMESPACE

class ModifyFileAttributesDialog : public QDialog
{
	Q_OBJECT

public:
	ModifyFileAttributesDialog(QWidget *parent = nullptr);
	~ModifyFileAttributesDialog();

public slots:
	void setFileName(const QString& fileName);
	void setFilePath(const QString& fileInfo);

	QString getFileName() const;
	QMap<QString, QString> getFileAttributesMap();

private:
	Ui::ModifyFileAttributesDialogClass *ui;

	QList<ZXLevel> zxLevels;
	ZXLevel zxLevel;
	QString level;
	QString chapter;
	QString difficulty;
	QMap<QString, QString> fileAttributesMap;
};
