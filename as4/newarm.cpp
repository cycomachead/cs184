#include "newarm.h"
using namespace Eigen;

float inline euclid(Vector4f x, Vector4f y) {
	return sqrt(sqr(x[0] - y[0]) + sqr(x[1] - y[1]) + sqr(x[2] - y[2]));
}

Arm::Arm(float length, float x, float y, float radian) {
	_length = length;
	_parent = NULL;
	_child = NULL;
	_x = x;
	_y = y;
	_radian = radian;
	_outboard = *new Vector4f(length, 0.0f, 0.0f, 1.0f);
	_inboard = *new Vector4f(0.0f, 0.0f, 0.0f, 1.0f);
	setLocalTransform();
	setWorldTransform();
	setWorldPoint();
	_Wparent << 1, 0, 0, 0,
	            0, 1, 0, 0, 
	            0, 0, 1, 0, 
	            0, 0, 0, 1;
	_Wchild << 1, 0, 0, 0,
	           0, 1, 0, 0,
	           0, 0, 1, 0,
	           0, 0, 0, 1;
}

/** Sets arm as parent. This is currently the most childish one. **/
Arm::Arm(Arm* arm, float length, float x, float y, float radian) {
	_length = length;
	_parent = arm;
	_child = NULL;
	_x = x;
	_y = y;
	_radian = radian;
	_outboard = *new Vector4f(length, 0.0f, 0.0f, 1.0f);
	_inboard = *new Vector4f(0.0f, 0.0f, 0.0f, 1.0f);
	_Wparent = _parent->_W;
	_Wchild << 1, 0, 0, 0,
	           0, 1, 0, 0,
	           0, 0, 1, 0,
	           0, 0, 0, 1;
	setLocalTransform();
	setWorldTransform();
	setWorldPoint();
}

void Arm::addChild(float length, float x, float y, float radian) {
	if (_child != NULL) {
		_child->addChild(length, x, y, radian);
		_Wchild = _child->_M * _child->_Wchild;
	} else {
		Arm* child = new Arm(this, length, x, y, radian);
		_child = child;
		_Wchild = _Wchild * _child->_M;
	}
}

void Arm::setLocalTransform() {
	float z = sqrt(1 - (sqr(_x) + sqr(_y)));
	Matrix4f new_rotate;
    Matrix3f rx;
    Matrix3f identity;
    float s = sin(_radian);
    float c = cos(_radian);
    identity << 1, 0, 0,
                0, 1, 0,
                0, 0, 1;
    /** Set rotation. **/
    rx << 0, -1*z, _y,
          z, 0, -1*_x,
          -1*_y, _x, 0;

    Matrix3f rot3f = identity + (rx)*s + (rx)*(rx)*(1-c);
    Matrix4f rotation4f;
    rotation4f << rot3f(0, 0), rot3f(0, 1), rot3f(0, 2), 0,
                  rot3f(1, 0), rot3f(1, 1), rot3f(1, 2), 0,
                  rot3f(2, 0), rot3f(2, 1), rot3f(2, 2), 0,
                  0, 0, 0, 1;
    _M = rotation4f;
}

Matrix4f Arm::getTranslationToParent() {
	Matrix4f translationOtoP;
	if (_parent != NULL) {
    	translationOtoP << 1, 0, 0, _parent->_outboard[0],
    	               0, 1, 0, _parent->_outboard[1],
    	               0, 0, 1, _parent->_outboard[2],
    	               0, 0, 0, 1;
    } else {
    	translationOtoP << 1, 0, 0, 0,
                       0, 1, 0, 0,
                       0, 0, 1, 0,
                       0, 0, 0, 1;
    }
    return translationOtoP;
}

void Arm::setWorldTransform() {
	if (_parent != NULL && _child != NULL) {
		_W = _parent->_W * _M;
		_W = _W * _Wchild;
	} else if (_parent != NULL) {
		_W = _parent->_W * _M;
	} else if (_child != NULL) {
		_W = _M * _Wchild;
	} else {
		_W = _M;
	}
}

void Arm::setWorldPoint() {
	_outboard = _W * _outboard;
	_inboard = _W * _inboard;
	Matrix4f parentout = getTranslationToParent();
	_outboard = parentout * _outboard;
	_inboard = parentout * _inboard;
}

void Arm::draw() {
	if (_length == 1) {
		COLOR_GREEN;
	} else if (_length == 2) {
		COLOR_YELLOW;
	} else if (_length == 3) {
		COLOR_BLUE;
	} else if (_length == 4) {
		COLOR_RED;
	}
	glBegin(GL_LINES);
	glVertex3f(_inboard[0], _inboard[1], _inboard[2]);
	glVertex3f(_outboard[0], _outboard[1], _outboard[2]);
	glEnd();
	if (_child != NULL) {
		_child->draw();
	}
}

