#include "zxlevels.h"

const QList<DifficultyMode>& ZXGameData::getDifficultyModes(int type)
{
    if (type == 0)
    {
        static QList<DifficultyMode> modes = {
            {"挑战", 1}, {"普通", 2}, {"困难", 3}, {"噩梦", 4},
            {"专家一", 5}, {"专家二", 6}, {"专家三", 7}, {"专家四", 8},
            {"专家五", 9}, {"专家六", 10}, {"极速", 11}
        };
        return modes;
    }
    else if (type == 1)
    {
        static QList<DifficultyMode> modes = {
            {"挑战", 1}, {"普通", 2}, {"困难", 3}, {"噩梦", 4},
            {"专家一", 5}, {"专家二", 6}, {"专家三", 7}, {"专家四", 8},
            {"专家五", 9}, {"专家六", 10}
        };
        return modes;
    }
    else{
        static QList<DifficultyMode> modes = {
            {"无", 1}
        };
        return modes;
    }
}

const QList<ZXChapter>& ZXGameData::getChapters()
{
    static QList<ZXChapter> chapters = initializeChapters();
    return chapters;
}

const ZXChapter& ZXGameData::getChapterByName(QString name)
{
    const auto& chapters = getChapters();
    for (const ZXChapter& chapter : chapters) {
        if (chapter.name == name) {
            return chapter;
        }
    }
    return ZXChapter();
}

const QList<DifficultyMode>& ZXGameData::getDifficultyByLevelName(QString name)
{
    static QList<ZXChapter> chapters = initializeChapters();
    for (const ZXChapter& chapter : chapters) {
        for (const ZXLevel& level : chapter.levels) {
            if (level.name == name) {
                return level.modes;
            }
        }
    }
    return getDifficultyModes(-1);
}

QList<ZXChapter> ZXGameData::initializeChapters()
{
    return {
        createChapter("工业区", {"工业一", "工业二", "工业三", "工业四", "工业五",
                              "工业六", "工业七", "工业八", "工业九", "工业十", "双子大厦"}),
        createChapter("城市突袭", {"城市一", "城市二", "城市三", "城市四", "城市五",
                              "城市六", "城市七", "城市八", "城市九", "城市十", "星界工厂"}),
        createChapter("玛雅迷踪", {"玛雅一", "玛雅二", "玛雅三", "玛雅四", "玛雅五",
                              "玛雅六", "玛雅七", "玛雅八", "玛雅九", "玛雅十", "玛雅金字塔"}),
        createChapter("沙漠奇兵", {"沙漠一", "沙漠二", "沙漠三", "沙漠四", "沙漠五",
                              "沙漠六", "沙漠七", "沙漠八", "沙漠九", "沙漠十", "哈苏神庙"}),
        createChapter("地狱之城", {"地狱一", "地狱二", "地狱三", "地狱四", "地狱五",
                              "地狱六", "地狱七", "地狱八", "地狱九", "地狱十", "匿名者行动基地"}),
        createChapter("海岛魅影", {"海岛一", "海岛二", "海岛三", "海岛四", "海岛五", "海岛六", 
                              "海岛七", "海岛八", "海岛九", "海岛十", "海岛十一", "海岛十二", "F.N.R城堡废墟"}),
        createChapter("勇者试炼", {"暴乱监狱", "堕落废墟", "边境要塞", "失落祭坛", "堕落深渊", "绝望之谷",
            "遗迹哨站", "峡谷基地", "天使计划", "隧道恶魔", "失控试验", "地下惊魂"}),
        createChapter("禁区", {"丛林遗迹", "泰坦基地", "泰坦试验场", "阿布辛贝", "寄生巢穴（一）",
            "寄生巢穴（二）", "绝命回收"})
    };
}

ZXChapter ZXGameData::createChapter(const QString& name, const QList<QString>& levelNames)
{
    ZXChapter chapter;
    chapter.name = name;
    // 根据章节名称确定难度模式
    if (chapter.name == "工业区" || chapter.name == "城市突袭" || chapter.name == "玛雅迷踪" || 
        chapter.name == "沙漠奇兵" || chapter.name == "地狱之城")
    {
        const auto& modes = getDifficultyModes(0);
        for (const QString& levelName : levelNames) {
            chapter.levels.append({ levelName, modes });
        }
    }
    else if (chapter.name == "海岛魅影" || chapter.name == "勇者试炼" || chapter.name == "禁区")
    {
        const auto& modes = getDifficultyModes(1);
        for (const QString& levelName : levelNames) {
            chapter.levels.append({ levelName, modes });
        }
    }
    return chapter;
}
