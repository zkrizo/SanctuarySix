#include <Outland\Vector.h>
#include <Outland\UnitAbstraction.h>

class BuildingGraph
{
public:

	BuildingGraph(const Vector3<s64> &startPos, const Vector3<s64> &endPos, const int &blockType, const bool &mergeEnabled, const bool &ghosted);
	~BuildingGraph();
	int getData(const int& xPos, const int& yPos, const int& zPos){return graphData[xPos][yPos][zPos];};
	Vector3<s64> getSize(){return size;};
	Vector3<s64> getMinPos(){return minPos;};
	bool needsMerged()const{return merge;};

private:

	int*** graphData;
	Vector3<s64> size;
	Vector3<s64> minPos;
	int blockType;
	bool mergeEnabled;
	bool ghosted;
	bool merge;

	void nullifyCorners();
	void generateData();
	int check6Directions(const Vector3<s64>& pos);
};
