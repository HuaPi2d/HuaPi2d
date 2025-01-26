#include "stringcheck.h"

bool isOnlyWhitespaceOrNewline(const QString& text) {
    // 匹配普通文本的空格和换行
    QRegularExpression rePlain(R"(^[\s]*$)");

    // 匹配 HTML 格式的 <br> 标签和空格
    QRegularExpression reHtml(R"(^([\s]|<br\s*/?>)*$)");

    return rePlain.match(text).hasMatch() || reHtml.match(text).hasMatch();
}
