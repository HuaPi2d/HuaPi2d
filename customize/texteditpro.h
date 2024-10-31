#ifndef TEXTEDITPRO_H
#define TEXTEDITPRO_H

#include <QObject>
#include <QTextEdit>
#include <QKeyEvent>

class TextEditPro : public QTextEdit {
    Q_OBJECT

public:
    TextEditPro(QWidget *parent = nullptr) : QTextEdit(parent) {
        connect(this, &QTextEdit::textChanged, this, &TextEditPro::updatePlaceholderText);
    }

    QString currentText; // 当前文本
    QString placeholderText; // 占位符文本

protected:
    // 重载 keyPressEvent 函数，监听键盘事件
    void keyPressEvent(QKeyEvent *event) override {
        if (event->key() == Qt::Key_B && event->modifiers() & Qt::ControlModifier) {
            // 获取当前光标
            QTextCursor cursor = this->textCursor();
            if (cursor.hasSelection()) {
                // 获取当前字符格式
                QTextCharFormat format = cursor.charFormat();
                // 切换字体加粗样式
                if (format.fontWeight() == QFont::Bold) {
                    format.setFontWeight(QFont::Normal);
                } else {
                    format.setFontWeight(QFont::Bold);
                }
                // 设置选中区域的字符格式
                cursor.mergeCharFormat(format);
            }
            return; // 防止事件继续传递
        }
        // 传递其他按键事件
        QTextEdit::keyPressEvent(event);
    }

    void focusInEvent(QFocusEvent *event) override {
        // 获得焦点时，更新占位符文本
        updatePlaceholderText();
        QTextEdit::focusInEvent(event);
    }

    void focusOutEvent(QFocusEvent *event) override {
        // 失去焦点时，更新占位符文本
        updatePlaceholderText();
        QTextEdit::focusOutEvent(event);
    }

private:
    void updatePlaceholderText() 
    {
        // 若当前没有焦点且文本框内容与placholderText不同，则显示占位符文本
        if (hasFocus() == true) 
        {
            if (currentText == placeholderText) 
            {
                currentText = "";
                this->clear();
            }
        }
        else 
        {
            if (currentText != placeholderText) {
                if (this->toPlainText().isEmpty() == true) {
                    currentText = placeholderText;
                    this->setHtml(placeholderText);
                }
            }
        }
    }

public slots:
    void receivePlacholderText(QString text) {
        placeholderText = QString("<p style='color:gray;'>%1</p>").arg(text.replace("\n", "<br>"));
        emit updatePlaceholderText();
    }
};

#endif // TEXTEDITPRO_H
