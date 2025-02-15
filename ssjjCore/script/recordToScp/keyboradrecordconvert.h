#pragma once

#include <QRegularExpression>
#include <QString>
#include <QMap>

typedef struct KeyEvent {
	QString type;
} KeyEvent;

QString convertRecordToScp(QString record);
