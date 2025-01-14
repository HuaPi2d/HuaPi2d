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
    // �ı������ڴ�С�ź�
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

    // ���ݿ��б�����ݱ����
    void on_questionListWidget_itemClicked(QListWidgetItem *item);

    // �������ݿ�
    void connectDatabase();

private:
    Ui::RandomLearningWidget *ui;

    void closeEvent(QCloseEvent *event);

    KnowledgeItemsDataBase *m_knowledgeItemsDataBase;
    QList<KnowledgeItem> currentKnowledgeItemList;
    int currentListIndex;
    KnowledgeItem currentKnowledgeItem;

    // ���浱ǰ����
    void saveSetttings();

    // ��������
    void loadSettings();

    // ���½��漰����
    void updateWidget();

    // �������ݿ��������б�
    void loadDatabaseToList();
};

#endif // RANDOMLEARNINGWIDGET_H
