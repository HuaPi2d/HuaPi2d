#include "rocketsimopenglwidget.h"

RocketSimOpenGLWidget::RocketSimOpenGLWidget(QWidget *parent)
	: QOpenGLWidget(parent)
{}

RocketSimOpenGLWidget::~RocketSimOpenGLWidget()
{}

void RocketSimOpenGLWidget::initializeGL()
{
	this->initializeOpenGLFunctions();
}

void RocketSimOpenGLWidget::resizeGL(int w, int h)
{
}

void RocketSimOpenGLWidget::paintGL()
{
	glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);
}
