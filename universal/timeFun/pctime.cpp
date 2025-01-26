#include "pctime.h"

QString getYYYYMMDDHHMMSS()
{
    QDateTime currentDateTime = QDateTime::currentDateTime();
    return currentDateTime.toString("yyyy-MM-dd hh:mm:ss");
}
