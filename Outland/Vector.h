#ifndef Vector_H
#define Vector_H

#include <math.h>

template <typename T>
class Vector2
{
public:
	Vector2<T>():x(0),y(0){};
	Vector2<T>(T a, T b):x(a),y(b){};
	T x,y;
	bool operator==(const Vector2<T>&b) const;
	bool operator!=(const Vector2<T>&b) const;
};

template <typename T>
bool Vector2<T>::operator==(const Vector2<T>&b) const
{
	return b.x==x && 
			b.y == y;
}

template <typename T>
bool Vector2<T>::operator!=(const Vector2<T>&b) const
{
	return b.x!=x || 
			b.y != y;
}

template <typename T>
class Vector3
{
public:
	Vector3<T>():x(0),y(0),z(0){};
	Vector3<T>(T a,T b, T c):x(a),y(b),z(c){};
	Vector3<T>(T var[3]):x(var[0]),y(var[1]),z(var[2]){};

	float dotProduct(const Vector3<T> &b);
	Vector3<T> crossProduct(const Vector3<T> &b);
	float length();
	void normalize();
	void scale(T val);

	Vector3<T> operator+ (const Vector3<T> &b) const;
	Vector3<T> operator- (const Vector3<T> &b) const;
	Vector3<T> operator/ (const Vector3<T> &b) const;
	Vector3<T> operator* (const Vector3<T> &b) const;
	void operator+=(const Vector3<T> &b);
	bool operator== (const Vector3<T> &b) const;
	bool operator!= (const Vector3<T> &b) const;

	T x,y,z;
};

template <typename T>
Vector3<T> Vector3<T>::operator*(const Vector3<T> &b) const
{
	Vector3<T> temp;

	temp.x=x*b.x;
	temp.y=y*b.y;
	temp.z=z*b.z;

	return temp;
}

template <typename T>
Vector3<T> Vector3<T>::operator/(const Vector3<T> &b) const
{
	Vector3<T> temp;

	temp.x=x/b.x;
	temp.y=y/b.y;
	temp.z=z/b.z;

	return temp;
}

template <typename T>
Vector3<T> Vector3<T>::operator+(const Vector3<T> &b) const
{
	Vector3<T> temp;

	temp.x=x+b.x;
	temp.y=y+b.y;
	temp.z=z+b.z;

	return temp;
}

template <typename T>
Vector3<T> Vector3<T>::operator-(const Vector3<T> &b) const
{
	Vector3<T> temp;

	temp.x=x-b.x;
	temp.y=y-b.y;
	temp.z=z-b.z;

	return temp;
}
template <typename T>
void Vector3<T>::operator+=(const Vector3<T> &b)
{
	this->x+=b.x;
	this->y+=b.y;
	this->z+=b.z;
}

template <typename T>
bool Vector3<T>::operator==(const Vector3<T> &b) const
{
	return
		b.x==x &&
		b.y==y &&
		b.z==z;
}

template <typename T>
bool Vector3<T>::operator!=(const Vector3<T> &b) const
{
	return
		b.x!=x ||
		b.y!=y ||
		b.z!=z;
}

template <typename T>
float Vector3<T>::dotProduct(const Vector3<T> &b)
{
	return(x*b.x+y*b.y+z*b.z);
}

template <typename T>
Vector3<T> Vector3<T>::crossProduct(const Vector3<T> &b)
{
	Vector3<T> temp;
	Vector3<T> temp2=b;
	
	//makes sure the vectors are normalized
	normalize();
	temp2.normalize();

	//calculates the cross product
	temp.x=y*temp2.z-z*temp2.y;
	temp.y=z*temp2.x-x*temp2.z;
	temp.z=x*temp2.y-y*temp2.x;

	//ensures the new vector is normalized
	temp.normalize();
	return temp;
}

template <typename T>
float Vector3<T>::length()
{
	return (sqrt(float(x*x+y*y+z*z)));
}

template <typename T>
void Vector3<T>::normalize()
{
	float len;
	len=length();
	if(len!=1)
	{
		x=x/len;
		y=y/len;
		z=z/len;
	}
}

template <typename T>
void Vector3<T>::scale(T val)
{
	this->x=x*val;
	this->y=y*val;
	this->z=z*val;
}

template <typename T>
Vector3<T> CrossProduct(const Vector3<T> &v1, const Vector3<T> &v2)
{
	Vector3<T> temp;

	//calculates the cross product
	temp.x=v1.y*v2.z-v1.z*v2.y;
	temp.y=v1.z*v2.x-v1.x*v2.z;
	temp.z=v1.x*v2.y-v1.y*v2.x;

	//ensures the new vector is normalized
	temp.normalize();

	return temp;	
}

template <typename T>
class Vector4
{
public:
	Vector4<T>():x(0),y(0),z(0),s(0){};
	Vector4<T>(T a,T b, T c, T d):x(a),y(b),z(c),s(d){};
	Vector4<T>(T var[4]):x(var[0]),y(var[1]),z(var[2]),s(var[3]){};

	float length();
	void normalize();

	Vector4<T> operator+ (const Vector4<T> &b);
	Vector4<T> operator- (const Vector4<T> &b);
	Vector4<T> operator/ (const Vector4<T> &b);
	Vector4<T> operator* (const Vector4<T> &b);
	bool operator== (const Vector4<T> &b);
	bool operator!= (const Vector4<T> &b);

	T x,y,z,s;
};

template <typename T>
Vector4<T> Vector4<T>::operator*(const Vector4<T> &b)
{
	Vector4<T> temp;

	temp.x=x*b.x;
	temp.y=y*b.y;
	temp.z=z*b.z;
	temp.s=s*b.s;

	return temp;
}

template <typename T>
Vector4<T> Vector4<T>::operator/(const Vector4<T> &b)
{
	Vector4<T> temp;

	temp.x=x/b.x;
	temp.y=y/b.y;
	temp.z=z/b.z;
	temp.s=s/b.s;

	return temp;
}

template <typename T>
Vector4<T> Vector4<T>::operator+(const Vector4<T> &b)
{
	Vector4<T> temp;

	temp.x=x+b.x;
	temp.y=y+b.y;
	temp.z=z+b.z;
	temp.s=s+b.s;

	return temp;
}

template <typename T>
Vector4<T> Vector4<T>::operator-(const Vector4<T> &b)
{
	Vector4<T> temp;

	temp.x=x-b.x;
	temp.y=y-b.y;
	temp.z=z-b.z;
	temp.s=s-b.s;

	return temp;
}

template <typename T>
bool Vector4<T>::operator==(const Vector4<T> &b)
{
	return
		b.x==x &&
		b.y==y &&
		b.z==z &&
		b.s==s;
}

template <typename T>
bool Vector4<T>::operator!=(const Vector4<T> &b)
{
	return
		b.x!=x ||
		b.y!=y ||
		b.z!=z ||
		b.s!=s;
}

template <typename T>
float Vector4<T>::length()
{
	return float((sqrt(x*x+y*y+z*z+s*s)));
}

template <typename T>
void Vector4<T>::normalize()
{
	float len;
	len=length();
	if(len!=1)
	{
		x=x/len;
		y=y/len;
		z=z/len;
		s=s/len;
	}
}


#endif