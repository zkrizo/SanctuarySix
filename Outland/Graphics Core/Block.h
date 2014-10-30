#ifndef Block_H
#define Block_H

#include <Outland\UnitAbstraction.h>
#include <Outland\Vector.h>


class Block
{
public:
	Block();
	Block(int blockType, bool active,Vector3<s32>);
	~Block();

	bool checkActive();
	void setActive(bool active);
	void Render();
	s32 getType() const{return blockType;};
	int getFaceData() const{return faceData;};
	Vector3<s32> getPos() const{return pos;};
	void setType(s32 type);
	void setPos(Vector3<s32> pos);
	void addFace(const int & faceMask);
	void removeFace(const int & faceMask);
	void recordFaceCheck(const int & faceMask);
	void clearFaceChecks();
	bool check6Faces(const Vector3<s64>& chunkPos);
void setTexture();
private:
	s32 blockType;
	int faceData;
	bool isActive;
	Vector3<s32> pos;

};

#endif