#include "Building.h"
#include <Outland\Singletons.h>

Building::Building():
	pos(0,0,0),
	walls(0),
	orient(0),
	roomType(0),
	wallType(0),
	doorPosition(0),
	cornerPosition(0),
	wallBitmask(Face_INVALID),
	doorBitmask(Face_INVALID),
	cornerBitmask(Face_INVALID)
{}

Building::Building(Vector3<s32> buildPos, int buildingType, int wallBit):
	pos(buildPos),
	walls(0),
	orient(0),
	roomType(buildingType),
	wallType(0),
	doorPosition(0),
	cornerPosition(0),
	wallBitmask(wallBit),
	doorBitmask(Face_INVALID),
	cornerBitmask(Face_INVALID)
{
	calculateWallDataFromBitmask();
}

/****************************
* Render
****************************/
void Building::Render(tBOOL powered)
{
	//calls the OpenGLRenderer test function for singular buildings. 
	grCore->pRenderer->RenderTestBuilding((*this),powered);
}

/****************************
* setPos
****************************/
void Building::setPos(Vector3<s32> newPos)
{
	pos=newPos;
}

/****************************
* setWalls
****************************/
void Building::setWalls(s32 wall)
{
	if(wall<5 && wall>=0)
	{
		walls=wall;
	}
	else
	{
		walls=4;
	}
}

/****************************
* setOrient
****************************/
void Building::setOrient(s32 ori)
{
	if(ori%90==0)
	{
		orient=ori;
	}
	else if(ori<=3 && ori>=-3)
	{
		orient=ori*90;
	}
}

/****************************
* setRoomType
****************************/
void Building::setRoomType(s32 type)
{
	roomType=type;
}

/****************************
* setWallType
****************************/
void Building::setWallType(s32 type)
{
	if(type==0 || type==1)
	{
		wallType=type;
	}
	else
	{
		//if passed an invalid number, type will be set to regular
		wallType=0;
	}
}

/****************************
* setInfo
****************************/
void Building::setInfo(Vector3<s32> newpos, s32 wall, s32 ori, s32 rType, s32 wType)
{
	pos=newpos;
	walls=wall;
	orient=ori;
	roomType=rType;
	wallType=wType;
}

/****************************
* setDoor
****************************/
void Building::setDoor(s32 doors)
{
	doorPosition=doors;
}

