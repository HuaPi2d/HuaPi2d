#ifndef EDITORCONFIG_H
#define EDITORCONFIG_H

#include <QFont>
#include <QColor>

typedef struct EditorConfig {
    struct  font {
        QFont font;
        int size;
    }font;
    struct  color {
        QColor background, foreground, text, keyword, number, string, operateur, function, variable, comment;
    }color;
} EditorConfig;

#endif // EDITORCONFIG_H
