#ifndef Path_H
#define Path_H

#include <Outland\UnitAbstraction.h>
#include <Outland\Vector.h>
#include <Outland\Quaternion.h>
#include <queue>

class Path
{
friend class Pathfinding;

public:
	Path();
	~Path();
	
	Vector3<real64> startPos;
	Vector3<real64> endPos;
	void removeFrontNode();
	void addNode(Vector3<s64> pos);
protected:

	std::queue<Vector3<s64>> path;
};

#endif