#include "mainglobalvar.h"

bool developerMode = false;
GlobalVariableQString* Language = new GlobalVariableQString(nullptr);
GlobalVariableQString* themeType = new GlobalVariableQString(nullptr);

void reloadLanguage(QString language)
{
    QTranslator newTranslator;
    qApp->removeTranslator(&newTranslator);
    if (language == "zh_CN")
        newTranslator.load(":/translations/zh_CN.qm");
    else if (language == "en_US")
        newTranslator.load(":/translations/en_US.qm");
    qApp->installTranslator(&newTranslator);
    Language->setValue(language);
}
