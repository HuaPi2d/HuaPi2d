#include "languageeditorsignalshelper.h"

LanguageEditorSignalsHelper::LanguageEditorSignalsHelper(QObject *parent)
	: QObject(parent)
{
	connect(this, &LanguageEditorSignalsHelper::resetStyles, [=](EditorConfig config) {
		m_editorConfig = config;
		});
}

LanguageEditorSignalsHelper::~LanguageEditorSignalsHelper()
{}
