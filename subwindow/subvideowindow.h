#ifndef SUBVIDEOWINDOW_H
#define SUBVIDEOWINDOW_H

#include <QMainWindow>
#include <QMdiSubWindow>
#include "otherwidgets/singledownloadframe.h"
#include "subwidgets/subvideowidget.h"

class SubVideoWindow : public QMdiSubWindow
{
    Q_OBJECT
public:
    explicit SubVideoWindow(QWidget *parent = nullptr);

public: signals:
    void sendStateInfo(QString state);
    void windowDestroyed(QMdiSubWindow *window);
    void updateQuickWidgetTheme(acss::QtAdvancedStylesheet* advancedStyleSheet);
};

#endif // SUBVIDEOWINDOW_H
