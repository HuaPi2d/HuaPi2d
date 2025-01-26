#ifndef SUBVIDEOWIDGET_H
#define SUBVIDEOWIDGET_H

#include "ui_subvideowidget.h"
#include "info/videoinfolistmodel.h"
#include <QWidget>
#include <QQuickWidget>
#include <QQuickItem>
#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonObject>
#include <QQmlContext>
#include <QDateTime>
#include <QQmlEngine>
#include <QtAdvancedStylesheet.h>
#include <Qsci/QsciScintilla.h>
#include <QmlStyleUrlInterceptor.h>

#include "universal/file/userssettings.h"

namespace Ui {
class SubVideoWidget;
}

class SubVideoWidget : public QWidget
{
    Q_OBJECT

protected:
    void closeEvent(QCloseEvent *event) override;

public:
    explicit SubVideoWidget(QWidget *parent = nullptr);
    ~SubVideoWidget();

    void saveSettings();
    void loadSettings();
    void searchVideo();
    void searchByBiliBili();
    void searchByMoGu();
    void setQMLPath();
    void updateQuickWidgetTheme(acss::QtAdvancedStylesheet* advancedStyleSheet);

public slots:
    void scrollToBottom();

private:
    Ui::SubVideoWidget *ui;
    int currentPage;
    VideoInfoListModel *videoInfoListModel;
    QString currentWeb;
    QString currentSearchContent;
    QQuickItem *rootObject;

public: signals:
    void widgetClosed();
    void sendStateInfo(QString state);
};

#endif // SUBVIDEOWIDGET_H
