#ifndef BuildInfo_H
#define BuildInfo_H

#include <Outland\UnitAbstraction.h>
#include <Outland\Vector.h>
#include <Outland\Quaternion.h>
#include <string>

class BuildInfo
{
public:
	BuildInfo();
	BuildInfo(std::string description, s32 minSize1, s32 minSize2, s32 resourceName[4], s32 resourceCost[4], s32 techLevel, s32 TechsReq[4], s32 TechsLev[4], s8 Valid);

	std::string description;
	s32 minSize1;
	s32 minSize2;
	s32 techLevel;
	s32 resourceName[4];
	s32 resourceCost[4];
	s32 TechsReq[4];
	s32 TechsLev[4];
	s8 Valid;

	void setInfo(std::string description, s32 minSize1, s32 minSize2, s32 resourceName[4], s32 resourceCost[4], s32 techLevel, s32 TechsReq[4], s32 TechsLev[4], s8 Valid);

private:

};

#endif