#ifndef NEWARM_H
#define NEWARM_H

#include "as4.h"

using namespace Eigen;
void print(Vector3f vec);
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

	void makedr(Vector3f);

};

class Arm {

public:
	/** This pointer, points back to the origin. **/
	Arm* _parent;
	/** This pointer, points further out. **/
	Arm* _child;
	/** This is the out position. **/
	Vector3f _outboard;
	/** This is the inboard position. **/
	Vector3f _inboard;
	/** This is the local transformation. **/
	Matrix3f _M;
	/** This is the world transformation. **/
	Matrix3f _W;
	/** This is phi and psi for the axis, angle representation. **/
	float _x, _y, _radian;
	/** This is the transformation up to the parent's transformation. **/
	Matrix3f _Wparent;
	/** This is the transformation of everything beyond this arm. **/
	Matrix3f _Wchild;
	/** Jacobian calculator that calculates the updates. **/
	Jacob _jacob;

	float _length;
	Arm() {
		//Empty constructor.
	}

	/** This constructor will create the oldest parent.
	    This arm will have no parent. **/
	Arm(float, float, float, float);
	/** Sets arm as parent. **/
	Arm(Arm*, float, float, float, float);
	/** Concatenate a child with following float and local 
	    transformation. **/
	void addChild(float, float, float, float);
	void setLocalTransform();
	// void setWorldTransform();
	void setWorldPoint();
	void draw();
	void update(Vector3f pe);
	void translateToParent();
	void translateToOrigin();
	void updateWparentWchild();
	void updateControl(Vector3f);
	void constructM();
	void finishUpdate();
	void setJacob();
	Matrix3f getJacobian();
	Vector3f getEndEffector();


};

#endif