#include "stringcheck.h"

bool isOnlyWhitespaceOrNewline(const QString& text) {
    // ƥ����ͨ�ı��Ŀո�ͻ���
    QRegularExpression rePlain(R"(^[\s]*$)");

    // ƥ�� HTML ��ʽ�� <br> ��ǩ�Ϳո�
    QRegularExpression reHtml(R"(^([\s]|<br\s*/?>)*$)");

    return rePlain.match(text).hasMatch() || reHtml.match(text).hasMatch();
}
