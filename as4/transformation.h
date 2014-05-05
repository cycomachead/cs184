#ifndef TRANS_H
#define TRANS_H


#include "as4.h"

using namespace Eigen;

class Transformation {
    Matrix4f _mat;
public:
    Transformation();
    Transformation(Matrix4f mat);
    void add_rotation(float x, float y, float z, float radian);
    void add_translation(float x, float y, float z);
    void add_scaling(float x, float y, float z);
    void add_transformation(Transformation& trans);
    Transformation& invt();
    Transformation& inv();
    Vector4f operator*(Vector4f v);
    Vector3f operator*(Vector3f v);
};

#endif
