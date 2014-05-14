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

Matrix3f inline singular(Vector3f v) {
	float x, y, z;
	if (v(0) > -0.0001f && v(0) < 0.0001f) {
		x = 0;
	} else {
		x = 1/v(0);
	}
	if (v(1) > -0.0001f && v(1) < 0.0001f) {
		y = 0;
	} else {
		y = 1/v(1);
	}
	bool a =v(2) > -0.0001f;
	bool b =v(2) < 0.0001f;
	cout << a << endl;
	cout << b << endl;
	if(v(2) > -0.0001f && v(2) < 0.0001f) {
		z = 0;
	} else {
		z = 1/v(2);
	}
	Matrix3f mat;
	mat << x, 0, 0,
	       0, y, 0,
	       0, 0, z;
	return mat;
}

Vector3f dif(Vector3f a, Vector3f b) {
	Vector3f r;
	if (abs(a(0) - b(0)) < .0001) {
		r(0) = 0;
	} else {
		r(0) = a(0) - b(0);
	}

	if (abs(a(1) - b(1)) < .0001) {
		r(1) = 0;
	} else {
		r(1) = a(1) - b(1);
	}

	if (abs(a(2) - b(2)) < .0001) {
		r(2) = 0;
	} else {
		r(2) = a(2) - b(2);
	}

	return r;
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

void print(Matrix<double, 3, 12> mat) {
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
	updateWparentWchild();
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
	updateWparentWchild();
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
	cout <<endl;
	cout << _x << " " << _y << " " << z << " theta: " << _radian;
	cout <<endl;
	Matrix4f new_rotate;
    Matrix3f rx;
    Matrix3f identity;
    float s = sin(_radian);
    float c = cos(_radian);
    identity << 1, 0, 0,
                0, 1, 0,
                0, 0, 1;
    /** Set rotation. **/
    rx << 0, -z, _y,
          z, 0, -_x,
          -_y, _x, 0;

    Matrix3f rot = identity + (rx)*s + (rx)*(rx)*(1-c);
    _M = rot;
    cout << endl;
    cout << "Transformation matrix";
    print(_M);
    cout << endl;
}

// void Arm::setWorldTransform() {
// 	if (_parent != NULL) {
// 		_W = _parent->_W * _M;
// 	} else {
// 		_W = _M;
// 	}
// }

void Arm::setWorldPoint() {
	translateToOrigin();
	_outboard = _Wparent * _outboard;
	_inboard = _Wparent * _inboard;
	translateToParent();
}

void Arm::translateToOrigin() {
	_outboard = *new Vector3f(_length, 0.0f, 0.0f);
	_inboard = *new Vector3f(0.0f, 0.0f, 0.0f);
}

void Arm::translateToParent() {
	if (_parent != NULL) {
		_outboard = _outboard + _parent->_outboard;
		_inboard = _parent->_outboard;
	}
}

// void Arm::update(Vector3f pe) {
// 	Vector3f p = getEndEffector();
// 	Vector3f dp = dif(pe, p);
// 	Matrix3f pseudoJ = getPseudoInverseJacobian(pe);
// 	Vector3f dr = pseudoJ * dp;
// 	print(dr);
// 	_x = _x + dr[0];
// 	_y = _y + dr[1];
// 	_radian = _radian + dr[2];
// 	setLocalTransform();
// 	if (_child != NULL) {
// 		_child->update(pe);
// 	}
// 	updateWparentWchild();
// 	setWorldTransform();
// 	setWorldPoint();
// }

void Arm::updateWparentWchild() {
	Arm* pointUp = _parent;
	Arm* pointDown = _child;
	_Wparent = _M;
	if (pointUp != NULL) {
		_Wparent = _Wparent * pointUp->_M;
		while(pointUp->_parent != NULL) {
			pointUp = pointUp->_parent;
			_Wparent = pointUp->_M * _Wparent;
		}
	}
	_Wchild << 1, 0, 0,
	           0, 1, 0,
	           0, 0, 1;
	if (pointDown != NULL) {
		_Wchild = _Wchild * pointDown->_M;
		while (pointDown->_child != NULL) {
			pointDown = pointDown->_child;
			_Wchild = _Wchild * pointDown->_M;
		}
	}
}

void Arm::update(Vector3f pe) {
	_jacob.makedr(pe);
}

void Arm::updateControl(Vector3f g) {
	Vector3f p = getEndEffector();
	Vector3f dp = dif(p, g);
	cout << endl;
	cout << "difference" << endl;
	print(dp);
	cout << endl;
	Matrix3f j = getJacobian();
	JacobiSVD<MatrixXf> svd(j, ComputeThinU | ComputeThinV);
	Vector3f sing = svd.singularValues();
	j = svd.matrixU() * singular(sing) * svd.matrixV().transpose();
	cout << j << endl;
	cout << dp;
	Vector3f dr = j * dp;
	Vector3f zero(0, 0, 0);
	cout << endl;
	cout << "control change" << endl;
	print(dr);
	cout << endl;
	cout << "control before" << endl;
	cout << _x << endl;
	cout << _y << endl;
	cout << _radian << endl;
	cout << endl;
	_x = _x - dr[2];
	_y = _y - dr[0];
	_radian = _radian - dr[1];

	cout << endl;
	cout << "control after" <<endl;
	cout << _x << endl;
	cout << _y << endl;
	cout << _radian << endl;
	cout << endl;
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
	updateWparentWchild();
	while (child != NULL) {
		child->updateWparentWchild();
		child = child->_child;
	}
	child = _child;
	setWorldPoint();
	while (child != NULL) {
		child->setWorldPoint();
		child = child->_child;
	}
}

Vector3f Arm::getEndEffector() {
	if (_child == NULL) {
		return _outboard;
	}
	return _child->getEndEffector();
}

Matrix3f Arm::getJacobian() {
	Vector3f end = getEndEffector();
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


Jacob::Jacob(Arm* arm) {
	_arm = arm;
	// _arm2 = arm->_child;
	// _arm3 = arm->_child->_child;
	// _arm4 = arm->_child->_child->_child;
}

void Jacob::makedr(Vector3f g) {
	cout << endl;
	cout << "real goal";
	print(g);
	cout <<endl;
	Vector3f point = _arm->getEndEffector();
	if (point(0) - g(0) > 0.1f) {
		g(0) = point(0)  - .1;
	} else if (point(0) - g(0) < -0.1f) {
		g(0) = point(0) + .1;
	}

	if (point(1) - g(1) > 0.1f) {
		g(1) = point(1) - .1;
	} else if (point(1) - g(1) < -0.1f) {
		g(1) = point(1) + .1;
	}

	if (point(2) - g(2) > .1) {
		g(2) = point(2) - .1;
	} else if (point(2) - g(2) < -0.1f) {
		g(2) = point(2) + .1;
	}
	cout <<endl;
	cout << "goal point";
	print(g);
	cout << endl;
	_arm->updateControl(g);
	// _arm2->updateControl(pe);
	// _arm3->updateControl(pe);
	// _arm4->updateControl(pe);

	_arm->constructM();
	_arm->finishUpdate();

}
