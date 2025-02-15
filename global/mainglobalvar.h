#ifndef MAINGLOBALVAR_H
#define MAINGLOBALVAR_H

#include <QTranslator>
#include <QApplication>

#include "global/basicGlobalVar/globalvariableqstring.h"

extern bool developerMode;
extern GlobalVariableQString* Language;
extern GlobalVariableQString* themeType;

void reloadLanguage(QString language);

#endif // MAINGLOBALVAR_H