/****************************
* addDoor
****************************/
void Building::addDoor(s32 doors)
{
	switch(doorPosition)
	{
	case RM_DOOR0:
		doorPosition=doors;
		break;
	case RM_DOOR8:
		switch(doors)
		{
		case RM_DOOR2:
			doorPosition=RM_DOOR82;
			break;
		case RM_DOOR6:
			doorPosition=RM_DOOR86;
			break;
		case RM_DOOR4:
			doorPosition=RM_DOOR84;
			break;
		case RM_DOOR62:
			doorPosition=RM_DOOR862;
			break;
		case RM_DOOR24:
			doorPosition=RM_DOOR824;
			break;
		case RM_DOOR64:
			doorPosition=RM_DOOR64;
			break;
		case RM_DOOR624:
			doorPosition=RM_DOOR8624;
			break;
		}
		break;
	case RM_DOOR86:
		switch(doors)
		{
		case RM_DOOR2:
			doorPosition=RM_DOOR862;
			break;
		case RM_DOOR4:
			doorPosition=RM_DOOR864;
			break;
		case RM_DOOR24:
			doorPosition=RM_DOOR8624;
			break;
		}
		break;
	case RM_DOOR862:
		if(doors==RM_DOOR4)
			doorPosition=RM_DOOR8624;
		break;
	case RM_DOOR864:
		if(doors==RM_DOOR2)
			doorPosition=RM_DOOR8624;
		break;
	case RM_DOOR82:
		switch(doors)
		{
		case RM_DOOR6:
			doorPosition=RM_DOOR862;
			break;
		case RM_DOOR4:
			doorPosition=RM_DOOR824;
			break;
		case RM_DOOR64:
			doorPosition=RM_DOOR8624;
			break;
		}
		break;
	case RM_DOOR824:
		if(doors==RM_DOOR6)
			doorPosition=RM_DOOR8624;
		break;
	case RM_DOOR84:
		switch(doors)
		{
		case RM_DOOR6:
			doorPosition=RM_DOOR864;
			break;
		case RM_DOOR2:
			doorPosition=RM_DOOR824;
			break;
		case RM_DOOR62:
			doorPosition=RM_DOOR8624;
			break;
		}
		break;
	case RM_DOOR6:
		switch(doors)
		{
		case RM_DOOR8:
			doorPosition=RM_DOOR86;
			break;
		case RM_DOOR2:
			doorPosition=RM_DOOR62;
			break;
		case RM_DOOR4:
			doorPosition=RM_DOOR64;
			break;
		case RM_DOOR82:
			doorPosition=RM_DOOR862;
			break;
		case RM_DOOR84:
			doorPosition=RM_DOOR864;
			break;
		case RM_DOOR24:
			doorPosition=RM_DOOR624;
			break;
		case RM_DOOR824:
			doorPosition=RM_DOOR8624;
			break;
		}
		break;
	case RM_DOOR62:
		switch(doors)
		{
		case RM_DOOR8:
			doorPosition=RM_DOOR862;
			break;
		case RM_DOOR4:
			doorPosition=RM_DOOR624;
			break;
		case RM_DOOR84:
			doorPosition=RM_DOOR8624;
			break;
		}
		break;
	case RM_DOOR624:
		if(doors==RM_DOOR8)
			doorPosition=RM_DOOR8624;
		break;
	case RM_DOOR64:
		switch(doors)
		{
		case RM_DOOR8:
			doorPosition=RM_DOOR864;
			break;
		case RM_DOOR2:
			doorPosition=RM_DOOR624;
			break;
		case RM_DOOR82:
			doorPosition=RM_DOOR8624;
			break;
		}
		break;
	case RM_DOOR2:
		switch(doors)
		{
		case RM_DOOR8:
			doorPosition=RM_DOOR82;
			break;
		case RM_DOOR6:
			doorPosition=RM_DOOR62;
			break;
		case RM_DOOR4:
			doorPosition=RM_DOOR24;
			break;
		case RM_DOOR86:
			doorPosition=RM_DOOR862;
			break;
		case RM_DOOR84:
			doorPosition=RM_DOOR824;
			break;
		case RM_DOOR64:
			doorPosition=RM_DOOR624;
			break;
		case RM_DOOR864:
			doorPosition=RM_DOOR8624;
			break;
		}
		break;
	case RM_DOOR24:
		switch(doors)
		{
		case RM_DOOR8:
			doorPosition=RM_DOOR824;
			break;
		case RM_DOOR6:
			doorPosition=RM_DOOR624;
			break;
		case RM_DOOR86:
			doorPosition=RM_DOOR8624;
			break;
		}
		break;
	case RM_DOOR4:
		switch(doors)
		{
		case RM_DOOR8:
			doorPosition=RM_DOOR84;
			break;
		case RM_DOOR6:
			doorPosition=RM_DOOR64;
			break;
		case RM_DOOR2:
			doorPosition=RM_DOOR24;
			break;
		case RM_DOOR86:
			doorPosition=RM_DOOR864;
			break;
		case RM_DOOR82:
			doorPosition=RM_DOOR824;
			break;
		case RM_DOOR62:
			doorPosition=RM_DOOR624;
			break;
		case RM_DOOR862:
			doorPosition=RM_DOOR8624;
			break;
		}
		break;
	}
}

