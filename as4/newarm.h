#ifndef NEWARM_H
#define NEWARM_H

#include "as4.h"

using namespace Eigen;

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
	void setWorldTransform();
	void setWorldPoint();
	void draw();
	void update(Vector3f pe);
	void translateToParent();
	void translateToOrigin();
	void updateWparentWchild();
	Matrix3f getPseudoInverseJacobian(Vector3f);
	Vector3f getEndEffector();


};

#endif