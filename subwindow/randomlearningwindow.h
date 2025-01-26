#ifndef RANDOMLEARNINGWINDOW_H
#define RANDOMLEARNINGWINDOW_H

#include <QMainWindow>
#include <QMdiSubWindow>

#include "subwidgets/randomlearningwidget.h"

class RandomLearningWindow : public QMdiSubWindow
{
    Q_OBJECT
public:
    explicit RandomLearningWindow(QWidget *parent = nullptr);

signals:
    void sendStateInfo(QString state);
    void windowDestroyed(QMdiSubWindow *window);
    void downloadFinished();
    void sendDsizeInfo(int dwidth, int dheight);
};

#endif // RANDOMLEARNINGWINDOW_H
