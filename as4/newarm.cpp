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
	// _Wparent << 1, 0, 0,
	//             0, 1, 0,
	//             0, 0, 1;
	// _Wchild << 1, 0, 0,
	//            0, 1, 0,
	//            0, 0, 1;
	setLocalTransform();
	// updateWparentWchild();
	setWorldPoint();
}

/** Sets arm as parent. This is currently the most childish one. **/
Arm::Arm(Arm* arm, float length, Vector3f r) {
	_length = length;
	_parent = arm;
	_child = NULL;
	_r = r;
	_outboard = *new Vector4f(length, 0.0f, 0.0f, 1.0f);
	_inboard = *new Vector4f(0.0f, 0.0f, 0.0f, 1.0f);
	// _Wparent = _parent->_W;
	// _Wchild << 1, 0, 0,
	//            0, 1, 0,
	//            0, 0, 1;
	setLocalTransform();
	// updateWparentWchild();
	setWorldPoint();
}

void Arm::addChild(float length, Vector3f r) {
	if (_child != NULL) {
		_child->addChild(length, r);
		// updateWparentWchild();
	} else {
		Arm* child = new Arm(this, length, r);
		_child = child;
		// updateWparentWchild();
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
    if (_parent != NULL) {
    	trans << 1, 0, 0, _parent->_outboard(0),
             0, 1, 0, _parent->_outboard(1),
             0, 0, 1, _parent->_outboard(2),
             0, 0, 0, 1;
    } else {
    	trans << 1, 0, 0, 0,
    	         0, 1, 0, 0,
    	         0, 0, 1, 0,
    	         0, 0, 0, 1;
    }
    _M = trans * rot4f;
}

void Arm::setWorldPoint() {
	Vector4f in(0, 0, 0, 1);
	Vector4f out(_length, 0, 0, 1);
	Arm* arm = mostparent();
	Matrix4f trans;
	trans << 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1;
	if (arm != this) {
		trans = trans * arm->_M;
		arm = arm->_child;
	}
	trans = trans * _M;
	_inboard = trans * in;
	_outboard = trans * out;
}

Arm* Arm::mostparent() {
	if (_parent != NULL) {
		return _parent->mostparent();
	}
	return this;
}

// void Arm::translateToOrigin() {
// 	_outboard = *new Vector3f(_length, 0.0f, 0.0f);
// 	_inboard = *new Vector3f(0.0f, 0.0f, 0.0f);
// }

// void Arm::translateToParent() {
// 	if (_parent != NULL) {
// 		_outboard = _outboard + _parent->_outboard;
// 		_inboard = _parent->_outboard;
// 	}
// }

// void Arm::updateWparentWchild() {
// 	Arm* pointUp = _parent;
// 	Arm* pointDown = _child;
// 	_Wparent = _M;
// 	if (pointUp != NULL) {
// 		_Wparent = _Wparent * pointUp->_M;
// 		while(pointUp->_parent != NULL) {
// 			pointUp = pointUp->_parent;
// 			_Wparent = pointUp->_M * _Wparent;
// 		}
// 	}
// 	_Wchild << 1, 0, 0,
// 	           0, 1, 0,
// 	           0, 0, 1;
// 	if (pointDown != NULL) {
// 		_Wchild = _Wchild * pointDown->_M;
// 		while (pointDown->_child != NULL) {
// 			pointDown = pointDown->_child;
// 			_Wchild = _Wchild * pointDown->_M;
// 		}
// 	}
// }

void Arm::update(Vector3f pe) {
	_jacob.makedr(pe);
}

// void Arm::updateControl(Vector3f g) {
// 	Vector3f p = getEndEffector();
// 	Vector3f dp = g - p;
// 	Matrix3f j = getJacobian();
// 	print(j);
// 	j = j.transpose() * (j * j.transpose()).inverse();
// 	// if (j(0, 0) != j(0, 0)) {
// 	// 	cout << " perturb " << endl;
// 	// 	perturb();
// 	// 	updateControl(g);
// 	// 	return;
// 	// }	
// 	Vector3f dr = j * dp;
// 	_r = _r + dr;
// }

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
	// child = _child;
	// updateWparentWchild();
	// while (child != NULL) {
	// 	child->updateWparentWchild();
	// 	child = child->_child;
	// }
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
	Vector4f end = getEndEffector();
	return -makeCross(end);
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

void Arm::perturb() {
	Vector3f pert(0, 0, 0);
	_r = _r + pert;
	constructM();
	finishUpdate();
}

Jacob::Jacob(Arm* arm) {
	_arm = arm;
	_arm2 = arm->_child;
	_arm3 = arm->_child->_child;
	_arm4 = arm->_child->_child->_child;
}


void Jacob::makedr(Vector3f g) {
	// Vector4f point = _arm->getEndEffector();
	// Vector3f dp = g - point;
	// Matrix3f J1 = _arm->getJacobian();
	// Matrix3f J2 = _arm2->getJacobian();
	// Matrix3f J3 = _arm3->getJacobian();
	// Matrix3f J4 = _arm4->getJacobian();
	// MatrixXf C(J1.rows(), J1.cols() + J2.cols() + J3.cols() + J4.cols());
	// C << J1, J2, J3, J4;
	// MatrixXf Cplus = C.transpose() * (C * C.transpose()).inverse();
	// MatrixXf dr = Cplus * dp;

	// _arm->constructM();
	// _arm->finishUpdate();

}
