#include "helpwidget.h"
#include "ui_helpwidget.h"

#include <QFileInfo>
#include <QMessageBox>
#include <QVBoxLayout>
#include <QSplitter>
#include <QTextBrowser>

HelpWidget::HelpWidget(QWidget* parent) :
    QWidget(parent),
    ui(new Ui::HelpWidget),
    m_helpEngine(nullptr)
{
    ui->setupUi(this);

    m_qhcFilePath = "resources/doc/help.qhc";

    // 在构造函数中修改
    m_searchResultsPopup = new QListWidget(this);
    m_searchResultsPopup->setWindowFlags(Qt::Tool | Qt::FramelessWindowHint);
    m_searchResultsPopup->setAttribute(Qt::WA_ShowWithoutActivating);
    m_searchResultsPopup->setFocusPolicy(Qt::NoFocus);
    m_searchResultsPopup->setStyleSheet(
        "QListWidget {"
        "   border: 1px solid #dcdcdc;"
        "   padding: 1px;"
        "   outline: none;"
        "}"
        "QListWidget::item {"
        "   padding: 5px;"
        "   border-bottom: 1px solid #f0f0f0;"
        "}"
        "QListWidget::item:hover {"
        "   background: lightgray;"
        "}"
        "QScrollBar:vertical {"
        "   width: 8px;"
        "}"
    );
    m_searchResultsPopup->hide();

    // 在构造函数中添加
    connect(m_searchResultsPopup, &QListWidget::itemClicked, [this](QListWidgetItem* item) {
        if (item->data(Qt::UserRole).isValid()) {
            QUrl url = item->data(Qt::UserRole).toUrl();
            ui->textBrowser->setSource(url);
        }
        hideSearchResultsPopup();
        });

    // 添加键盘导航支持
    connect(m_searchResultsPopup, &QListWidget::activated, [this](const QModelIndex& index) {
        if (index.isValid()) {
            QUrl url = index.data(Qt::UserRole).toUrl();
            ui->textBrowser->setSource(url);
            hideSearchResultsPopup();
        }
        });


    m_searchResultsPopup->installEventFilter(this);
    ui->searchLineEdit->installEventFilter(this);

    hideItems();

    addStatusBar();
    m_statusBar->showMessage(tr("窗口初始化中..."));
}

HelpWidget::~HelpWidget()
{
    delete m_helpEngine;
    delete ui;
}

bool HelpWidget::setupHelpSystem(const QString& qhcFilePath)
{
    // 检查 qhc 文件是否存在
    if (!QFile(qhcFilePath).exists()) {
        QMessageBox::warning(this, tr("错误"), tr("帮助文件不存在: %1").arg(qhcFilePath));
        return false;
    }

    // 若是资源路径，先导出为临时文件（假设 exportQHCFile 会设置 m_qhcFilePath）
    if (qhcFilePath.startsWith(":/")) {
        exportQHCFile(qhcFilePath);  // 你应在这个函数中设置 m_qhcFilePath
    }
    else {
        m_qhcFilePath = qhcFilePath;
    }

    // 初始化帮助系统
    m_helpEngine = new QHelpEngine(m_qhcFilePath, this);

    m_helpEngine->registeredDocumentations();

    // 实现搜索功能
    m_searchEngine = m_helpEngine->searchEngine();

    connect(m_searchEngine, &QHelpSearchEngine::indexingStarted, this, &HelpWidget::indexingStarted);
    connect(m_searchEngine, &QHelpSearchEngine::indexingFinished, this, &HelpWidget::indexingFinished);

    connect(m_searchEngine, &QHelpSearchEngine::searchingFinished, this, [=](int hits) {
        qDebug() << "搜索完成，找到" << hits << "个结果";
        QList<QHelpSearchResult> results = m_searchEngine->searchResults(0, hits);
        updateSearchResultsPopup(results);
        });

    // 连接搜索按钮信号
    connect(ui->searchLineEdit, &QLineEdit::returnPressed, this, &HelpWidget::searchActivated);
    connect(ui->searchLineEdit, &QLineEdit::textChanged, this, &HelpWidget::searchActivated);

    if (!m_helpEngine->setupData()) {
        QMessageBox::warning(this, tr("错误"), tr("无法初始化帮助系统: %1").arg(m_helpEngine->error()));
        return false;
    }

    // 取出第一个命名空间
    QString ns = m_helpEngine->registeredDocumentations().value(0);
    if (ns.isEmpty()) {
        QMessageBox::warning(this, tr("错误"), tr("未找到任何帮助文档命名空间"));
        return false;
    }

    // 找到帮助文档的虚拟路径
    QList<QUrl> fileList = m_helpEngine->files(ns, QString());
    QString virtualFolder;
    if (!fileList.isEmpty()) {
        // 提取第一个路径的虚拟目录部分
        QString path = fileList.first().toString(); // 例如 "doc/index.html"
        virtualFolder = path.section('/', 3, 3); // 提取 "doc"
    }
    else {
        QMessageBox::warning(this, tr("错误"), tr("未找到任何帮助文档文件"));
        return false;
    }

    // 添加内容目录 widget 到 UI 中，并调整界面外观
    QHelpContentWidget* contentWidget = m_helpEngine->contentWidget();
    contentWidget->setModel(m_helpEngine->contentModel());
    contentWidget->setStyleSheet("border: none;");
    ui->verticalLayout_2->addWidget(contentWidget);
    this->resize(1000, 700);
    ui->splitter->setStretchFactor(0, 1);
    ui->splitter->setStretchFactor(1, 3);

    // 设置 QTextBrowser 使用 QHelpEngine 提供资源
    connect(ui->textBrowser, &QTextBrowser::sourceChanged, this, [=](const QUrl& url) {
        m_statusBar->showMessage(url.toString(), 3000);
        QByteArray data = m_helpEngine->fileData(url);
        // 同步contentWidget的当前索引
        if (!data.isEmpty()) {
            ui->textBrowser->setHtml(QString::fromUtf8(data)); 
            QModelIndex index = contentWidget->indexOf(url);
            if (index.isValid()) {
                contentWidget->setCurrentIndex(index);
            }
        }
        else {
            ui->textBrowser->setHtml("<h3>页面加载失败</h3><p>" + url.toString() + "</p>");
        }
        });

    // 单击目录项跳转页面
    connect(contentWidget, &QHelpContentWidget::clicked, this, [=](const QModelIndex& index) {
        QHelpContentItem* item = m_helpEngine->contentModel()->contentItemAt(index);
        if (item && item->url().isValid()) {
            ui->textBrowser->setSource(item->url());
        }
        });
    connect(contentWidget, &QHelpContentWidget::linkActivated, this, [=](const QUrl& link) {
        ui->textBrowser->setSource(link); 
        });

    // 加载首页
    ui->textBrowser->setSource(QUrl(QString("qthelp://%1/%2/index.html").arg(ns).arg(virtualFolder)));

    return true;
}

