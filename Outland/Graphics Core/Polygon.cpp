#include "Polygon.h"
#include <gl\glew.h>
#include <gl\GLU.h>
#include <Outland\Singletons.h>


Poly::Poly(const Vector3<s32>& point1,const Vector3<s32>& point2,const Vector3<s32>& point3,const Vector3<s32>& point4,const int& polyType):
	type(polyType)
{
	points.push_back(point1);
	points.push_back(point2);
	points.push_back(point3);
	points.push_back(point4);

	int sizeX(abs(point3.x-point1.x)),sizeY(abs(point3.y-point1.y)),sizeZ(abs(point3.z-point1.z));

	calcNormal(Vector3<s32>(point4.x-point1.x,point4.y-point1.y,point4.z-point1.z),Vector3<s32>(point2.x-point1.x,point2.y-point1.y,point2.z-point1.z));

	uvs.push_back(Vector2<s32>(0,0));

	if(normal.x!=0)
	{
		uvs.push_back(Vector2<s32>(sizeZ,0));
		uvs.push_back(Vector2<s32>(sizeZ,sizeY));
		uvs.push_back(Vector2<s32>(0,sizeY));
	}
	else if(normal.y!=0)
	{
		uvs.push_back(Vector2<s32>(sizeZ,0));
		uvs.push_back(Vector2<s32>(sizeZ,sizeX));
		uvs.push_back(Vector2<s32>(0,sizeX));
	}
	else
	{
		uvs.push_back(Vector2<s32>(sizeX,0));
		uvs.push_back(Vector2<s32>(sizeX,sizeY));
		uvs.push_back(Vector2<s32>(0,sizeY));
	}
}

void Poly::calcNormal(const Vector3<s32>& point1, const Vector3<s32>& point2)
{
	Vector3<s32> temp=point2;
	normal=temp.crossProduct(point1);
}

Vector3<s32> Poly::getPoint(const int& point) const
{
	if(point>=0 && point<4)
		return points[point];
	else
		return Vector3<s32>(255,255,255);
}

Vector2<s32> Poly::getUV(const int& uv) const
{
	if(uv>=0 && uv<4)
		return uvs[uv];
	else
		return Vector2<s32>(255,255);
}

bool Poly::operator== (const Poly& poly)
{
	return(
		poly.getPoint(0)==points[0] &&
		poly.getPoint(1)==points[1] &&
		poly.getPoint(2)==points[2] &&
		poly.getPoint(3)==points[3] &&
		poly.getNormal()==normal &&
		poly.getUV(0)==uvs[0] &&
		poly.getUV(1)==uvs[1] &&
		poly.getUV(2)==uvs[2] &&
		poly.getUV(3)==uvs[3] &&
		poly.getType() == type
		);
}

void Poly::renderPoly()
{
	grCore->pRenderer->setBlockTexture(type);
	glBegin(GL_QUADS);
		glNormal3f(normal.x,normal.y,normal.z);
		for(int i(0);i<4;++i)
		{
			glTexCoord2i(uvs[i].x,uvs[i].y);	glVertex3i(points[i].x,points[i].y,points[i].z);
		}
	glEnd();
}