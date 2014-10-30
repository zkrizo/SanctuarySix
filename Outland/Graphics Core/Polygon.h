#ifndef Poly_H
#define Poly_H

#include <vector>
#include <Outland\UnitAbstraction.h>
#include <Outland\Vector.h>

class Poly
{
public:
	Poly(const Vector3<s32>& point1,const Vector3<s32>& point2,const Vector3<s32>& point3,const Vector3<s32>& point4,const int& polyType); 
	std::vector<Vector3<s32>> getPoints() const {return points;};
	std::vector<Vector2<s32>> getUVs() const {return uvs;};
	Vector3<s32> getNormal() const {return normal;};
	int getType() const {return type;};

	Vector3<s32> getPoint(const int& point) const;
	Vector2<s32> getUV(const int& uv) const;
	bool operator== (const Poly& poly);

	void renderPoly();
private:
	std::vector<Vector3<s32>> points;
	std::vector<Vector2<s32>> uvs;
	Vector3<s32> normal;
	int type;
	void calcNormal(const Vector3<s32>& point1, const Vector3<s32>& point2);
};

#endif