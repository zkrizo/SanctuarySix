#include "Quaternion.h"
#include <math.h>

/****************************
* Default Constructor
****************************/
Quaternion::Quaternion():w(1),x(0),y(0),z(0){}

/****************************
* Data Constructor
****************************/
Quaternion::Quaternion(real32 a, real32 b, real32 c, real32 d):w(a),x(b),y(c),z(d){}

/****************************
* Normalize - Only called if the tolerance has been passed
****************************/
Quaternion Quaternion::Normalize()
{
	Quaternion temp;
	real32 len=sqrt(w*w+x*x+y*y+z*z);
	if(len!=1)
	{
		temp.w=w/len;
		temp.x=x/len;
		temp.y=y/len;
		temp.z=z/len;
	}

	return temp;
}

Quaternion Quaternion::getConjugate()
{
	Quaternion temp;

	temp.x=-x;
	temp.y=-y;
	temp.z=-z;
	temp.w=w;

	return temp;
}

/****************************
* Operator *
****************************/
Quaternion Quaternion::operator*(Quaternion &b)
{
	Quaternion temp;

	//check for tolerance fault on both quaternions
	real32 len1=w*w+x*x+y*y+z*z;
	real32 len2=b.w*b.w+b.x*b.x+b.y*b.y+b.z*b.z;

	if(abs(len1-1)>QUATERNION_TOLERANCE)
	{
		Normalize();
	}
	if(abs(len2-1)>QUATERNION_TOLERANCE)
	{
		b.Normalize();
	}

	//Multiplies using quaternion mathematics
	temp.w=(w*b.w-x*b.x-y*b.y-z*b.z);
	temp.x=(w*b.x+x*b.w+y*b.z-z*b.y);
	temp.y=(w*b.y-x*b.z+y*b.w+z*b.x);
	temp.z=(w*b.z+x*b.y-y*b.x+z*b.w);

	return temp;
}

/****************************
* Operator +
****************************/
Quaternion Quaternion::operator+(Quaternion &b)
{
	Quaternion temp;

	//check for tolerance fault on both quaternions
	real32 len1=w*w+x*x+y*y+z*z;
	real32 len2=b.w*b.w+b.x*b.x+b.y*b.y+b.z*b.z;

	if(abs(len1-1)>QUATERNION_TOLERANCE)
	{
		Normalize();
	}
	if(abs(len2-1)>QUATERNION_TOLERANCE)
	{
		b.Normalize();
	}

	temp.w=w+b.w;
	temp.x=x+b.x;
	temp.y=y+b.y;
	temp.z=z+b.z;

	return temp;
}


/**********************************************
***********************************************
* Helper Functions for Quaternion Mathematics *
***********************************************
**********************************************/

/****************************
* Matrix to Quaternion Conversion
****************************/
void MatrixToQuat(real32 m[4][4],Quaternion *quat)
{
	real32 trans,s,q[4];
	s32 i,j,k;
	s32 next[3]={1,2,0};
	trans=m[0][0]+m[1][1]+m[2][2];

	//check diagonal data;
	if(trans>0)
	{
		s=sqrt(trans+1);
		quat->w=s/2;
		s=(real32)0.5/s;
		quat->x=(m[1][2]-m[2][1])*s;
		quat->y=(m[2][0]-m[0][2])*s;
		quat->z=(m[0][1]-m[1][0])*s;
	}
	else
	{
		//diagonal data is negative
		i=0;

		//finds the greatest matrix value
		if(m[1][1]>m[0][0])
		{
			i=1;
		}
		if(m[2][2]>m[i][i])
		{
			i=2;
		}

		j=next[i];
		k=next[j];
		s=sqrt((m[i][i]-(m[j][j]+m[k][k]))+1);
		q[i]=s*(real32)0.5;
		
		if(s!=0)
		{
			s=(real32)0.5/s;
		}
		
		q[3]=(m[j][k]-m[k][j])*s;
		q[j]=(m[i][j]+m[j][i])*s;
		q[k]=(m[i][k]+m[k][i])*s;

		quat->x=q[0];
		quat->y=q[1];
		quat->z=q[2];
		quat->w=q[3];
	}
}

/****************************
* Quaternion to Matrix Conversion
****************************/
void QuatToMatrix(real32 m[4][4],Quaternion *quat)
{
    float wx, wy, wz, xx, yy, yz, xy, xz, zz, x2, y2, z2;
	
    // calculate coefficients
    x2 = quat->x + quat->x; y2 = quat->y + quat->y;
    z2 = quat->z + quat->z;
    xx = quat->x * x2; xy = quat->x * y2; xz = quat->x * z2;
    yy = quat->y * y2; yz = quat->y * z2; zz = quat->z * z2;
    wx = quat->w * x2; wy = quat->w * y2; wz = quat->w * z2;
	
    m[0][0] =(real32) 1.0 - (yy + zz); 
	m[1][0] = xy - wz;
    m[2][0] = xz + wy; 
	m[3][0] = 0.0;

    m[0][1] = xy + wz; 
	m[1][1] =(real32) 1.0 - (xx + zz);
    m[2][1] = yz - wx; 
	m[3][1] = 0.0;
	
    m[0][2] = xz - wy; 
	m[1][2] = yz + wx;
    m[2][2] =(real32) 1.0 - (xx + yy); 
	m[3][2] = 0.0;
	
    m[0][3] = 0; 
	m[1][3] = 0;
    m[2][3] = 0; 
	m[3][3] = 1;
}

/****************************
* Angle to Quaternion Conversion
****************************/
void AngleToQuat(real32 angle,Vector3<real32> axis, Quaternion *newquat)
{
	newquat->w=cos((angle/2));
	newquat->x=sin(angle/2)*axis.x;
	newquat->y=sin(angle/2)*axis.y;
	newquat->z=sin(angle/2)*axis.z;
}

/****************************
* Euler to Quaternion Conversion
****************************/
void EulerToQuat(float roll, float pitch, float yaw, Quaternion *quat)
{
	float cr, cp, cy, sr, sp, sy, cpcy, spsy;

	//Convert degrees to radians
	roll=(float)(roll*3.14159/180);
	pitch=(float)(pitch*3.14159/180);
	yaw=(float)(yaw*3.14159/180);

	// calculate trig identities
	cr = cos(roll/2);
	cp = cos(pitch/2);
	cy = cos(yaw/2);
	sr = sin(roll/2);
	sp = sin(pitch/2);
	sy = sin(yaw/2);
	cpcy = cp * cy;
	spsy = sp * sy;
	
	//Save calculations to the quaternion
	quat->w = cr * cpcy + sr * spsy;
	quat->x = sr * cpcy - cr * spsy;
	quat->y = cr * sp * cy + sr * cp * sy;
	quat->z = cr * cp * sy - sr * sp * cy;
}