void HelpWidget::exportQHCFile(const QString& sourceFilePath)
{
    // 导出 qch 文件
    copyFile(sourceFilePath, m_qhcFilePath);
}

void HelpWidget::searchActivated()
{

    QString query = ui->searchLineEdit->text().trimmed();
    if (!query.isEmpty() && ui->searchLineEdit->hasFocus()) {
        showSearchResultsPopup();
        m_searchEngine->cancelSearching();
        m_searchEngine->search(processChineseQuery(query));
    }
    else {
        hideSearchResultsPopup();
    }
}

void HelpWidget::hideItems()
{
    if (developerMode == false) {
        ui->searchLineEdit->hide();
    }
}

bool HelpWidget::eventFilter(QObject* obj, QEvent* event)
{
    if (obj == ui->searchLineEdit) {
        if (event->type() == QEvent::FocusOut) {
            QFocusEvent* fe = static_cast<QFocusEvent*>(event);
            // 只有当焦点不是转移到结果框时才隐藏
            if (!m_searchResultsPopup->underMouse()) {
                hideSearchResultsPopup();
            }
        }
        else if (event->type() == QEvent::KeyPress) {
            QKeyEvent* ke = static_cast<QKeyEvent*>(event);
            if (ke->key() == Qt::Key_Down && m_searchResultsPopup->isVisible()) {
                m_searchResultsPopup->setFocus();
                return true;
            }
        }
    }
    else if (obj == m_searchResultsPopup) {
        if (event->type() == QEvent::Hide) {
            ui->searchLineEdit->setFocus();
        }
    }
    return QWidget::eventFilter(obj, event);
}

void HelpWidget::mousePressEvent(QMouseEvent* event)
{
    if (!m_searchResultsPopup->geometry().contains(event->pos()) &&
        !ui->searchLineEdit->geometry().contains(event->pos())) {
        hideSearchResultsPopup();
    }
    QWidget::mousePressEvent(event);
}

void HelpWidget::showSearchResultsPopup()
{
    if (!m_searchResultsPopup->isVisible()) {
        QPoint pos = ui->searchLineEdit->mapToGlobal(QPoint(0, ui->searchLineEdit->height()));
        m_searchResultsPopup->move(pos);
        m_searchResultsPopup->setFixedWidth(ui->searchLineEdit->width());
        m_searchResultsPopup->show();

        // 保持输入框焦点
        ui->searchLineEdit->setFocus();
    }
}

void HelpWidget::hideSearchResultsPopup()
{
    if (m_searchResultsPopup->isVisible()) {
        m_searchResultsPopup->hide();
        ui->searchLineEdit->setFocus();
    }
}

void HelpWidget::updateSearchResultsPopup(const QList<QHelpSearchResult>& results)
{
    m_searchResultsPopup->clear();

    if (results.isEmpty()) {
        m_searchResultsPopup->addItem(tr("未找到结果"));
        return;
    }

    // 限制显示数量
    int maxResults = 5;
    for (int i = 0; i < qMin(results.size(), maxResults); i++) {
        const QHelpSearchResult& result = results.at(i);
        QListWidgetItem* item = new QListWidgetItem(result.title());
        item->setData(Qt::UserRole, result.url());
        m_searchResultsPopup->addItem(item);
    }

    // 调整高度
    int rowHeight = m_searchResultsPopup->sizeHintForRow(0);
    m_searchResultsPopup->setFixedHeight(rowHeight * qMin(results.size(), maxResults) + 2);
}

void HelpWidget::addStatusBar()
{
    m_statusBar = new QStatusBar(this);
    m_statusBar->setFixedHeight(20);
    ui->verticalLayout->addWidget(m_statusBar);
    ui->verticalLayout->setContentsMargins(0, 0, 0, 0);
}

void HelpWidget::indexingStarted()
{
    m_statusBar->showMessage(tr("正在索引..."));
}

void HelpWidget::indexingFinished()
{
    m_statusBar->showMessage(tr("索引完成"), 3000);
}