/****************************
* removeDoor
****************************/
void Building::removeDoor(s32 doors)
{
switch(doorPosition)
	{
	case RM_DOOR8: 
		if(doors==RM_DOOR8)
			doorPosition=RM_DOOR0;
		break;
	case RM_DOOR86: 
		switch(doors)
		{
		case RM_DOOR8:
			doorPosition=RM_DOOR6;
			break;
		case RM_DOOR6:
			doorPosition=RM_DOOR8;
			break;
		case RM_DOOR86:
			doorPosition=RM_DOOR0;
			break;
		}
		break;
	case RM_DOOR862: 
		switch(doors)
		{
		case RM_DOOR8:
			doorPosition=RM_DOOR62;
			break;
		case RM_DOOR86:
			doorPosition=RM_DOOR2;
			break;
		case RM_DOOR82:
			doorPosition=RM_DOOR6;
			break;
		case RM_DOOR862:
			doorPosition=RM_DOOR0;
			break;
		case RM_DOOR6:
			doorPosition=RM_DOOR82;
			break;
		case RM_DOOR62:
			doorPosition=RM_DOOR8;
			break;
		case RM_DOOR2:
			doorPosition=RM_DOOR86;
			break;
		}
	case RM_DOOR864:
		switch(doors)
		{
		case RM_DOOR864:
			doorPosition=RM_DOOR0;
			break;
		case RM_DOOR8:
			doorPosition=RM_DOOR64;
			break;
		case RM_DOOR86:
			doorPosition=RM_DOOR4;
			break;
		case RM_DOOR84:
			doorPosition=RM_DOOR6;
			break;
		case RM_DOOR6:
			doorPosition=RM_DOOR84;
			break;
		case RM_DOOR64:
			doorPosition=RM_DOOR8;
			break;
		case RM_DOOR4:
			doorPosition=RM_DOOR86;
			break;
		}
		break;
	case RM_DOOR8624: 
		switch(doors)
		{
		case RM_DOOR8: 
			doorPosition=RM_DOOR624;
			break;
		case RM_DOOR86: 
			doorPosition=RM_DOOR24;
			break;
		case RM_DOOR862: 
			doorPosition=RM_DOOR4;
			break;
		case RM_DOOR8624: 
			doorPosition=RM_DOOR0;
			break;
		case RM_DOOR82: 
			doorPosition=RM_DOOR64;
			break;
		case RM_DOOR824: 
			doorPosition=RM_DOOR6;
			break;
		case RM_DOOR84: 
			doorPosition=RM_DOOR62;
			break;
		case RM_DOOR6: 
			doorPosition=RM_DOOR824;
			break;
		case RM_DOOR62: 
			doorPosition=RM_DOOR84;
			break;
		case RM_DOOR624: 
			doorPosition=RM_DOOR8;
			break;
		case RM_DOOR64: 
			doorPosition=RM_DOOR82;
			break;
		case RM_DOOR2: 
			doorPosition=RM_DOOR864;
			break;
		case RM_DOOR24: 
			doorPosition=RM_DOOR86;
			break;
		case RM_DOOR4: 
			doorPosition=RM_DOOR862;
			break;
		}
		break;
	case RM_DOOR82: 
		switch(doors)
		{
		case RM_DOOR82:
			doorPosition=RM_DOOR0;
			break;
		case RM_DOOR8:
			doorPosition=RM_DOOR2;
			break;
		case RM_DOOR2:
			doorPosition=RM_DOOR8;
			break;
		}
		break;
	case RM_DOOR824: 
		switch(doors)
		{
		case RM_DOOR824:
			doorPosition=RM_DOOR0;
			break;
		case RM_DOOR82:
			doorPosition=RM_DOOR4;
			break;
		case RM_DOOR84:
			doorPosition=RM_DOOR2;
			break;
		case RM_DOOR24:
			doorPosition=RM_DOOR8;
			break;
		case RM_DOOR8:
			doorPosition=RM_DOOR24;
			break;
		case RM_DOOR2:
			doorPosition=RM_DOOR84;
			break;
		case RM_DOOR4:
			doorPosition=RM_DOOR82;
			break;
		}
		break;
	case RM_DOOR84: 
		switch(doors)
		{
		case RM_DOOR84:
			doorPosition=RM_DOOR0;
			break;
		case RM_DOOR8:
			doorPosition=RM_DOOR4;
			break;
		case RM_DOOR4:
			doorPosition=RM_DOOR8;
			break;
		}
		break;
	case RM_DOOR6: 
		if(doors==RM_DOOR6)
			doorPosition=RM_DOOR0;
		break;
	case RM_DOOR62: 
		switch(doors)
		{
		case RM_DOOR62:
			doorPosition=RM_DOOR0;
			break;
		case RM_DOOR6:
			doorPosition=RM_DOOR2;
			break;
		case RM_DOOR2:
			doorPosition=RM_DOOR6;
			break;
		}
		break;
	case RM_DOOR624: 
		switch(doors)
		{
		case RM_DOOR624:
			doorPosition=RM_DOOR0;
			break;
		case RM_DOOR62:
			doorPosition=RM_DOOR4;
			break;
		case RM_DOOR64:
			doorPosition=RM_DOOR2;
			break;
		case RM_DOOR24:
			doorPosition=RM_DOOR6;
			break;
		case RM_DOOR6:
			doorPosition=RM_DOOR24;
			break;
		case RM_DOOR2:
			doorPosition=RM_DOOR64;
			break;
		case RM_DOOR4:
			doorPosition=RM_DOOR62;
			break;
		}
		break;
	case RM_DOOR64: 
		switch(doors)
		{
		case RM_DOOR64:
			doorPosition=RM_DOOR0;
			break;
		case RM_DOOR6:
			doorPosition=RM_DOOR4;
			break;
		case RM_DOOR4:
			doorPosition=RM_DOOR6;
			break;
		}
		break;
	case RM_DOOR2: 
		if(doors==RM_DOOR2)
			doorPosition=RM_DOOR0;
		break;
	case RM_DOOR24: 
		switch(doors)
		{
		case RM_DOOR24:
			doorPosition=RM_DOOR0;
			break;
		case RM_DOOR2:
			doorPosition=RM_DOOR4;
			break;
		case RM_DOOR4:
			doorPosition=RM_DOOR2;
			break;
		}
		break;
	case RM_DOOR4: 
		if(doors==RM_DOOR4)
			doorPosition=RM_DOOR0;
		break;
	default:
		break;
	}
}

