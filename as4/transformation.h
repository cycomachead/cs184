#ifndef TRANS_H
#define TRANS_H


#include "as4.h"

using namespace Eigen;

class Transformation {
    Matrix4f _mat;
public:
    Transformation();
    Transformation(Matrix4f mat);
    void add_rotation(Vector4f axis, float radian);
    void add_translation(float x, float y, float z);
    void add_scaling(float x, float y, float z);
    Transformation& invt();
    Transformation& inv();
    Vector4f operator*(Vector4f v);
};

#endif
