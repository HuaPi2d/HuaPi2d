#include "randomlearningwidget.h"
#include "ui_randomlearningwidget.h"

RandomLearningWidget::RandomLearningWidget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::RandomLearningWidget)
{
    ui->setupUi(this);

    // 设置小数位数为 0，只能输入整数
    ui->doubleSpinBox->setDecimals(0);

    // 设置最小值和最大值
    ui->doubleSpinBox->setMinimum(10);  // 设置下限为 1
    ui->doubleSpinBox->setMaximum(30); // 设置上限为 100

    // 加载配置
    loadSettings();

    // 更新界面
    updateWidget();

    // 加载数据库至列表
    loadDatabaseToList();
}

RandomLearningWidget::~RandomLearningWidget()
{
    // 保存配置
    saveSetttings();
    delete ui;
}

void RandomLearningWidget::closeEvent(QCloseEvent *event)
{
    emit widgetClosed();
}

void RandomLearningWidget::saveSetttings()
{
    /* 声明对象 */
    QSettings settings(qApp->applicationDirPath() + "/Settings/randomLearningSettings.ini", QSettings::IniFormat);

    /* 写入配置 */
    settings.beginGroup("randomLearning");
    settings.setValue("autoSave", ui->autoSaveCheckBox->isChecked());
    settings.setValue("viewDataList", ui->viewDataListCheckBox->isChecked());
    settings.setValue("readOnly", ui->readonlyCheckBox->isChecked());
    settings.setValue("viewKeyWords", ui->viewKeyWordsCheckBox->isChecked());
    settings.setValue("highlight", ui->highlightCheckBox->isChecked());
    settings.endGroup();
}

void RandomLearningWidget::loadSettings()
{
    /* 声明对象 */
    QSettings settings(qApp->applicationDirPath() + "/Settings/randomLearningSettings.ini", QSettings::IniFormat);

    /* 读取配置 */
    settings.beginGroup("randomLearning");
    // 自动保存
    ui->autoSaveCheckBox->setChecked(settings.value("autoSave", true).toBool());
    // 显示数据列表
    ui->viewDataListCheckBox->setChecked(settings.value("viewDataList", false).toBool());
    // 只读
    ui->readonlyCheckBox->setChecked(settings.value("readOnly", false).toBool());
    // 显示关键词
    ui->viewKeyWordsCheckBox->setChecked(settings.value("viewKeyWords", false).toBool());
    // 高亮
    ui->highlightCheckBox->setChecked(settings.value("highlight", false).toBool());
    settings.endGroup();
}

void RandomLearningWidget::updateWidget()
{
    if (ui->viewDataListCheckBox->isChecked() == true)
    {
        if (ui->questionListHorizontalWidget->isHidden() == true)
        {
            // 设置窗口大小
            emit this->sendDsizeInfo(ui->questionListHorizontalWidget->maximumWidth(), 0);
            // 显示数据列表
            ui->questionListHorizontalWidget->show();
        }
    }
    else
    {
        if (ui->questionListHorizontalWidget->isHidden() == false)
        {
            // 设置窗口大小
            emit this->sendDsizeInfo(-ui->questionListHorizontalWidget->maximumWidth(), 0);
            // 隐藏数据列表
            ui->questionListHorizontalWidget->hide();
        }
    }
}

void RandomLearningWidget::loadDatabaseToList()
{
    // 连接数据库
    connectDatabase();

    // 获取数据
    currentKnowledgeItemList = m_knowledgeItemsDataBase->getKnowledgeItems();

    // 创建 KnowledgeItemWidget 对象，并添加至列表
    int index = 0;
    ui->questionListWidget->clear();
    for (KnowledgeItem item : currentKnowledgeItemList)
    {
        // 创建 QListWidgetItem 对象
        QListWidgetItem* listWidgetItem = new QListWidgetItem(ui->questionListWidget);

        // 创建 LisItemWidget 对象
        KnowledgeItemWidget* itemWidget = new KnowledgeItemWidget(ui->questionListWidget, index++, item.question, item.answer);
        listWidgetItem->setSizeHint(QSize(ui->questionListWidget->width(), itemWidget->height()));

        // 添加条目至列表
        ui->questionListWidget->addItem(listWidgetItem);
        listWidgetItem->setData(Qt::UserRole, index);
        ui->questionListWidget->setItemWidget(listWidgetItem, itemWidget);
    }
}

