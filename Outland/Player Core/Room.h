#ifndef Room_H
#define Room_H

#include <Outland\UnitAbstraction.h>
#include <Outland\Vector.h>
#include <Outland\Quaternion.h>
#include <Outland\Graphics Core\OpenGLRenderer.h>
#include "Building.h"
#include <vector>


class Room
{
public:
	Room();
	~Room();
	void addBlock(const Building& block);
	void removeBlock(const Building& block);
	void removeBlock(const Vector3<s64>& pos);
	void Render();
	void alterMergeBlock(const s32& xplus, const s32& xminus, const s32& zplus, const s32& zminus,const Vector3<s64>& pos, const tBOOL& add);
	void alterMergeBlock(const int& faceData, const Vector3<s64>& pos, const bool& add);
	void recalcPower();
	void recalcMaxMin();
	void addDoor(const Vector3<s64>& pos,const s32& direction);
	void rayTrace(const Ray<real32>& ray, RayCol<Building> &rayBuilding);
	void SaveAll(FILE *file);

	void setPos(const Vector3<s64>& newPos){pos=newPos;};
	void setPower(const tBOOL& powerStatus){powered=powerStatus;};

	Vector3<s64> getPos() const{return pos;};
	s32 getMinX() const{return minX;};
	s32 getMinY() const{return minY;};
	s32 getMinZ() const{return minZ;};
	s32 getMaxX() const{return maxX;};
	s32 getMaxY() const{return maxY;};
	s32 getMaxZ() const{return maxZ;};
	s32 getType() const{return blocks[0].getRoomType();};
	float getPower() const{return power;};
	tBOOL isPowered() const{return powered;};
	std::vector<Building> getBlocks() const{return blocks;};

	tBOOL checkPos(const Vector3<s64>& cpos);
	void updateCorners();
private:
	Vector3<s64> pos;
	std::vector<Building> blocks;	
	s32 minX;
	s32 minY;
	s32 maxX;
	s32 maxY;
	s32 minZ;
	s32 maxZ;
	float power;
	tBOOL powered;
	Building* const getBuilding(const Vector3<s32> &pos);
};

#endif