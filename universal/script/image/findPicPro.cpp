#include "findPicPro.h"

cv::Mat convertToGray(const cv::Mat& inputImage) {
    cv::Mat grayImage;

    // 如果输入图像是彩色的，则进行转换
    if (inputImage.channels() == 3) {
        cv::cvtColor(inputImage, grayImage, cv::COLOR_BGR2GRAY);
    }
    // 如果已经是灰度图，则直接复制
    else if (inputImage.channels() == 1) {
        grayImage = inputImage.clone();
    } else {
        std::cerr << "Unsupported image format!" << std::endl;
    }

    return grayImage;
}

cv::Mat QImageToMat(const QImage &image) {
    cv::Mat mat;
    switch (image.format()) {
    case QImage::Format_RGB32: {
        mat = cv::Mat(image.height(), image.width(), CV_8UC4, (void*)image.constBits(), image.bytesPerLine());
        break;
    }
    case QImage::Format_RGB888: {
        mat = cv::Mat(image.height(), image.width(), CV_8UC3, (void*)image.constBits(), image.bytesPerLine());
        cv::cvtColor(mat, mat, cv::COLOR_RGB2BGR);
        break;
    }
    case QImage::Format_Indexed8: {
        mat = cv::Mat(image.height(), image.width(), CV_8UC1, (void*)image.constBits(), image.bytesPerLine());
        break;
    }
    default:
        break;
    }
    return mat.clone();
}

cv::Point templateMatch(const cv::Mat& screenImage, const cv::Mat& templateImage, double threshold) {
    cv::Mat result;
    cv::matchTemplate(templateImage, screenImage, result, cv::TM_CCOEFF_NORMED);  // 使用标准化相关系数匹配
    double minVal, maxVal;
    cv::Point minLoc, maxLoc;
    cv::minMaxLoc(result, &minVal, &maxVal, &minLoc, &maxLoc);  // 找到匹配的最大值位置

    // 判断最大相似度是否超过阈值
    if (maxVal >= threshold) {
        return cv::Point(maxLoc.x + templateImage.cols / 2, maxLoc.y + templateImage.rows / 2);  // 返回匹配度最高的点
    } else {
        return cv::Point(-1, -1);  // 如果不满足相似度阈值，返回(-1, -1)
    }
}

cv::Point findPicInFullScreen(QString picPath){
    QImage qTemplateImage(picPath);
    cv::Mat templateImage = QImageToMat(qTemplateImage);
    QPixmap screenShoot = captureFullScreen();
    cv::Mat screenImage = QImageToMat(screenShoot.toImage());
    return templateMatch(screenImage, templateImage, 0.9);
}

cv::Point findPicInFullScreen(cv::Mat& image)
{
    QPixmap screenShoot = captureFullScreen();
    cv::Mat screenImage = QImageToMat(screenShoot.toImage());
    return templateMatch(screenImage, image, 0.95);
}

cv::Point findPicInLimitedTime(QString picPath, int timeLimit, int interval) {
    QElapsedTimer timer;
    timer.start();

    cv::Point matchPoint(-1, -1);  // 默认返回值
    while (timer.elapsed() < timeLimit) {
        // 在全屏截图中寻找图片
        matchPoint = findPicInFullScreen(picPath);

        // 如果找到图片（假设findPicInFullScreen返回的坐标是有效的）
        if (matchPoint.x != -1 && matchPoint.y != -1) {
            return matchPoint;
        }

        // 等待指定的时间间隔
        QThread::msleep(interval);
    }

    // 如果超时则返回 (-1, -1)
    return cv::Point(-1, -1);
}

cv::Mat getPartOfScreen(int x_part, int y_part, int x_index, int y_index)
{
    QPixmap screenShoot = captureFullScreen();
    cv::Mat screenImage = QImageToMat(screenShoot.toImage());
    cv::Mat partImage = cutImage(x_part, y_part, x_index, y_index, screenImage);
    return partImage;
}

cv::Mat getPartsOfScreen(int x_part, int y_part, int x_index, int y_index, int x_num, int y_num)
{
    QPixmap screenShoot = captureFullScreen();
    cv::Mat screenImage = QImageToMat(screenShoot.toImage());
    cv::Mat partImage = cutImages(x_part, y_part, x_index, y_index, x_num, y_num, screenImage);
    return partImage;
}

cv::Mat cutImage(int x_part, int y_part, int x_index, int y_index, cv::Mat& image)
{
    // 裁剪图片
    // 原图大小
    int width = image.cols;
    int height = image.rows;

    // 新图片大小
    int new_width = width / x_part;
    int new_height = height / y_part;

    // 裁剪坐标
    int x_start = new_width * x_index;
    int y_start = new_height * y_index;

    // 裁剪图片
    cv::Mat new_image = image(cv::Rect(x_start, y_start, new_width, new_height));

    return new_image;
}

cv::Mat cutImages(int x_part, int y_part, int x_index, int y_index, int x_num, int y_num, cv::Mat& image)
{
    // 裁剪图片
    // 原图大小
    int width = image.cols;
    int height = image.rows;

    // 新图片大小
    int new_width = width / x_part * x_num;
    int new_height = height / y_part * y_num;

    // 裁剪坐标
    int x_start = new_width * x_index;
    int y_start = new_height * y_index;

    // 裁剪图片
    cv::Mat new_image = image(cv::Rect(x_start, y_start, new_width, new_height));

    return new_image;
}

cv::Mat captureScreen(int x_begin, int y_begin, int x_end, int y_end)
{
    QPixmap screenShoot = captureFullScreen();
    cv::Mat screenImage = QImageToMat(screenShoot.toImage());
    cv::Mat partImage = screenImage(cv::Rect(x_begin, y_begin, x_end - x_begin, y_end - y_begin));
    return partImage;
}

CompeteRes competeFindPic(QStringList picList, int timeLimit, int interval)
{
    QElapsedTimer timer;
    timer.start();
    int num;

    cv::Point matchPoint(-1, -1);  // 默认返回值
    while (timer.elapsed() < timeLimit) {
        num = 0;
        // 在全屏截图中寻找图片
        for (QString pic : picList) {
            matchPoint = findPicInFullScreen(pic);

            // 如果找到图片（假设findPicInFullScreen返回的坐标是有效的）
            if (matchPoint.x != -1 && matchPoint.y != -1) {
                CompeteRes competeRes;
                competeRes.point = matchPoint;
                competeRes.num = num;
                return competeRes;
            }

            num++;
        }

        // 等待指定的时间间隔
        QThread::msleep(interval);
    }

    // 如果超时则返回 (-1, -1)
    CompeteRes competeRes;
    competeRes.point = cv::Point(-1, -1);
    competeRes.num = -1;
    return competeRes;
}

