#ifndef FINDPICPRO_H
#define FINDPICPRO_H

#include <QImage>
#include <QPixmap>
#include <opencv2/opencv.hpp>
#include <QElapsedTimer>
#include <QThread>

#include "snapShoot.h"

struct CompeteRes
{
    cv::Point point;
    int num;
};

cv::Mat convertToGray(const cv::Mat& inputImage);

// 将 QImage 转换为 cv::Mat
cv::Mat QImageToMat(const QImage &image);

cv::Point templateMatch(const cv::Mat& screenImage, const cv::Mat& templateImage, double threshold);

// 支持.png格式的资源文件图片
cv::Point findPicInFullScreen(QString picPath);

cv::Point findPicInFullScreen(cv::Mat& image);

// 循环找图
cv::Point findPicInLimitedTime(QString picPath, int timeLimit = 3000, int interval = 1000);

cv::Mat getPartOfScreen(int x_part, int y_part, int x_index, int y_index);
cv::Mat getPartsOfScreen(int x_part, int y_part, int x_index, int y_index, int x_num, int y_num);

cv::Mat cutImage(int x_part, int y_part, int x_index, int y_index, cv::Mat& image);
cv::Mat cutImages(int x_part, int y_part, int x_index, int y_index, int x_num, int y_num, cv::Mat& image);
cv::Mat captureScreen(int x_begin, int y_begin, int x_end, int y_end);

// 分情况找图，返回找到的图片点及图片序号
CompeteRes competeFindPic(QStringList picList, int timeLimit = 3000, int interval = 1000);

#endif // FINDPICPRO_H
