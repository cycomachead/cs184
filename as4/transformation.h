#ifndef TRANS_H
#define TRANS_H


#include "as4.h"

using namespace Eigen;

class Transformation {
    Matrix3f _mat;
public:
    Transformation();
    Transformation(Matrix3f mat);
    void add_rotation(float x, float y, float radian);
    void add_transformation(Transformation& trans);
    Transformation& invt();
    Transformation& inv();
    Transformation& transpose();
    Vector3f operator*(Vector3f v);
    void rightMultiply(Matrix3f right);
    void leftMultiply(Matrix3f left);
    Matrix3f getMatrix();
};

#endif
