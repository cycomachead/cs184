#include "newarm.h"
using namespace Eigen;

float inline euclid(Vector4f x, Vector4f y) {
	return sqrt(sqr(x[0] - y[0]) + sqr(x[1] - y[1]) + sqr(x[2] - y[2]));
}

Matrix3f makeCross(Vector3f x) {
	Matrix3f cross;
	cross << 0, -x[2], x[1],
             x[2], 0, -x[0],
             -x[1], x[0], 0;
    return cross;	                  
}

void print(Vector3f vec) {
	cout << endl;
	cout << vec << endl;
	cout << endl;
}

void print(Matrix3f mat) {
	cout << endl;
	cout << mat << endl;
	cout << endl;
}

Arm::Arm(float length, float x, float y, float radian) {
	_length = length;
	_parent = NULL;
	_child = NULL;
	_x = x;
	_y = y;
	_radian = radian;
	_outboard = *new Vector3f(length, 0.0f, 0.0f);
	_inboard = *new Vector3f(0.0f, 0.0f, 0.0f);
	_Wparent << 1, 0, 0,
	            0, 1, 0,
	            0, 0, 1;
	_Wchild << 1, 0, 0,
	           0, 1, 0,
	           0, 0, 1;
	setLocalTransform();
	setWorldTransform();
	setWorldPoint();
}

/** Sets arm as parent. This is currently the most childish one. **/
Arm::Arm(Arm* arm, float length, float x, float y, float radian) {
	_length = length;
	_parent = arm;
	_child = NULL;
	_x = x;
	_y = y;
	_radian = radian;
	_outboard = *new Vector3f(length, 0.0f, 0.0f);
	_inboard = *new Vector3f(0.0f, 0.0f, 0.0f);
	_Wparent = _parent->_W;
	_Wchild << 1, 0, 0,
	           0, 1, 0,
	           0, 0, 1;
	setLocalTransform();
	setWorldTransform();
	setWorldPoint();
}

void Arm::addChild(float length, float x, float y, float radian) {
	if (_child != NULL) {
		_child->addChild(length, x, y, radian);
		updateWparentWchild();
	} else {
		Arm* child = new Arm(this, length, x, y, radian);
		_child = child;
		updateWparentWchild();
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

    Matrix3f rot = identity + (rx)*s + (rx)*(rx)*(1-c);
    _M = rot;
}

void Arm::setWorldTransform() {
	if (_parent != NULL) {
		_W = _parent->_W * _M;
	} else {
		_W = _M;
	}
}

void Arm::setWorldPoint() {
	translateToOrigin();
	_outboard = _W * _outboard;
	_inboard = _W * _inboard;
	translateToParent();
}

void Arm::translateToOrigin() {
	_outboard = *new Vector3f(_outboard[0] - _inboard[0], 
		_outboard[1] - _inboard[1], _outboard[2] - _inboard[2]);
	_inboard = *new Vector3f(0.0f, 0.0f, 0.0f);
}

void Arm::translateToParent() {
	if (_parent != NULL) {
		Arm* p = _parent;
		_outboard = *new Vector3f(_outboard[0] + p->_outboard[0], 
			_outboard[1] + p->_outboard[1], _outboard[2] + p->_outboard[2]);
		_inboard = *new Vector3f(p->_outboard[0], p->_outboard[1], p->_outboard[2]);
	}
}

void Arm::update(Vector3f pe) {
	Vector3f p = getEndEffector();
	Vector3f dp = pe - p;
	Matrix3f pseudoJ = getPseudoInverseJacobian(pe);
	Vector3f dr = pseudoJ * dp;
	_x = _x + dr[0];
	_y = _y + dr[1];
	_radian = _radian + dr[2];
	setLocalTransform();
	setWorldTransform();
	setWorldPoint();
	if (_child != NULL) {
		_child->update(pe);
	}
	updateWparentWchild();
}

void Arm::updateWparentWchild() {
	Arm* pointUp = _parent;
	Arm* pointDown = _child;
	if (pointUp != NULL) {
		_Wparent = _parent->_M;
		while(pointUp->_parent != NULL) {
			pointUp = pointUp->_parent;
			_Wparent = pointUp->_M * _Wparent;
		}
	}
	_Wchild = _M;
	if (pointDown != NULL) {
		_Wchild = _Wchild * pointDown->_M;
		while (pointDown->_child != NULL) {
			pointDown = pointDown->_child;
			_Wchild = _Wchild * pointDown->_M;
		}
	}
}

Vector3f Arm::getEndEffector() {
	if (_child == NULL) {
		return _outboard;
	}
	return _child->getEndEffector();
}

Matrix3f Arm::getPseudoInverseJacobian(Vector3f pe) {
	Matrix3f j = -_Wparent * makeCross(_Wchild * pe);
	Matrix3f j2 = (j * j.transpose()).inverse();
	print(j2);
	Matrix3f psudoj = j.transpose() * (j * j.transpose()).inverse();
	return psudoj;
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

