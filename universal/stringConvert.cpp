#include "stringConvert.h"

QString convertTimeFormat(const QString& timeStr) {
    // 分割字符串
    QStringList timeParts = timeStr.split(":");

    if (timeParts.size() != 2) {
        // 如果格式不对，返回原始字符串
        return timeStr;
    }

    int totalMinutes = timeParts[0].toInt();
    int seconds = timeParts[1].toInt();

    // 计算小时、分钟、秒钟
    int hours = totalMinutes / 60;
    int minutes = totalMinutes % 60;

    QString formattedTime;

    // 格式化输出
    if (hours > 0) {
        formattedTime = QString::asprintf("%d:%02d:%02d", hours, minutes, seconds);
    }
    else {
        formattedTime = QString::asprintf("%02d:%02d", minutes, seconds);
    }

    return formattedTime;
}

QString processChineseQuery(const QString& query) {
    QString processed;
    for (QChar ch : query) {
        processed += ch;
        if (ch.script() == QChar::Script_Han)
            processed += " "; // 强制分词
    }
    return processed.trimmed();
}
