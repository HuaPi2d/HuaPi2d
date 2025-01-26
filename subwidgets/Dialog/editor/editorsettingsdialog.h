#pragma once

#include <QDialog>
#include <QSplitter>
#include <QVBoxLayout>
#include <QToolBox>
#include <QList>
#include <QWidget>
#include <QFontDatabase>
#include <QFont>
#include <QIntValidator>

#include "ui_editorsettingsdialog.h"
#include "struct/editor/EditorConfig.h"
#include "subwidgets/Dialog/color/colorpickerdialog.h"


QT_BEGIN_NAMESPACE
namespace Ui { class EditorSettingsDialogClass; };
QT_END_NAMESPACE

class EditorSettingsDialog : public QDialog
{
	Q_OBJECT

public:
	EditorSettingsDialog(EditorConfig editorConfig, QWidget *parent = nullptr);
	~EditorSettingsDialog();

public: signals:
	void applySettings(EditorConfig editorConfig);

public slots:
	EditorConfig getEditorConfig();

private:
	Ui::EditorSettingsDialogClass *ui;

	QList<QWidget*> m_widgets;
	EditorConfig m_editorConfig;

	void updateInterface(int index);
	void loadCurrentEditorConfig();
};
