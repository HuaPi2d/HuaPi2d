#ifndef HELPWIDGET_H
#define HELPWIDGET_H

#include <QWidget>
#include <QHelpEngine>
#include <QHelpEngineCore>
#include <QHelpContentWidget>
#include <QHelpSearchEngine>
#include <QHelpSearchQueryWidget>
#include <QListWidget>
#include <QFile>
#include <QFileInfo>
#include <QTextBrowser>
#include <QProxyStyle>
#include <QTextDocument>
#include <QKeyEvent>
#include <QStatusBar>

#include "universal/filepro.h"
#include "universal/stringConvert.h"
#include "global/mainglobalvar.h"

namespace Ui {
    class HelpWidget;
}

class HelpWidget : public QWidget
{
    Q_OBJECT

public:
    explicit HelpWidget(QWidget* parent = nullptr);
    ~HelpWidget();

    bool setupHelpSystem(const QString& qhcFilePath);

    void exportQHCFile(const QString& sourceFilePath);

    void searchActivated();

    void hideItems();

private:
    Ui::HelpWidget* ui;
    QString m_qhcFilePath;
    QHelpEngine* m_helpEngine;
    QHelpEngineCore* m_helpEngineCore;
    QHelpSearchEngine* m_searchEngine;
    QListWidget* m_searchResultsPopup; // 浮动结果框
    QStatusBar* m_statusBar; // 状态栏

    // 事件过滤器
    bool eventFilter(QObject* obj, QEvent* event);
    void mousePressEvent(QMouseEvent* event);

    // 搜索结果栏
    void showSearchResultsPopup();
    void hideSearchResultsPopup();
    void updateSearchResultsPopup(const QList<QHelpSearchResult>& results);

    // 添加窗口部件
    void addStatusBar();

    void indexingStarted();
    void indexingFinished();
};

#endif // HELPWIDGET_H