#include "coordinate.h"

QVector<double> euler_rotation_matrix_C(QVector<QVector2D> rotation_vector)
{
    // 初始化 MATLAB 环境
    if (!euler_rotation_matrixInitialize())
        return QVector<double>();

    // 定义传入和传出参数
    mwArray rotation_matrix(3, 3, mxDOUBLE_CLASS);
    mwArray rotation_axis(rotation_vector.length(), 2, mxINT32_CLASS, mxREAL);

    int* rotation_vector_data = new int[2 * rotation_vector.length()];
    for (int i = 0; i < rotation_vector.length(); i++)
    {
        rotation_vector_data[2 * i] = int(rotation_vector[i].x());
        rotation_vector_data[2 * i + 1] = int(rotation_vector[i].y());
    }
    qDebug() << rotation_vector_data[0] << " " << rotation_vector_data[1];

    rotation_axis.SetData(rotation_vector_data, 2 * rotation_vector.length());

    // 调用 MATLAB 函数
    euler_rotation_matrix(1, rotation_axis, rotation_matrix);

    // 从 mwArray rotation_matrix 中提取数据
    double matrix_data[9];
    rotation_matrix.GetData(matrix_data, 9);

    // 打印输出
    for(int i = 0; i < 3; i++)
    {
        for(int j = 0; j < 3; j++)
        {
            std::cout << matrix_data[3 * i + j] << " ";
        }
        std::cout << std::endl;
    }

    // 提取计算后的旋转矩阵中的 2D 分量
    

    // 清理 MATLAB 环境
    euler_rotation_matrixTerminate();

    return QVector<double>();
}

void test()
{
    if (!AddInitialize())
        return ;
    int a = 10, b = 20;
    int c;
    mwArray mwA(1, 1, mxINT32_CLASS);
    mwArray mwB(1, 1, mxINT32_CLASS);
    mwArray mwC(1, 1, mxINT32_CLASS);
    mwA.SetData(&a, 1);
    mwB.SetData(&b, 1);
    Add(1, mwC, mwA, mwB);
    c = mwC.Get(1, 1);
    std::cout << "c=" << c << std::endl;
}
