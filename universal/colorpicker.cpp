#include "colorpicker.h"

ColorPicker::ColorPicker(QWidget *parent)
	: QWidget(parent)
{
    // 初始化界面元素和布局
    this->setWindowTitle("取色器");
    this->resize(300, 200);

    colorLabel = new QLabel(this);
    colorLabel->setAlignment(Qt::AlignCenter);
    colorLabel->setMinimumSize(100, 50);

    rgbValue = new QLabel("RGB: ", this);
    hexValue = new QLabel("HEX: ", this);

    QPushButton* selectBtn = new QPushButton("选择颜色", this);
    connect(selectBtn, &QPushButton::clicked, this, &ColorPicker::showColorDialog);

    QGridLayout* layout = new QGridLayout(this);
    layout->addWidget(colorLabel, 0, 0, 1, 2);
    layout->addWidget(rgbValue, 1, 0);
    layout->addWidget(hexValue, 2, 0);
    layout->addWidget(selectBtn, 3, 0);

    timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, &ColorPicker::updateColor);
    timer->start(50); // 每50ms更新一次颜色

    updateColor();
}

ColorPicker::~ColorPicker()
{}

void ColorPicker::showColorDialog()
{
    QColor initialColor = color;
    QColorDialog::getColor(initialColor, this, "选择颜色");
    if (initialColor.isValid()) {
        color = initialColor;
        updateDisplay();
    }
}

void ColorPicker::updateDisplay()
{
    colorLabel->setStyleSheet(QString("background-color: %1").arg(color.name()));
    rgbValue->setText(QString("RGB: (%1, %2, %3)").arg(color.red()).arg(color.green()).arg(color.blue()));
    hexValue->setText(QString("HEX: %1").arg(color.name().toUpper()));
}

void ColorPicker::updateColor()
{
    QPoint cursorPos = QCursor::pos();
    HWND hwnd = FindWindow(nullptr, nullptr); // 获取顶层窗口
    HDC hdc = GetDC(hwnd); // 获取设备上下文
    if (hwnd) {
        color = QColor(GetPixel(hdc, cursorPos.x(), cursorPos.y()));
    }
    ReleaseDC(hwnd, hdc); // 释放设备上下文
    updateDisplay();
}
