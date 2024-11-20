#pragma once

#include <QOpenGLWidget>
#include <QOpenGLFunctions_4_3_Core>
#include <QWidget>

class RocketSimOpenGLWidget  : public QOpenGLWidget, QOpenGLFunctions_4_3_Core
{
	Q_OBJECT

public:
	explicit RocketSimOpenGLWidget(QWidget* parent = nullptr);
	~RocketSimOpenGLWidget();

// ������������QOpenGLWidget���麯������Ҫ��д
protected:
	virtual void initializeGL();
	virtual void resizeGL(int w, int h);
	virtual void paintGL();
};
