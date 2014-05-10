#include "as4.h"
#include "transformation.h"

using namespace Eigen;

/** This constructor will create an identity matrix.
    If applied this matrix will give a identity transformation.
    REMEMBER THAT THE MATRIX TENDS TO EXECUTE TRANSFORMATION IN THE
    ORDER THEY ARE PUT IN. **/
Transformation::Transformation() {
    _mat << 1, 0, 0,
           0, 1, 0,
           0, 0, 1;
}

/** A constructor that takes in a MAT which is a Matrix4f and copies it
    to our member. **/
Transformation::Transformation(Matrix3f mat) {
    _mat = mat;
}

/** Please make sure x + y is less than 1, This will be your phi and psi. **/
void Transformation::add_rotation(float x, float y, float radian) {
    float z = 1 - sqrt(sqr(x) + sqr(y));
    Matrix3f new_rotate;
    Matrix3f rx;
    Matrix3f identity;
    float s = sin(radian);
    float c = cos(radian);
    identity << 1, 0, 0,
                0, 1, 0,
                0, 0, 1;
    rx << 0, -1*z, y,
          z, 0, -1*x,
          -1*y, x, 0;
    new_rotate = identity + (rx)*s + (rx)*(rx)*(1-c);
    _mat = _mat * new_rotate;
}

/** Adds another transformation into this transformation.**/
void Transformation::add_transformation(Transformation& trans) {
  _mat = _mat * trans._mat;
}

Transformation& Transformation::invt() {
    return *new Transformation(_mat.inverse().transpose());
}

Transformation& Transformation::inv() {
    return *new Transformation(_mat.inverse());
}

Transformation& Transformation::transpose() {
    return *new Transformation(_mat.transpose());
}

Vector3f Transformation::operator*(Vector3f vec3) {
    return _mat * vec3;
}

void Transformation::rightMultiply(Matrix3f right) {
    _mat = _mat * right;
}

void Transformation::leftMultiply(Matrix3f left) {
    _mat = left * _mat;
}

Matrix3f Transformation::getMatrix() {
  return _mat;
}



