#include "newarm.h"
using namespace Eigen;

Vector4f convertTo4(Vector3f a) {
	Vector4f alpha(a[0], a[1], a[2], 1);
	return alpha;
}

Vector3f convertTo4(Vector4f a) {
	Vector3f alpha(a[0], a[1], a[2]);
	return alpha;
}
float inline euclid(Vector3f x, Vector3f y) {
	return sqrt(sqr(x[0] - y[0]) + sqr(x[1] - y[1]) + sqr(x[2] - y[2]));
}

float inline euclid(Vector4f x, Vector4f y) {
	return euclid(convertTo3(x), convertTo3(y));
}

Matrix3f makeCross(Vector3f x) {
	Matrix3f cross;
	cross << 0, -x[2], x[1],
             x[2], 0, -x[0],
             -x[1], x[0], 0;
    return cross;
}

Matrix3f makeCross(Vector4f x) {
    return mmakeCross(convertTo3(x));
}


void print(MatrixXf mat) {
	cout << endl;
	cout << mat << endl;
	cout << endl;
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
    Matrix3f identity = Matrix3f::Identity();
    float s = sin(_r.norm());
    float c = cos(_r.norm());
    float x, y, z;

    if (_r(0) != 0) {
    	x = _r(0)/_r.norm();
    }

    if (_r(1) != 0) {
    	y = _r(1)/_r.norm();
    }

    if (_r(2) != 0) {
    	z = _r(2)/_r.norm();
    }

    Vector3f rhat(x, y, z);
    Matrix3f rx = makeCross(rhat);
    Matrix3f rot = identity + (rx)*c + (rx)*(rx)*(1-s);
    _R = rot;
}

void Arm::setWorldPoint() {
	Vector3f in(0, 0, 0);
	Vector3f out(_length, 0, 0);
	Arm* arm = mostparent();
    Vector3f sum;

    if (_parent != NULL) {
        sum = convertTo3(_parent->_outboard);
    } else {
        sum = in;
    }

    Matrix3f prodCk = Matrix3f::Identity();
    while (true) {
        prodCk = prodCk * arm->_R;
        if (this == mostparent() or arm == this) {
            break;
        }
        arm = arm->_child;
    }
    _outboard = convertTo4(sum + (prodCk * out));
}

Arm* Arm::mostparent() {
	if (_parent != NULL) {
		return _parent->mostparent();
	}
	return this;
}

