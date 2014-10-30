#ifndef ChunkManager_H
#define ChunkManager_H

#include "Chunk.h"
#include <vector>
#include <Outland\Vector.h>
#include <Outland\Ray.h>
#include <Outland\UnitAbstraction.h>
#include <hash_map>

//External includes
extern s32 RENDER_DIST;
extern s32 CHUNK_SIZE;
extern s32 HEIGHT_CHUNK;

class ChunkManager
{
	friend class Loader;
	friend class CameraControl;
protected:
	Chunk ***p_chunks;
	Vector4<real64> planes[6];
	Block *blockHit;
	std::hash_map<std::string,Chunk*> chunkMap;
private:
	std::vector<Chunk*> renderList;
	tBOOL FrustrumCheck(Vector3<s64> pos);

	Vector3<s64> calculateChunkPos(const Vector3<s64> &pos);
	Vector3<s32> calculateBlockPos(const Vector3<s64> &pos, const Vector3<s64> &chunkPos);
public:
	ChunkManager();
	~ChunkManager();
	Block tempRay;

	void RDistChange(int newDist);
	void Render();
	void UnloadAll();
	void Unload(Chunk*);
	void Load();
	void ChangeBlock(Vector3<s64> pos,bool removed);
	void CheckEdges();
	void CheckEdges(const Vector3<s64>& pos);				//CHECK EDGES may now be unneeded since hashmap implementation. Removal pending.
	bool CheckBlock(const Vector3<s64>& pos);
	bool Check6Block(const Vector3<s64>& pos);
	Block* getReferenceToBlock(const Vector3<s64>&pos);
	void GenerateChunk(Vector3<s64> pos);
	void ACull();
	void VCull();
	void rayTrace(const Ray<real32>& ray,RayCol<Block>& rayBlock);
};

#endif