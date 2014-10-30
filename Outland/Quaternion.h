#ifndef Quaternion_H
#define Quaternion_H

#include "UnitAbstraction.h"
#include "Vector.h"

class Quaternion
{
public:
	real32 w,x,y,z;

	Quaternion();
	Quaternion(real32 w, real32 x, real32 y, real32 z);

	Quaternion Normalize();
	Quaternion operator* (Quaternion &b);
	Quaternion operator+ (Quaternion &b);
	Quaternion operator- (Quaternion &b);
	Quaternion getConjugate();
};

//Helper functions for Quaternion Math
void MatrixToQuat(real32 mat[4][4], Quaternion *newquat);
void QuatToMatrix(real32 mat[4][4], Quaternion *oldquat);
void AngleToQuat(real32 angle,Vector3<real32> axis, Quaternion *newquat);
void EulerToQuat(real32 roll, real32 pitch, real32 yaw, Quaternion *quat);

#endif