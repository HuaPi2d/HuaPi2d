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
        QColor caret_line_color;
    }color;
    struct margin {
        QColor line_number_color, background_color;
    }margin;
} EditorConfig;

#endif // EDITORCONFIG_H
