#ifndef ZXLEVELS_H
#define ZXLEVELS_H

#include <QString>
#include <QList>

struct DifficultyMode {
    QString name;
    int value;
};


struct ZXLevel {
    QString name;
    QList<DifficultyMode> modes;
};


struct ZXChapter {
    QString name;
    QList<ZXLevel> levels;
};


class ZXGameData {
public:
    static const QList<DifficultyMode>& getDifficultyModes(int type);

    static const QList<ZXChapter>& getChapters();

    static const ZXChapter& getChapterByName(QString name);

    static const QList<DifficultyMode>& getDifficultyByLevelName(QString name);

private:
    static QList<ZXChapter> initializeChapters();

    static ZXChapter createChapter(const QString& name, const QList<QString>& levelNames);
};

#endif // ZXLEVELS_H