/****************************
* setCorner
****************************/
void Building::setCorner(s32 corners)
{
	cornerPosition=corners;
}

/****************************
* addCorner
****************************/
void Building::addCorner(s32 corners)
{
	cornerPosition|=corners;
}

/****************************
* removeCorner
****************************/
void Building::removeCorner(s32 corners)
{
	if((cornerPosition&corners)==corners)
	{
		cornerPosition-=corners;
	}
}


void Building::setWall(s32 walls)
{
	wallBitmask=walls;
}

void Building::addWall(s32 walls)
{

	if(wallBitmask==Face_INVALID)
		wallBitmask=walls;
	else
		wallBitmask=wallBitmask|walls;
}

void Building::removeWall(s32 walls)
{
	if((wallBitmask&walls)==walls)
	{
		wallBitmask-=walls;
		calculateWallDataFromBitmask();
	}
}

void Building::calculateWallDataFromBitmask()
{
	switch(wallBitmask)
	{
	case 0:
		break;
	case 4:
		walls=1;
		orient=90;
		break;
	case 8:
		walls=1;
		orient=270;
		break;
	case 12:
		walls=2;
		wallType=1;
		orient=90;
		break;
	case 16:
		walls=1;
		break;
	case 20:
		walls=2;
		break;
	case 24:
		walls=2;
		orient=270;
		break;
	case 28:
		walls=3;
		orient=270;
		break;
	case 32:
		walls=1;
		orient=180;
		break;
	case 36:
		walls=2;
		orient=90;
		break;
	case 40:
		walls=2;
		orient=180;
		break;
	case 44:
		walls=3;
		orient=90;
		break;
	case 48:
		walls=2;
		wallType=1;
		break;
	case 52:
		walls=3;
		orient=180;
		break;
	case 56:
		walls=3;
		break;
	case 60:
		walls=4;
		break;
	}
}