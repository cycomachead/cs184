#include "arm.h"

using namespace Eigen;



Arm::Arm(float length, float x, float y, float radian) {
	_parent = NULL;
	_child = NULL;
	_x = x;
	_y = y;
	_radian = radian;
	_X = (length, 0, 0, 1);
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
	_parent = arm;
	_child = NULL;
	_x = x;
	_y = y;
	_radian = radian;
	_X = (length, 0, 0, 1);
	setLocalTransform();
	setWorldTransform();
	setWorldPoint();
	_Wparent = _parent->_W;
	_Wchild << 1, 0, 0, 0,
	           0, 1, 0, 0,
	           0, 0, 1, 0,
	           0, 0, 0, 1;
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
	float z = 1 - sqrt(x*x + y*y);
	Matrix4f new_rotate;
    Matrix4f rx;
    Matrix4f identity;
    Matrix4f translation;
    float s = sin(_radian);
    float c = cos(_radian);
    identity << 1, 0, 0, 0,
                0, 1, 0, 0,
                0, 0, 1, 0,
                0, 0, 0, 1;

    /** Set translation. **/
    if (_parent != NULL) {
    	translation << 1, 0, 0, _parent->_p[0],
    	               0, 1, 0, _parent->_p[1],
    	               0, 0, 1, _parent->_p[2],
    	               0, 0, 0, 1;
    } else {
    	translation = identity;
    }

    /** Set rotation. **/
    rx << 0, -1*z, _y, 0,
          z, 0, -1*_x, 0,
          -1*_y, _x, 0, 0,
          0, 0, 0, 1;
    _M = translation * (identity + (rx)*s + (rx)*(rx)*(1-c));
}

void Arm::setWorldTransform() {
	if (_parent != NULL) {
		_W = _W * _M;
	} else {
		_W = _M;
	}
}

void Arm::setWorldPoint() {
	_p = _W * _X;
}

void Arm::draw() {
	COLOR_GREEN
	glBegin(GL_LINES);
	if (_parent == NULL) {
		glVertex(0.0f, 0.0f, 0.0f);
	} else {
		glVertex(_parent->_p[0], _parent->_p[1], _parent->_p[2]);
	}
	glVertex(_p[0], _p[1], _p[2]);
	glEnd();
	if (_child != NULL) {
		_child->draw();
	}
}

