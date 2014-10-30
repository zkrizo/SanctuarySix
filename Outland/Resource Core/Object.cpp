#include "Object.h"
#include <Outland\Singletons.h>
#include <Outland\Singletons.h>


/****************************
* Default Constructor
****************************/
Object::Object(const Vector3<real64>& npos,const std::string& filename):
	pos(npos),
	rotation(0)
{
	mesh.LoadMesh(filename);
	if(filename!="")
		GUILoad->incrementAndRender();
}

Object::Object(const Vector3<real64>& npos,const std::string& filename,const GLuint* diffuse,const GLuint* normals,const GLuint* bump):
	pos(npos),
	rotation(0)
{
	mesh.LoadMesh(filename);
	if(diffuse)
		mesh.AddTexture(diffuse);
	if(normals)
		mesh.AddTexture(normals);
	if(bump)
		mesh.AddTexture(bump);
	if(filename!="")
		GUILoad->incrementAndRender();
}
/****************************
* Render
****************************/
void Object::Render()
{
	glPushMatrix();
	glTranslatef(pos.x,pos.y,pos.z);
	glPushMatrix();
	glRotatef(rotation,0,1,0);
	mesh.Render();
	glPopMatrix();
	glPopMatrix();
}
