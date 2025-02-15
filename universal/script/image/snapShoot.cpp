#include "snapShoot.h"

QPixmap captureFullScreen() {
    // 获取当前屏幕
    QScreen *screen = QGuiApplication::primaryScreen();
    if (!screen)
        return QPixmap();

    // 截图
    QPixmap pixmap = screen->grabWindow(0);

    return pixmap;
}

cv::Point getScreenResolution() {
    QScreen* screen = QGuiApplication::primaryScreen();
    cv::Mat screenImage = QImageToMat(screen->grabWindow(0).toImage());
    if (screenImage.empty() == false) {
        return cv::Point(screenImage.cols, screenImage.rows);
    }
    else {
        return cv::Point();
    }
}

float getScreenScale()
{
    // 获取系统DPI
    float dpi = static_cast<float>(GetDpiForSystem());
    // 计算缩放比
    float scale = dpi / 96.0f;
    return scale;
}
