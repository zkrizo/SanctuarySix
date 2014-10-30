#ifndef Ray_H
#define Ray_H

#include <math.h>
#include <stdlib.h>
#include "Vector.h"

template <typename T>
class Ray
{
public:
	Ray(Vector3<T> origin, Vector3<T> direction):orig(origin),dir(direction),tmin(T(0)),tmax(T(10000)){
		invdir.x=1/dir.x;
		invdir.y=1/dir.y;
		invdir.z=1/dir.z;
		sign[0]=(invdir.x<0);
		sign[1]=(invdir.y<0);
		sign[2]=(invdir.z<0);
	}
	Vector3<T> orig,dir;
	Vector3<T> invdir;
	T tmin, tmax;
	int sign[3];
};

template <typename T>
class RayCol
{
public:
	RayCol():dist(100000),face(0),blockCol(NULL),sOffset(0,0,0),rOffset(0,0,0){};

	real32 dist;
	s32 face;
	T* blockCol;
	Vector3<s64> sOffset;
	Vector3<real64> rOffset;
};
#endif