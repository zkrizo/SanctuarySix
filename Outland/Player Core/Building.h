#ifndef Building_H
#define Building_H

/*
	This class is for a singular room block within a room. Each room will contain several of these
	objects to form a complete room.
*/

#include <Outland\UnitAbstraction.h>
#include <Outland\Vector.h>
#include <Outland\Quaternion.h>


class Building
{
public:
	Building();
	Building(Vector3<s32> buildPos, int buildingType, int wallBitmask);
	void Render(tBOOL powered);
	void setPos(Vector3<s32> newPos);
	void setWalls(s32 walls);
	void setOrient(s32 orient);
	void setRoomType(s32 type);
	void setWallType(s32 type);
	void setInfo(Vector3<s32> newpos, s32 walls, s32 orient, s32 roomType, s32 wallType);
	void setDoor(s32 doors);
	void addDoor(s32 doors);
	void removeDoor(s32 doors);
	void setCorner(s32 corners);
	void addCorner(s32 corners);
	void removeCorner(s32 corners);
	void setWall(s32 walls);
	void addWall(s32 walls);
	void removeWall(s32 walls);
	Vector3<s32> getPos() const {return pos;};
	s32 getWalls() const {return walls;};
	s32 getOrient() const {return orient;};
	s32 getRoomType() const {return roomType;};
	s32 getWallType() const {return wallType;};
	s32 getCornerPosition() const {return cornerPosition;};
	s32 getDoorPosition() const {return doorPosition;};
	s32 getWallBitmask() const {return wallBitmask;};
	s32 getDoorBitmask() const {return doorBitmask;};
	s32 getCornerBitmask() const {return cornerBitmask;};

private:
	Vector3<s32> pos;
	s32 walls;
	s32 orient;
	s32 roomType;
	s32 wallType;
	s32 doorPosition;
	s32 cornerPosition;

	//New bitmasks to replace doorPosition and cornerPosition:
	s32 wallBitmask;
	s32 doorBitmask;
	s32 cornerBitmask;

	void calculateWallDataFromBitmask();
};

#endif