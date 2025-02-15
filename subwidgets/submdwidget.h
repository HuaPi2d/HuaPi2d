#pragma once

#include <QWidget>
#include <QString>
#include <QStringList>
#include <QVBoxLayout>

#include "ui_submdwidget.h"
#include "universal/file/userssettings.h"
#include "ssjjCore/script/scripteditor/mdfileseditor.h"

QT_BEGIN_NAMESPACE
namespace Ui { class SubMDWidgetClass; };
QT_END_NAMESPACE

class SubMDWidget : public QWidget
{
	Q_OBJECT

public:
	SubMDWidget(QWidget *parent = nullptr);
	~SubMDWidget();

private:
	Ui::SubMDWidgetClass *ui;

	QString currentFile;
	QStringList recentFiles;
	MDFilesEditor *mdFilesEditor;
	EditorConfig mdEditorConfig;

private:
	void loadSettings();
	void saveSettings();

public: signals:
	void sendStateInfo(QString state);
	void widgetClosed();
};