void RandomLearningWidget::on_closePushButton_clicked()
{
    this->close();
}


void RandomLearningWidget::on_openSourceFirstPushButton_clicked()
{

}


void RandomLearningWidget::on_fontComboBox_currentFontChanged(const QFont &f)
{
    /*QFont font = f;
    font.setPointSize(ui->doubleSpinBox->value());
    ui->textEdit->setFont(font);*/
}


void RandomLearningWidget::on_doubleSpinBox_valueChanged(double arg1)
{
    /*QFont font = ui->textEdit->font();
    font.setPointSize(arg1);
    ui->textEdit->setFont(font);*/
}

void RandomLearningWidget::on_viewDataListCheckBox_clicked(bool checked)
{
    // 获取questionListHorizontalWidget窗口宽度
    int width = ui->questionListHorizontalWidget->maximumWidth();
    if (ui->viewDataListCheckBox->isChecked()) {
        // 设置窗口大小
        emit this->sendDsizeInfo(width, 0);
        // 显示数据列表
        ui->questionListHorizontalWidget->show();
    }
    else {
        // 设置窗口大小
        emit this->sendDsizeInfo(-width, 0);
        // 隐藏数据列表
        ui->questionListHorizontalWidget->hide();
    }
}

// 保存当前的知识点
void RandomLearningWidget::saveCurrentKnowledgeItem()
{
    KnowledgeItem item;
    item.type = "English Words";
    item.question = ui->questionTextEdit->toPlainText();
    item.answer = ui->answerTextEdit->toPlainText();
    item.latestReviewTime = QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss");
    item.reviewCount = QString::number(1);

    if (isOnlyWhitespaceOrNewline(item.question) == false) {
        // 保存到数据库
        m_knowledgeItemsDataBase->insertKnowledgeItem(item);
    }
    else 
    {
        sendStateInfo("问题处为空白，未保存");
    }

    // 更新数据库
    loadDatabaseToList();
}

void RandomLearningWidget::on_firstNextPushButton_clicked()
{
    // 保存当前的知识点
    saveCurrentKnowledgeItem();
}

void RandomLearningWidget::on_firstLastPushButton_clicked()
{
}

void RandomLearningWidget::on_addNewKnowledgeItemPushButton_clicked()
{
    // 保存当前的知识点
    saveCurrentKnowledgeItem();

    // 清空输入框内容
    ui->questionTextEdit->clear();
    ui->answerTextEdit->clear();
}

void RandomLearningWidget::on_questionListWidget_itemClicked(QListWidgetItem* item)
{
    qDebug() << "item clicked";
    // 获取数据 ID
    currentListIndex = item->data(Qt::UserRole).toInt();

    // 获取当前知识点
    currentKnowledgeItem = currentKnowledgeItemList.at(currentListIndex);

    // 显示当前知识点
    ui->questionTextEdit->setText(currentKnowledgeItem.question);
    ui->answerTextEdit->setText(currentKnowledgeItem.answer);
}

void RandomLearningWidget::connectDatabase()
{
    // 获取数据库对象
    m_knowledgeItemsDataBase = KnowledgeItemsDataBase::getKnowledgeItemsDataBase("knowledgeItems.db");

    // 打开数据库
    if (!m_knowledgeItemsDataBase->open()) {
        qDebug() << "数据库打开失败" << m_knowledgeItemsDataBase->lastError().text();
        return;
    }
    else
    {
        qDebug() << "数据库打开成功";
    }

    // 创建表
    m_knowledgeItemsDataBase->createKnowledgeItemsTable();

}

