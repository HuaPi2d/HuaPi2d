#include "deletemusicdialog.h"
#include "ui_deletemusicdialog.h"

#include <QPushButton>

DeleteMusicDialog::DeleteMusicDialog(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::DeleteMusicDialog)
{
    ui->setupUi(this);
    this->setWindowTitle(tr("删除本地歌曲"));
    ui->buttonBox->button(QDialogButtonBox::Ok)->setText(tr("确定"));
    ui->buttonBox->button(QDialogButtonBox::Cancel)->setText(tr("取消"));

    // 更新语言
    connect(Language, &GlobalVariableQString::valueChanged, this, [=]() {
        ui->retranslateUi(this);
        });
    reloadLanguage(Language->value());
}

DeleteMusicDialog::~DeleteMusicDialog()
{
    delete ui;
}
