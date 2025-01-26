// userssettings.h
#ifndef USERSSETTINGS_H
#define USERSSETTINGS_H

#include <QSettings>
#include <QFileInfo>
#include <QDir>

class UsersSettings : public QSettings
{
    Q_OBJECT

public:
    UsersSettings(const QString& fileName, QSettings::Format format = QSettings::IniFormat, QObject* parent = nullptr);
    ~UsersSettings();

private:
    QFileInfo settingsFileInfo;
};

#endif // USERSSETTINGS_H

