#ifndef FILEOFDIFFERENTPATH_H
#define FILEOFDIFFERENTPATH_H

#include <QFile>
#include <QFileInfo>
#include <QDir>
#include <QTreeWidgetItem>

QString tips_file_path(QString file_path);

void check_file_path(QString file_path);
void check_file_path(QFileInfo file_info);

void populateTree(const QString dir, QTreeWidgetItem* parent);

#endif