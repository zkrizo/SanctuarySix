#ifndef BuildingManage_H
#define BuildingManage_H

#include <Outland\UnitAbstraction.h>
#include <Outland\Vector.h>
#include <Outland\Quaternion.h>
#include "Room.h"
#include "BuildInfo.h"
#include <Outland\Resource Core\Bot.h>
#include <Outland\Ray.h>

class BuildingManage
{
public:
	BuildingManage();
	~BuildingManage();
	tBOOL checkMergeRooms(Vector3<s64> pos1, s32 type);
	s32 checkNearbyRooms(Vector3<s64> pos1, s32 type);
	s32 checkRooms(const Vector3<s64>& pos1,const s32& type);
	tBOOL checkNearbyRooms(Vector3<s64> pos1, Vector3<s64> pos2, s32 type);
	void BuildRect(Vector3<s64> pos1, Vector3<s64> pos2, s32 type, tBOOL merge, bool ghosted);
	void RemoveRect(Vector3<s64> pos1, Vector3<s64> pos2);
	void Render();
	void addDoor(const Vector3<s64>& pos,const s32 direction);
	void rayTrace(const Ray<real32>& ray, RayCol<Building> &rayBuilding);
	void clearGhostRoom(){if(ghostRoom){ delete ghostRoom;ghostRoom=NULL;}};
	real32 powerUpdate(real32 curpower);

	void SaveAllBuildings();
	void LoadAllBuildings();
	void buildRoomGenerateGraph(const Vector3<s64> &firstPos, const Vector3<s64> &secondPos,const int &type, const bool &merge, const bool &ghosted);
	
	void alterMergeBlock(const int& faceData, const Vector3<s64>& pos, const bool& add);

	std::vector<Room> Rooms;

	BuildInfo *T1Airlock;
	BuildInfo *T1Bathrooms;
	BuildInfo *T1CommandCenter;
	BuildInfo *T1FreightAirlock;
	BuildInfo *T1Galley;
	BuildInfo *T1Hallway;
	BuildInfo *T1IndustrialWorkshop;
	BuildInfo *T1Laboratory;
	BuildInfo *T1Library;
	BuildInfo *T1LivingQuarters;
	BuildInfo *T1MiningControlCenter;
	BuildInfo *T1OxygenScrubber;
	BuildInfo *T1PowerFusion;
	BuildInfo *T1Warehouse;
	BuildInfo *T1WaterCollector;
	BuildInfo *T1WaterRecycling;
private:
	//vector index value to edit vectors
	std::vector<Room*> interRooms;
	std::vector<Room*> mergeRooms;
	void clearInterRooms();
	void mergeExistingRooms();
	void alterMergeBlock(s32 xplus, s32 xminus, s32 zplus, s32 zminus,Vector3<s64> pos, tBOOL add);
	void removeRoom(Vector3<s64> pos);
	Room *ghostRoom;
	void mergeExistingRooms(const Room& newRoom);

	void buildRoomFromGraph(const Vector3<s64> &posMin, const Vector3<s64> &posMax/*BuildGraph buildingGraph*/);
	int generateRoomMask(const Vector3<s32>& current, const Vector3<s64>& graphSize);

	//Temporary test variables
	Room temp;
	Building block;
	
};

#endif