#include "BuildingGraph.h"
#include <math.h>
#include <Outland\Singletons.h>

BuildingGraph::BuildingGraph(const Vector3<s64> &startPos, const Vector3<s64> &endPos, const int &type, const bool &mergeEn, const bool &ghost):
							size( abs(startPos.x-endPos.x)+3 , abs(startPos.y-endPos.y)+3, abs(startPos.z-endPos.z)+3),
							blockType(type),
							mergeEnabled(mergeEn),
							ghosted(ghost),
							merge(false)
{
//generate dynamic memory to store the graph in
	graphData=new int**[size.x];

	for(int xVal(0);xVal<size.x;++xVal)
	{
		graphData[xVal]=new int*[size.y];

		for(int yVal(0);yVal<size.y;++yVal)
		{
			graphData[xVal][yVal]=new int[size.z];
		}
	}

	//Calculate the min positions
	minPos.x=startPos.x<endPos.x? startPos.x:endPos.x;
	minPos.y=startPos.y<endPos.y? startPos.y:endPos.y;
	minPos.z=startPos.z<endPos.z? startPos.z:endPos.z;

	generateData();


}

BuildingGraph::~BuildingGraph()
{
	for(int xVal(0);xVal<size.x;++xVal)
	{
		for(int yVal(0);yVal<size.y;++yVal)
		{
			delete [] graphData[xVal][yVal];
		}
		delete [] graphData[xVal];
	}
	delete [] graphData;
	graphData=NULL;
}

void BuildingGraph::generateData()
{
	for(int xVal(0);xVal<size.x;++xVal)
	{
		for(int yVal(0);yVal<size.y;++yVal)
		{
			for(int zVal(0);zVal<size.z;++zVal)
			{
				if(xVal==0 || xVal==size.x-1)
				{
					graphData[xVal][yVal][zVal]=Face_INVALID;
					int checkVal=check6Directions(Vector3<s64>(minPos.x-1+xVal,minPos.y-1+yVal,minPos.z-1+zVal));
					if(checkVal==Face_MERGE)
					{
						graphData[xVal][yVal][zVal]=Face_MERGE;
						merge=true;
					}
				}
				else if(yVal==0 || yVal==size.y-1)
				{
					graphData[xVal][yVal][zVal]=Face_INVALID;
					int checkVal=check6Directions(Vector3<s64>(minPos.x-1+xVal,minPos.y-1+yVal,minPos.z-1+zVal));
					if(checkVal==Face_MERGE)
					{
						graphData[xVal][yVal][zVal]=Face_MERGE;
						merge=true;
					}
				}
				else if(zVal==0 || zVal==size.z-1)
				{
					graphData[xVal][yVal][zVal]=Face_INVALID;
					int checkVal=check6Directions(Vector3<s64>(minPos.x-1+xVal,minPos.y-1+yVal,minPos.z-1+zVal));
					if(checkVal==Face_MERGE)
					{
						graphData[xVal][yVal][zVal]=Face_MERGE;
						merge=true;
					}
				}
				else
				{
					graphData[xVal][yVal][zVal]=check6Directions(Vector3<s64>(minPos.x-1+xVal,minPos.y-1+yVal,minPos.z-1+zVal));
				}
			}
		}
	}

	nullifyCorners();

	//Ensures resources exist to build the room before altering blocks
	if(gameCore->resourceMan.checkRoomBuildResources(blockType,((size.x-2)*(size.y-2)*(size.z-2)),"Default"))
	{
		for(int xVal(0);xVal<size.x;++xVal)
		{
			for(int yVal(0);yVal<size.y;++yVal)
			{
				for(int zVal(0);zVal<size.z;++zVal)
				{
					if((graphData[xVal][yVal][zVal]&Face_MERGE)==Face_MERGE)
					{
						int bitmask(0);

						if(xVal<=1)
						{
							if((graphData[xVal+1][yVal][zVal]&Face_INVALID)==0)
								bitmask|=Face_XP;
						}
						else if(xVal>=size.x-2)
						{
							if((graphData[xVal-1][yVal][zVal]&Face_INVALID)==0)
								bitmask|=Face_XM;
						}
						else
						{
							if((graphData[xVal+1][yVal][zVal]&Face_INVALID)==0)
								bitmask|=Face_XP;
							if((graphData[xVal-1][yVal][zVal]&Face_INVALID)==0)
								bitmask|=Face_XM;
						}

						if(zVal<=1)
						{
							if((graphData[xVal][yVal][zVal+1]&Face_INVALID)==0)
								bitmask|=Face_ZP;
						}
						else if(zVal>=size.z-2)
						{
							if((graphData[xVal][yVal][zVal-1]&Face_INVALID)==0)
								bitmask|=Face_ZM;
						}
						else
						{
							if((graphData[xVal][yVal][zVal+1]&Face_INVALID)==0)
								bitmask|=Face_ZP;
							if((graphData[xVal][yVal][zVal-1]&Face_INVALID)==0)
								bitmask|=Face_ZM;
						}

						if((zVal==size.z-1 && xVal==0) ||
							(zVal==size.z-1 && xVal==size.x-1) ||
							(zVal==0 && xVal==0) ||
							(zVal==0 && xVal==size.x-1))
						{
							//empty logic block
						}
						else
							gameCore->bMan.alterMergeBlock(bitmask,Vector3<s64>(minPos.x+xVal-1,minPos.y+yVal-1,minPos.z+zVal-1),false);
					}
				}
			}
		}
	}
}

