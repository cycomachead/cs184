#include "as4.h"
#include "transformation.h"

using namespace Eigen;


/** This constructor will create an identity matrix.
    If applied this matrix will give a identity transformation.
    REMEMBER THAT THE MATRIX TENDS TO EXECUTE TRANSFORMATION IN THE
    ORDER THEY ARE PUT IN. **/
Transformation::Transformation() {
    _mat << 1, 0, 0, 0,
           0, 1, 0, 0,
           0, 0, 1, 0,
           0, 0, 0, 1;
}

/** A constructor that takes in a MAT which is a Matrix4f and copies it
    to our member. **/
Transformation::Transformation(Matrix4f mat) {
    _mat = mat;
}

/** Adds a rotation to the current matrix. This matrix will rotate points
    about ABOUT by radian RADIAN. Remember that quaternion rotation matrix
    are NOT commutative, so take care when deciding the order of rotation. **/
void Transformation::add_rotation(Vector4f axis, float radian) {
    Matrix4f new_rotate;
    Matrix4f rx;
    Matrix4f identity;
    float s = sin(radian);
    float c = cos(radian);
    float x = axis(0);
    float y = axis(1);
    float z = axis(2);
    identity << 1, 0, 0, 0,
                0, 1, 0, 0,
                0, 0, 1, 0,
                0, 0, 0, 1;
    rx << 0, -1*z, y, 0,
          z, 0, -1*x, 0,
          -1*y, x, 0, 0,
          0, 0, 0, 1;
    new_rotate = identity + (rx)*s + (rx)*(rx)*(1-c);
    _mat = _mat * new_rotate;
}

/** A translation matrix that will move the point by, X, Y, Z. **/
void Transformation::add_translation(float x, float y, float z) {
    Matrix4f new_translate;
    new_translate << 1, 0, 0, x,
                     0, 1, 0, y,
                     0, 0, 1, z, 
                     0, 0, 0, 1;
    _mat = _mat * new_translate;
}

/** Adds on a transformation that will scale points by X, Y, Z. **/
void Transformation::add_scaling(float x, float y, float z) {
    Matrix4f new_scaling;
    new_scaling << x, 0, 0, 0,
                   0, y, 0, 0,
                   0, 0, z, 0,
                   0, 0, 0, 1;
    _mat = _mat * new_scaling;
}

Transformation& Transformation::invt() {
    return *new Transformation(_mat.inverse().transpose());
}

Transformation& Transformation::inv() {
    return *new Transformation(_mat.inverse());
}
