#include "mainwindow.h"
#include "qqml.h"

#include <QApplication>
#include <QQuickWidget>
#include <QCoreApplication>
#include <iostream>
#include "info/videoinfo.h"
#include "info/videoinfolistmodel.h"

int main(int argc, char *argv[])
{
    // 防止QQuickWidget在Windows下使用OpenGL渲染时出现黑屏 https://forum.qt.io/topic/148089/qopenglwidget-doesn-t-work-with-qquickwidget/3
    QQuickWindow::setGraphicsApi(QSGRendererInterface::OpenGL);
    QApplication a(argc, argv);
    qmlRegisterType<VideoInfo>("QMLVideoInfo", 1, 0, "VideoInfo");
    qmlRegisterType<VideoInfoListModel>("QMLVideoInfoListModel",1,0,"VideoInfoListModel");

    // 设置本地化为 UTF-8
    std::locale::global(std::locale(""));

    MainWindow w;
    w.show();
    return a.exec();
}
