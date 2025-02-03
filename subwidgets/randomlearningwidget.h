#ifndef RANDOMLEARNINGWIDGET_H
#define RANDOMLEARNINGWIDGET_H

#include <QWidget>
#include <QFont>
#include <QDateTime>
#include <QSettings>
#include <QListWidgetItem>

#include "server/knowledgeItem/knowledgeitemsdatabase.h"
#include "universal/string/stringcheck.h"
#include "server/knowledgeItem/knowledgeItemWidget/knowledgeitemwidget.h"
#include "global/mainglobalvar.h"

namespace Ui {
class RandomLearningWidget;
}

class RandomLearningWidget : public QWidget
{
    Q_OBJECT

public:
    explicit RandomLearningWidget(QWidget *parent = nullptr);
    ~RandomLearningWidget();

public: signals:
    void widgetClosed();
    void sendStateInfo(QString state);
    // 改变主窗口大小信号
    void sendDsizeInfo(int dwidth, int dheight);

private slots:
    void on_closePushButton_clicked();

    void on_openSourceFirstPushButton_clicked();

    void on_fontComboBox_currentFontChanged(const QFont &f);

    void on_doubleSpinBox_valueChanged(double arg1);

    void on_viewDataListCheckBox_clicked(bool checked);

    void saveCurrentKnowledgeItem();

    void on_firstNextPushButton_clicked();

    void on_firstLastPushButton_clicked();

    void on_addNewKnowledgeItemPushButton_clicked();

    // 数据库列表的内容被点击
    void on_questionListWidget_itemClicked(QListWidgetItem *item);

    // 连接数据库
    void connectDatabase();

private:
    Ui::RandomLearningWidget *ui;

    void closeEvent(QCloseEvent *event);

    KnowledgeItemsDataBase *m_knowledgeItemsDataBase;
    QList<KnowledgeItem> currentKnowledgeItemList;
    int currentListIndex;
    KnowledgeItem currentKnowledgeItem;

    // 保存当前配置
    void saveSetttings();

    // 加载配置
    void loadSettings();

    // 更新界面及内容
    void updateWidget();

    // 加载数据库内容至列表
    void loadDatabaseToList();
};

#endif // RANDOMLEARNINGWIDGET_H