/** Returns true if you should move on. Returns false if you should stay.**/
bool Arm::update(Vector3f pe) {
	return _jacob.makedr(pe);
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

Matrix4f buildFunky(Matrix3f mat, Vector3f vec) {
    Matrix4f ret;
    ret << mat(0, 0), mat(0, 1), mat(0, 2), vec(0),
           mat(1, 0), mat(1, 1), mat(1, 2), vec(1),
           mat(2, 0), mat(2, 1), mat(2, 2), vec(2),
           0,         0,         0,         1;
    return ret;
}

Vector4f Arm::getEndEffector() {
    if (_child == NULL) {
        return _outboard;
    }

    Arm* end = this;
    while (end->_child != NULL) {
        end = end->_child;
    }

    Vector4f pend = end->_outboard;


    Matrix3f mat = Matrix3f::Identity();
    Vector3f v(_length, 0, 0);

    if (_parent != NULL) {
        mat = _R;
    }


    return buildFunky(mat, v) * _child->getEndEffector();

    // if (_child == NULL) {
    //     return _outboard;
    // }
    //
    // return _child->getEndEffector();
}

Matrix3f Arm::getJacobian() {
	Vector3f out = convertTo3(_outboard);
	Matrix3f trans = Matrix3f::Identity();
	Vector4f end = getEndEffector();
	Matrix3f rot;
	Matrix4f trans;
	Matrix4f armtran;

	// if (_parent!=NULL) {
	// 	armtran = armtran * getTranslation(_parent->_length, 0, 0);
	// 	trans = trans * armtran;
	// } else {
	// 	armtran = armtran * getTranslation(0, 0, 0);
	// 	trans = trans * armtran;
	// // }

	// if (_parent == NULL) {
	// 	return -makeCross(end);
	// }

	Arm* arm = this;
	rot = rot * arm->_R;
	while (arm->_child != NULL) {
		arm = arm->_child;
		rot = arm->_R * rot ;
	}

	arm = this;

	while (arm->_parent != NULL) {
		arm = arm->_parent;
		if (arm->_parent != NULL) {
			armtran = arm->getTranslation(arm->_parent->_length, 0, 0);
		} else {
			armtran = arm->getTranslation(0, 0, 0);
		}
		trans = armtran * trans;
	}



	Vector4f pei = trans * end;
	return -rot * makeCross(pei);
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
    Vector3f in;
    if (_parent != NULL) {
        in = convertTo3(_parent->_outboard);
    } else {
        in = Vector3f(0, 0, 0);
    }

    COLOR_WHITE;
	glBegin(GL_LINES);
	glVertex3f(in[0], in[1], in[2]);
	glVertex3f(_outboard[0], _outboard[1], _outboard[2]);
	glEnd();
    COLOR_MAGENTA;
	glBegin(GL_POINTS);
	glVertex3f(in[0], in[1], in[2]);
	glVertex3f(_outboard[0], _outboard[1], _outboard[2]);
	glEnd();
	if (_child != NULL) {
		_child->draw();
	}
}

float Arm::armLength() {
	float len = _length;
	if (_child != NULL) {
        len += _child->armLength();
		return len;
	}
	return len;
}

void Arm::setJacob() {
	_jacob = *new Jacob(this);
}

Matrix4f Arm::getTranslation(float x, float y, float z) {
	Matrix4f alpha;
	alpha << _R(0, 0), _R(0, 1), _R(0, 2), x,
	         _R(1, 0), _R(1, 1), _R(1, 2), y,
	         _R(2, 0), _R(2, 1), _R(2, 2), z,
	         0, 0, 0, 1;
	return alpha;
}

Jacob::Jacob(Arm* arm) {
	_arm = arm;
	_arm2 = arm->_child;
	_arm3 = arm->_child->_child;
	_arm4 = arm->_child->_child->_child;
}

void Arm::debug() {
	//cout << _jacob._dr;
	// print(_jacob._inverse);
	// print(_jacob._dr);
}


bool Jacob::makedr(Vector3f g) {
	float step = stepSize;
	Vector4f point = _arm->getEndEffector();
	Vector3f dp = (g - convertTo3(point));
	float x, y, z;

	if (dp(0) != 0) {
    	dp(0) = dp(0)/dp.norm();
    }

    if (dp(1) != 0) {
    	dp(1) = dp(1)/dp.norm();
    }

    if (dp(2) != 0) {
    	dp(2) = dp(2)/dp.norm();
    }

	float len = _arm->armLength() - .1;
    g = dp * len;
    dp = (g - convertTo3(point));


    // cout << "END EFFECTOR ";
    // print(_arm->getEndEffector());
    // cout << "DP";
    // print(dp);

    float dist = euclid(convertTo3(point), g);
	if (dist < 1) {
		return true;
	}

	dp = dp * step;

	Matrix3f J1 = _arm->getJacobian();
	Matrix3f J2 = _arm2->getJacobian();
	Matrix3f J3 = _arm3->getJacobian();
	Matrix3f J4 = _arm4->getJacobian();
	MatrixXf C(J1.rows(), J1.cols() + J2.cols() + J3.cols() + J4.cols());
	C << J1, J2, J3, J4;
	_jacobian = C;
	MatrixXf Cplus = C.transpose() * (C * C.transpose()).inverse();
	_inverse = Cplus;
	MatrixXf dr = (Cplus * dp) ;
	_dr = dr;

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

    // float currentError = (convertTo3(_arm->getEndEffector()) - g).norm();
    // if (currentError > prevError) {
    //     if (stepSize / 2 > 0.001) {
    //         stepSize /= 2;
    //     }
    // } else {
    //     stepSize *= 2;
    // }
	return false;

}
