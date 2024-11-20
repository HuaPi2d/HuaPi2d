#pragma once

#include "euler_rotation_matrix.h"
#include "engine.h"
#include "Add.h"
#include "testparams.h"
#include <QVector2D>
#include <QVector>
#include <QDebug>

QVector<double> euler_rotation_matrix_C(QVector<QVector2D> rotation_vector);

void test();