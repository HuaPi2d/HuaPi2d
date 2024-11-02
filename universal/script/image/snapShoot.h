#ifndef SNAPSHOOT_H
#define SNAPSHOOT_H

#include <QApplication>
#include <QScreen>
#include <QGuiApplication>
#include <QPixmap>
#include <QDateTime>
#include <QFileDialog>
#include <QImage>
#include <opencv2/core.hpp>
#include "universal/script/image/findPicPro.h"

QPixmap captureFullScreen();

cv::Point getScreenResolution();

#endif // SNAPSHOOT_H
