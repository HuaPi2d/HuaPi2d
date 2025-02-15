#pragma once

#include <QObject>

#include "struct/editor/EditorConfig.h"


class LanguageEditorSignalsHelper  : public QObject
{
	Q_OBJECT

public:
	LanguageEditorSignalsHelper(QObject *parent);
	~LanguageEditorSignalsHelper();

	EditorConfig m_editorConfig;

public: signals:
	void resetStyles(EditorConfig config);
};