void BuildingGraph::nullifyCorners()
{
	//set corners of the graph to 0 since no building or checking can occur there
	graphData[0][0][0]=Face_INVALID;
	graphData[0][0][size.z-1]=Face_INVALID;
	graphData[0][size.y-1][0]=Face_INVALID;
	graphData[0][size.y-1][size.z-1]=Face_INVALID;
	graphData[size.x-1][0][0]=Face_INVALID;
	graphData[size.x-1][0][size.z-1]=Face_INVALID;
	graphData[size.x-1][size.y-1][0]=Face_INVALID;
	graphData[size.x-1][size.y-1][size.z-1]=Face_INVALID;
}

int BuildingGraph::check6Directions(const Vector3<s64>& pos)
{
	int checkRooms=gameCore->bMan.checkNearbyRooms(pos,blockType);
	//call the block checking algorithm to see if it is a valid building position first
	if(checkRooms==1)
	{
 	 	Vector3<s64> xPlus(Vector3<s64>(1,0,0)+pos),
	 	 	 	 xMinus (Vector3<s64>(-1,0,0)+pos),
	 	 	 	 yPlus(Vector3<s64>(0,1,0)+pos),
	 	 	 	 yMinus (Vector3<s64>(0,-1,0)+pos),
	 	 	 	 zPlus (Vector3<s64>(0,0,1)+pos),
	 	 	 	 zMinus (Vector3<s64>(0,0,-1)+pos) ;
	 	int returnBitmask(Face_INVALID);

	 	//A building can be placed at this location. Check the surrounding 6 directions to determine what walls are needed
	 	 /*if(gameCore->bMan.checkNearbyRooms(yPlus,blockType)!=false)
	 	 {
	 	 	returnBitmask|=Face_YP;
	 	 }
	 	 if(gameCore->bMan.checkNearbyRooms(yMinus,blockType)!=false)
	 	 {
	 	 	returnBitmask|=Face_YM;
	 	 }*/
		int xP=gameCore->bMan.checkRooms(xPlus,blockType);
		if(xP!=2)
		{
			if(pos.x==minPos.x+size.x-3)
				returnBitmask|=Face_XP;
			else if(xP==0)
				returnBitmask|=Face_XP;
		}
		int xM=gameCore->bMan.checkRooms(xMinus,blockType);
		if(xM!=2)
		{
			if(pos.x==minPos.x)
				returnBitmask|=Face_XM;
			else if(xM==0)
				returnBitmask|=Face_XM;
		}
		int zP=gameCore->bMan.checkRooms(zPlus,blockType);
		if(zP!=2)
		{
			if(pos.z==minPos.z+size.z-3)
				returnBitmask|=Face_ZP;
			else if(zP==0)
				returnBitmask|=Face_ZP;
		}
		int zM=gameCore->bMan.checkRooms(zMinus,blockType);
		if(zM!=2)
		{
			if(pos.z==minPos.z)
				returnBitmask|=Face_ZM;
			else if(zM==0)
				returnBitmask|=Face_ZM;
		}

		if(returnBitmask>=Face_INVALID)
			returnBitmask-=Face_INVALID;

	 	return returnBitmask;
	}
	else if(checkRooms==2)
	{
		if(gameCore->bMan.checkMergeRooms(pos,blockType))
		{
			int returnBitmask(Face_MERGE);
			return returnBitmask;
		}
		else
		{
			Vector3<s64> xPlus(Vector3<s64>(1,0,0)+pos),
	 	 	 	 xMinus (Vector3<s64>(-1,0,0)+pos),
	 	 	 	 yPlus(Vector3<s64>(0,1,0)+pos),
	 	 	 	 yMinus (Vector3<s64>(0,-1,0)+pos),
	 	 	 	 zPlus (Vector3<s64>(0,0,1)+pos),
	 	 	 	 zMinus (Vector3<s64>(0,0,-1)+pos) ;
	 		int returnBitmask(Face_INVALID);

	 		//A building can be placed at this location. Check the surrounding 6 directions to determine what walls are needed
	 		/*if(gameCore->bMan.checkNearbyRooms(yPlus,blockType)!=false)
	 		{
	 	 	returnBitmask|=Face_YP;
	 		}
	 		if(gameCore->bMan.checkNearbyRooms(yMinus,blockType)!=false)
	 		{
	 	 	returnBitmask|=Face_YM;
	 		}*/
			if(gameCore->bMan.checkNearbyRooms(xPlus,blockType)!=2)
			{
				if(pos.x==minPos.x+size.x-3)
					returnBitmask|=Face_XP;
			}
			if(gameCore->bMan.checkNearbyRooms(xMinus,blockType)!=2)
			{
				if(pos.x==minPos.x)
					returnBitmask|=Face_XM;
			}
			if(gameCore->bMan.checkNearbyRooms(zPlus,blockType)!=2)
			{
				if(pos.z==minPos.z+size.z-3)
					returnBitmask|=Face_ZP;
			}
			if(gameCore->bMan.checkNearbyRooms(zMinus,blockType)!=2)
			{
				if(pos.z==minPos.z)
					returnBitmask|=Face_ZM;
			}

			if(returnBitmask>=Face_INVALID)
				returnBitmask-=Face_INVALID;

	 		return returnBitmask;
		}
	}
	else
	{
	 	return Face_INVALID;
	}
}
