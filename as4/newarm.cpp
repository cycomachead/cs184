#include "newarm.h"
using namespace Eigen;

float inline euclid(Vector4f x, Vector4f y) {
	return sqrt(sqr(x[0] - y[0]) + sqr(x[1] - y[1]) + sqr(x[2] - y[2]));
}

Matrix3f makeCross(Vector4f x) {
	Matrix3f cross;
	cross << 0, -x[2], x[1],
             x[2], 0, -x[0],
             -x[1], x[0], 0;
    return cross;
}


Matrix3f makeCross(Vector3f x) {
	Matrix3f cross;
	cross << 0, -x[2], x[1],
             x[2], 0, -x[0],
             -x[1], x[0], 0;
    return cross;
}

Vector4f convertTo4(Vector3f a) {
	Vector4f alpha(a[0], a[1], a[2], 1);
	return alpha;
}

Vector3f convertTo3(Vector4f a) {
	Vector3f beta(a[0], a[1], a[2]);
	return beta;
}

void print(Vector3f vec) {
	cout << endl;
	cout << vec << endl;
	cout << endl;
}

void print(Vector4f vec) {
	cout << endl;
	cout << vec << endl;
	cout << endl;
}

void print(Matrix3f mat) {
	cout << endl;
	cout << mat << endl;
	cout << endl;
}

Arm::Arm(float length, Vector3f r) {
	_length = length;
	_parent = NULL;
	_child = NULL;
	_r = r;
	_outboard = *new Vector4f(length, 0.0f, 0.0f, 1.0f);
	_inboard = *new Vector4f(0.0f, 0.0f, 0.0f, 1.0f);
	setLocalTransform();
	setWorldPoint();
}

/** Sets arm as parent. This is currently the most childish one. **/
Arm::Arm(Arm* arm, float length, Vector3f r) {
	_length = length;
	_parent = arm;
	arm->_child = this;
	_child = NULL;
	_r = r;
	_outboard = *new Vector4f(length, 0.0f, 0.0f, 1.0f);
	_inboard = *new Vector4f(0.0f, 0.0f, 0.0f, 1.0f);
	setLocalTransform();
	setWorldPoint();
}

void Arm::addChild(float length, Vector3f r) {
	if (_child != NULL) {
		_child->addChild(length, r);
	} else {
		Arm* child = new Arm(this, length, r);
	}
}

void Arm::setLocalTransform() {
	Matrix4f new_rotate;
    Matrix3f identity;
    float s = sin(_r.norm());
    float c = cos(_r.norm());
    float x, y, z;

    if (_r(0) == 0) {
    	x = 0;
    } else {
    	x = _r(0)/_r.norm();
    }

    if (_r(1) == 0) {
    	y = 0;
    } else {
    	y = _r(1)/_r.norm();
    }

    if (_r(2) == 0) {
    	z = 0;
    } else {
    	z = _r(2)/_r.norm();
    }

    Vector3f rhat(x, y, z);
    Matrix3f rx = makeCross(rhat);
    identity << 1, 0, 0, 0, 1, 0, 0, 0, 1;
    Matrix3f rot = identity + (rx)*c + (rx)*(rx)*(1-s);
    _R = rot;
    Matrix4f rot4f;
    rot4f << rot(0, 0), rot(0, 1), rot(0, 2), 0,
             rot(1, 0), rot(1, 1), rot(1, 2), 0,
             rot(2, 0), rot(2, 1), rot(2, 2), 0,
             0, 0, 0, 1;
    Matrix4f trans;
    Vector3f l(_length, 0, 0);
    Vector3f atran = rot * l;
    trans << 1, 0, 0, atran(0),
             0, 1, 0, atran(1),
             0, 0, 1, atran(2),
             0, 0, 0, 1;

    // _M = trans * rot4f;
}

void Arm::setWorldPoint() {
	Vector3f in(0, 0, 0);
	Vector3f out(_length, 0, 0);
	Arm* arm = mostparent();
	Matrix3f trans;
	trans << 1, 0, 0, 0, 1, 0, 0, 0, 1;
	while (arm->_length != this->_length) {
		trans = trans * arm->_R;
		if (arm->_child != NULL) {
			arm = arm->_child;
		}
	}
	trans = trans * _R;
	_inboard = convertTo4(trans * in);
	_outboard = convertTo4(trans * out);
	if (_parent != NULL) {
		_inboard = _inboard + _parent->_outboard;
		_outboard = _outboard + _parent->_outboard;
		_inboard(3) = 1;
		_outboard(3) = 1;
	}
}

Arm* Arm::mostparent() {
	if (_parent != NULL) {
		return _parent->mostparent();
	}
	return this;
}

void Arm::update(Vector3f pe) {
	_jacob.makedr(pe);
}

void Arm::updateControl(Vector3f change) {
	_r = _r + change;
}

void Arm::constructM() {
	setLocalTransform();
	Arm* child = _child;
	while(child != NULL) {
		child->setLocalTransform();
		child = child->_child;
	}
}

void Arm::finishUpdate() {
	Arm* child;
	child = _child;
	setWorldPoint();
	while (child != NULL) {
		child->setWorldPoint();
		child = child->_child;
	}
}

Vector4f Arm::getEndEffector() {
	if (_child == NULL) {
		return _outboard;
	}
	return _child->getEndEffector();
}


Matrix3f Arm::getJacobian() {
	Vector3f out = convertTo3(_outboard);
	Matrix3f trans;
	trans << 1, 0, 0,
	         0, 1, 0,
	         0, 0, 1;
	Arm* arm = this;
	while (arm->_parent != NULL) {
		arm = arm->_parent;
		trans = trans * _parent->_R;
	}
	out = trans * out;
	return -makeCross(out);
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

void Arm::setJacob() {
	_jacob = *new Jacob(this);
}

Jacob::Jacob(Arm* arm) {
	_arm = arm;
	_arm2 = arm->_child;
	_arm3 = arm->_child->_child;
	_arm4 = arm->_child->_child->_child;
}


void Jacob::makedr(Vector3f g) {
	float k = 0.05;
	Vector4f point = _arm->getEndEffector();
	Vector3f dp = k*(g - convertTo3(point));
	Matrix3f J1 = _arm->getJacobian();
	Matrix3f J2 = _arm2->getJacobian();
	Matrix3f J3 = _arm3->getJacobian();
	Matrix3f J4 = _arm4->getJacobian();
	MatrixXf C(J1.rows(), J1.cols() + J2.cols() + J3.cols() + J4.cols());
	C << J1, J2, J3, J4;
	MatrixXf Cplus = C.transpose() * (C * C.transpose()).inverse();
	MatrixXf dr = (Cplus * dp) ;

	Vector3f alpha(dr(0, 0), dr(1, 0), dr(2, 0));
	Vector3f beta(dr(3, 0), dr(4, 0), dr(5, 0));
	Vector3f gamma(dr(6, 0), dr(7, 0), dr(8, 0));
	Vector3f delta(dr(9, 0), dr(10, 0), dr(11, 0));

	_arm->updateControl(alpha);
	_arm2->updateControl(beta);
	_arm3->updateControl(gamma);
	_arm4->updateControl(delta);

	_arm->constructM();
	_arm->finishUpdate();

}