#include "as4.h"
#include "transformation.h"

using namespace Eigen;


Matrix4f convertTo4(Matrix3f mat3) {
  Matrix4f mat = *new Matrix4f();
  mat << mat3(0, 0), mat3(0, 1), mat3(0, 2), 0,
         mat3(1, 0), mat3(1, 1), mat3(1, 2), 0,
         mat3(2, 0), mat3(2, 1), mat3(2, 2), 0,
         0, 0, 0, 1;
  return mat;
}

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
    are NOT commutative, so take care when deciding the order of rotation.
    X, Y, Z represents axis **/
void Transformation::add_rotation(float x, float y, float z, float radian) {
    Matrix4f new_rotate;
    Matrix4f rx;
    Matrix4f identity;
    float s = sin(radian);
    float c = cos(radian);
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

Vector4f Transformation::operator*(Vector4f vec4) {
  return _mat * vec4;
}

Vector3f Transformation::operator*(Vector3f vec3) {
  Vector4f vec4(vec3[0], vec3[1], vec3[2], 1);
  vec4 = _mat * vec4;
  Vector3f new_vec3(vec4[0], vec4[1], vec4[2]);
  return new_vec3;
}

void Transformation::rightMultiply(Matrix4f right) {
  _mat = _mat * right;
}

void Transformation::leftMultiply(Matrix4f left) {
  _mat = left * _mat;
}

void Transformation::rightMultiplyBy3f(Matrix3f right) {
  Matrix4f mult = convertTo4(right);
  _mat = _mat * mult;
}

void Transformation::leftMultiplyBy3f(Matrix3f left) {
  Matrix4f mult = convertTo4(left);
  _mat = mult * _mat;
}

Matrix4f Transformation::getMatrix() {
  return _mat;
}



