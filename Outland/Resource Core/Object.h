#ifndef Object_H
#define Object_H

#include <Outland\UnitAbstraction.h>
#include <Outland\Vector.h>
#include "Mesh.h"

class Object
{
public:
	Object(const Vector3<real64>& npos,const std::string& filename);
	Object(const Vector3<real64>& npos,const std::string& file,const GLuint* diffuse,const GLuint* normals,const GLuint* bump);
	Vector3<real64> getPos(){return pos;};
	real32 getRotation(){return rotation;};
	void setPos(const Vector3<real64>& npos){pos=npos;};
	void setRotation(const real32& rot){rotation=rot;};
	virtual void Render();
protected:
	Vector3<real64> pos;
	real32 rotation;
	Mesh mesh;
};

#endif