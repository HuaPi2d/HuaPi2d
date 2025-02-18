#ifndef FILEOFDIFFERENTPATH_H
#define FILEOFDIFFERENTPATH_H

#include <QFile>
#include <QFileInfo>
#include <QDir>
#include <QTreeWidgetItem>
#include <QApplication>

QString tips_file_path(QString file_path);

void check_file_path(QString file_path);
void check_file_path(QFileInfo file_info);

void populateTree(const QString dir, QTreeWidgetItem* parent);
QString getAbsolutePath(QString& file_path);
QString getRelativePath(const QString& file_path);

#endif