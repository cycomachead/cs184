#ifndef NEWARM_H
#define NEWARM_H

#include "as4.h"

using namespace Eigen;
void print(Vector3f vec);
void print(Vector4f vec);
void print(Matrix3f mat);
class Arm;

class Jacob {
	//this is 4 arms in one.
public:
	Arm* _arm;
	Arm* _arm2;
	Arm* _arm3;
	Arm* _arm4;
	Matrix<double, 3, 12> _jacobian;
	Matrix<double, 12, 1> _dr;
	Matrix<double, 3, 1> _dp;

	Jacob() {
		//Empty Constructor
	};
	Jacob(Arm*);

	bool makedr(Vector3f);

};

class Arm {

public:
	/** This pointer, points back to the origin. **/
	Arm* _parent;
	/** This pointer, points further out. **/
	Arm* _child;
	/** This is the out position. **/
	Vector4f _outboard;
	/** This is the inboard position. **/
	Vector4f _inboard;
	/** This is the world transformation. **/
	Matrix3f _R;
	/** This is phi and psi for the axis, angle representation. **/
	Vector3f _r;
	/** This updates our control. **/
	Jacob _jacob;

	float _length;
	Arm() {
		//Empty constructor.
	}

	/** This constructor will create the oldest parent.
	    This arm will have no parent. **/
	Arm(float, Vector3f);
	/** Sets arm as parent. **/
	Arm(Arm*, float, Vector3f);
	/** Concatenate a child with following float and local 
	    transformation. **/
	void addChild(float, Vector3f);
	void setLocalTransform();
	// void setWorldTransform();
	void setWorldPoint();
	void draw();
	bool update(Vector3f pe);
	void translateToParent();
	void translateToOrigin();
	void updateWparentWchild();
	void updateControl(Vector3f);
	void constructM();
	void finishUpdate();
	void setJacob();
	float armLength();
	Arm* mostparent();
	Matrix3f getJacobian();
	Vector4f getEndEffector();
	void perturb();

};

#endif