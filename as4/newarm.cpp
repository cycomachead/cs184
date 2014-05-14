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

Matrix<double, 3, 12> invertDiag(Vector3f mat) {
	float x, y, z;
	if (mat(0,0) != 0) {
		x = 1 / mat(0);
	} else {
		x = 0.0f;
	}
	if (mat(1,1) != 0) {
		y = 1 / mat(1);
	} else {
		y = 0.0f;
	}
	if (mat(2,2) != 0) {
		z = 1 / mat(2);
	} else {
		z = 0.0f;
	}

	Matrix<double, 3, 12>  rmat;
	rmat << x, 0, 0,
	        0, y, 0,
	        0, 0, z,
	        0, 0, 0,
	        0, 0, 0,
	        0, 0, 0,
	        0, 0, 0, 
	        0, 0, 0, 
	        0, 0, 0,
	        0, 0, 0,
	        0, 0, 0,
	        0, 0, 0;
	return rmat;
}

Matrix<double, 3, 1> dif(Vector3f a, Vector3f b) {
	Matrix<double, 3, 1> r;
	if (a(0) - b(0) < .0001) {
		r(0) = 0;
	} else {
		r(0) = a(0) - b(0);
	}

	if (a(1) - b(1) < .0001) {
		r(1) = 0;
	} else {
		r(1) = a(1) - b(1);
	}

	if (a(2) - b(2) < .0001) {
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
    rx << 0, -z, _y,
          z, 0, -_x,
          -_y, _x, 0;

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
	// cout << "====================";
	// cout << _length << endl;
	// cout << "outboard" << endl;
	// print(_outboard);
	// cout << "inboard" << endl;
	// print(_inboard);
	// cout << "====================";
}

void Arm::translateToOrigin() {
	_outboard = *new Vector3f(_length, 0.0f, 0.0f);
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
	if (pointUp != NULL) {
		_Wparent = pointUp->_M;
		while(pointUp->_parent != NULL) {
			pointUp = pointUp->_parent;
			_Wparent = pointUp->_M * _Wparent;
		}
	}
	_Wchild = _M;
	if (pointDown != NULL) {
		_Wchild = pointDown->_M;
		while (pointDown->_child != NULL) {
			pointDown = pointDown->_child;
			_Wchild = _Wchild * pointDown->_M;
		}
	}
}

void Arm::update(Vector3f pe) {
	_jacob.makedr(pe);
}

void Arm::updateControl(Vector3f dr) {
	_x += dr[0];
	_y += dr[1];
	_radian += dr[2];
}

void Arm::constructM() {
	setLocalTransform();
	if (_child != NULL) {
		_child->setLocalTransform();
	}
}

void Arm::finishUpdate() {
	updateWparentWchild();
	Arm* child;
	if (_child != NULL) {
		child = _child;
		while (child != NULL) {
			child->updateWparentWchild();
			child = child->_child;
		}
		child = _child;
		updateWparentWchild();
		while (child != NULL) {
			child->updateWparentWchild();
			child = child->_child;
		}
		child = _child;
		setWorldTransform();
		while (child!= NULL) {
			child->setWorldTransform();
			child = child->_child;
		}
		child = _child;
		setWorldPoint();
		while (child != NULL) {
			child->setWorldPoint();
			child = child->_child;
		}
	}	
}

Vector3f Arm::getEndEffector() {
	if (_child == NULL) {
		return _outboard;
	}
	return _child->getEndEffector();
}

Matrix3f Arm::getJacobian(Vector3f pe) {
	Matrix3f j = -_W * makeCross(_Wchild * pe);
	// JacobiSVD<MatrixXf> svd(j, ComputeThinU | ComputeThinV);
	// Vector3f sing = svd.singularValues();
	// Matrix3f pseudo = svd.matrixU() * invertDiag(sing) * svd.matrixV().transpose();
	return j;
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

void Jacob::makedr(Vector3f pe) {
	_dp = dif(pe, _arm->getEndEffector());
	Matrix3f origin  = _arm->getJacobian(pe);
	Matrix3f origin2 = _arm2->getJacobian(pe);
	Matrix3f origin3 = _arm3->getJacobian(pe);
	Matrix3f origin4 = _arm4->getJacobian(pe);

	_jacobian(0, 0)  = origin(0, 0);
	_jacobian(0, 1)  = origin(0, 1);
	_jacobian(0, 2)  = origin(0, 2);
	_jacobian(1, 0)  = origin(1, 0);
	_jacobian(1, 1)  = origin(1, 1);
	_jacobian(1, 2)  = origin(1, 2);
	_jacobian(2, 0)  = origin(2, 0);
	_jacobian(2, 1)  = origin(2, 1);
	_jacobian(2, 2)  = origin(2, 2);

	_jacobian(0, 3)  = origin2(0, 0);
	_jacobian(0, 4)  = origin2(0, 1);
	_jacobian(0, 5)  = origin2(0, 2);
	_jacobian(1, 3)  = origin2(1, 0);
	_jacobian(1, 4)  = origin2(1, 1);
	_jacobian(1, 5)  = origin2(1, 2);
	_jacobian(2, 3)  = origin2(2, 0);
	_jacobian(2, 4)  = origin2(2, 1);
	_jacobian(2, 5)  = origin2(2, 2);

	_jacobian(0, 6)  = origin3(0, 0);
	_jacobian(0, 7)  = origin3(0, 1);
	_jacobian(0, 8)  = origin3(0, 2);
	_jacobian(1, 6)  = origin3(1, 0);
	_jacobian(1, 7)  = origin3(1, 1);
	_jacobian(1, 8)  = origin3(1, 2);
	_jacobian(2, 6)  = origin3(2, 0);
	_jacobian(2, 7)  = origin3(2, 1);
	_jacobian(2, 8)  = origin3(2, 2);

	_jacobian(0, 9)  = origin4(0, 0);
	_jacobian(0, 10) = origin4(0, 1);
	_jacobian(0, 11) = origin4(0, 2);
	_jacobian(1, 9)  = origin4(1, 0);
	_jacobian(1, 10) = origin4(1, 1);
	_jacobian(1, 11) = origin4(1, 2);
	_jacobian(2, 9)  = origin4(2, 0);
	_jacobian(2, 10) = origin4(2, 1);
	_jacobian(2, 11) = origin4(2, 2);


	print(_jacobian);

	// JacobiSVD< Matrix<double, 3, 12> > svd(_jacobian, ComputeFullU | ComputeFullV);
	// // Vector3f sing = svd.singularValues();
	// cout << svd.matrixU() << endl;
	// cout << svd.singularValues() << endl;
	// cout << svd.matrixV() << endl;
	// Matrix<double, 3, 12> pseudo = svd.matrixU() * invertDiag(sing) * svd.matrixV().transpose();

	Matrix<double, 12, 3> pseudo = _jacobian.transpose() * (_jacobian * _jacobian.transpose()).inverse();

	_dr = pseudo * _dp;

	Vector3f alpha(_dr(0), _dr(1), _dr(2));
	Vector3f beta(_dr(3), _dr(4), _dr(5));
	Vector3f gamma(_dr(6), _dr(7), _dr(8));
	Vector3f delta(_dr(9), _dr(10), _dr(11));

	_arm->updateControl(alpha);
	_arm->_child->updateControl(beta);
	_arm->_child->_child->updateControl(gamma);
	_arm->_child->_child->_child->updateControl(delta);

	_arm->constructM();
	_arm->finishUpdate();

}
