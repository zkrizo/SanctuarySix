#include "Pathfinding.h"
#include <Outland\Vector.h>
#include <Windows.h>


/****************************
* Constructor
****************************/
Pathfinding::Pathfinding()
{

}

/****************************
* Destructor
****************************/
Pathfinding::~Pathfinding()
{

}

/****************************
* AStarPath - A* Pathfinding recursive algorithm to generate a path between points
****************************/
Path Pathfinding::AStarPath(Vector3<s64> start,Vector3<s64> end)
{
	//clears the path variable
	while(!tempPath.path.empty())
	{
		tempPath.path.pop();
	}

	//start recursion
	AStarRecursion(start,end);

	return tempPath;
}

/****************************
* AStarRecursion - A* Pathfinding recursive algorithm
****************************/
tBOOL Pathfinding::AStarRecursion(Vector3<s64> start, Vector3<s64> end)
{
	//add start point to the closed list
	closedList.push_back(start);

	curNode=start;

	if(curNode==end)
	{
		//Path found
		tempPath.addNode(start);
		return TRUE;
	}

	//calculate the manhattan distances
	s64 x,y,z,dist;
	x=abs(end.x-start.x);
	y=abs(end.y-start.y);
	z=abs(end.z-start.z);

	dist=x+y+z;
	//determine the closest node
	s64 dist2,dist3,dist4;

	dist2=abs(end.x-start.x+1)+abs(end.y-start.y)+abs(end.z-start.z);
	dist3=abs(end.x-start.x)+abs(end.y-start.y+1)+abs(end.z-start.z);
	dist4=abs(end.x-start.x)+abs(end.y-start.y)+abs(end.z-start.z+1);
	
	if(dist2<dist)
	{
		if(!AStarRecursion(Vector3<s64>(start.x+1,start.y,start.z),end))
		{

		}
	}
	else if(dist3<dist)
	{

	}
	else if(dist4<dist)
	{

	}
	else
	{
		
	}

	if(curNode==end)
	{
		//Path found
		tempPath.addNode(start);
		return TRUE;
	}

	return FALSE;
}