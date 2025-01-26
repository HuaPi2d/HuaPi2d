#include "userssettings.h"


UsersSettings::UsersSettings(const QString& fileName, QSettings::Format format, QObject* parent)
    : QSettings(fileName, format, parent), settingsFileInfo(fileName)
{
	// 查看保存路径是否存在，不存在则创建
	if (!settingsFileInfo.absoluteDir().exists())
		settingsFileInfo.absoluteDir().mkpath(".");
}

UsersSettings::~UsersSettings()
{
}
