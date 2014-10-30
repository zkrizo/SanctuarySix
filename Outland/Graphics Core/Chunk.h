#ifndef Chunk_H
#define Chunk_H

#include "Block.h"
#include <vector>
#include <Outland\UnitAbstraction.h>
#include <Outland\Vector.h>
#include <Outland\Ray.h>
#include "Polygroup.h"


class Chunk
{
public:
	Chunk();
	Chunk(const Vector3<s64>& pos);
	~Chunk();

	void activeCulling();		//Will cull all non-active blocks (completely surrounded blocks) from the render list
	void visibleCulling();		//Will cull all non-visible blocks from the render list
	void Render();
	void CheckActive(const Vector3<s32>& blockPos,const bool& active);
	void ChangeActive(const Vector3<s32>& blockPos);
	void Generate();
	void CopyChunk(const Chunk& chunk);
	real32 blockRayTrace(const Ray<real32>& ray,s32 &face,RayCol<Block>& rayBlock);
	real32 rayTrace(const Ray<real32>& ray);
	int getBlockFaceData(const Vector3<s32>& blockPos){return blocks[blockPos.x][blockPos.y][blockPos.z].getFaceData();}

	bool CheckActive(const Vector3<s32>& blockPos){return blocks[blockPos.x][blockPos.y][blockPos.z].checkActive();};
	s32 CheckType(const Vector3<s32>& blockPos){return blocks[blockPos.x][blockPos.y][blockPos.z].getType();};
	std::vector<Block> blockRenderList;
	Block*** blocks;
	Vector3<s64> pos;
	tBOOL TOTAL_RENDER;
private:
	Polygroup polys;
	void addPolysFromBlocks(const Block& block);

};

#endif