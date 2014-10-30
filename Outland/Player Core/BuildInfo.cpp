#include "BuildInfo.h"


/****************************
* Default Constructor
****************************/
BuildInfo::BuildInfo():
	description("NULL"),
	minSize1(0),
	minSize2(0),
	techLevel(0),
	Valid(0)
{	
	for(s32 i=0;i<4;i++)
	{
		TechsReq[i]= 0;
		TechsLev[i]= 0;
		resourceName[i]= 0;
		resourceCost[i]= 0;
	}
}


/****************************
* Info Constructor
****************************/
BuildInfo::BuildInfo(std::string desc, s32 Size1, s32 Size2, s32 rName[4], s32 rCost[4], s32 tLev, s32 TReq[4], s32 TLev[4], s8 Val):
	description(desc),
	minSize1(Size1),
	minSize2(Size2),
	techLevel(tLev),
	Valid(Val)
{	
	for(s32 i=0;i<4;i++)
	{
		TechsReq[i]= TReq[i];
		TechsLev[i]= TLev[i];
		resourceName[i]= rName[i];
		resourceCost[i]= rCost[i];
	}
}


/****************************
* SetInfo - performs the same job as the info constructor -
			sets up the variables with valid information.
****************************/
void BuildInfo::setInfo(std::string desc, s32 Size1, s32 Size2, s32 rName[4], s32 rCost[4], s32 tLev, s32 TReq[4], s32 TLev[4], s8 Val)
{
	description= desc;
	minSize1= Size1;
	minSize2= Size2;
	techLevel= tLev;
	Valid= Val;
	
	for(s32 i=0;i<4;i++)
	{
		TechsReq[i]= TReq[i];
		TechsLev[i]= TLev[i];
		resourceName[i]= rName[i];
		resourceCost[i]= rCost[i];
	}
}