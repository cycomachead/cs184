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
	Vector4f _p;
	/** This is the local transformation. **/
	Matrix4f _M;
	/** This is the original x to be transformed. **/
	Vector4f _X;
	/** This is the world transformation. **/
	Matrix4f _W;
	/** This is phi and psi for the axis, angle representation. **/
	float _x, _y, _radian;
	/** This is the transformation up to the parent's transformation. **/
	Matrix4f _Wparent;
	/** This is the transformation of everything beyond this arm. **/
	Matrix4f _Wchild;

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


};

#endif