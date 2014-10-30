#ifndef Pathfinding_H
#define Pathfinding_H

#include <Outland\UnitAbstraction.h>
#include <Outland\Vector.h>
#include <Outland\Quaternion.h>
#include "Path.h"


class Pathfinding
{
public:
	Pathfinding();
	~Pathfinding();


	Path AStarPath(Vector3<s64> p1, Vector3<s64> p2);
private:
	tBOOL AStarRecursion(Vector3<s64> p1, Vector3<s64> p2);
	std::vector<Vector3<s64>> closedList;
	Path tempPath;
	Vector3<s64> curNode;
};

#endif