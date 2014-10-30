#ifndef OpenGLRenderer_H
#define OpenGLRenderer_H

#include <Windows.h>
#include <gl\glew.h>
#include <gl\GLU.h>
#include <string>
#include <Outland\Player Core\Building.h>

#include "Chunk.h"

class OpenGLRenderer
{
public:
	OpenGLRenderer();
	~OpenGLRenderer();
	void RenderTestCube();
	//void RenderTestCube(Block* block,int x,int y, int z);
	void RenderTestCube(const Block& block,int x,int y, int z);

	void RenderTestChunk(Chunk *chunk);
	void RenderTestBuilding();
	void RenderTestBuilding(Vector3<s32> pos, s32 orient, s32 walls, s32 type, tBOOL special,tBOOL powered);
	void RenderTestBuilding(Building* const building,tBOOL powered);
	void RenderTestBuilding(const Building& building, const bool& powered);
	void startup();
	
	tBOOL LoadBMPImage(t8*file, GLuint &texture);
	tBOOL LoadBMPImageNoFlip(t8*file, GLuint &texture);
	void BatchBMPLoad();
	void ready3D();
	void ready2D();
	GLuint* getTexture(u32 texttype,s32 index);
	void drawLine(Vector3<real32> pos1, Vector3<real32> pos2);
	void drawLine(Vector3<real32> pos1, Vector3<real32> direction, s32 magnitude);
	void debugDraw();
	void setBlockTexture(const int &blockType);
private:
	GLuint *blockTextures;
	GLuint *menuTextures;
	GLuint *GUITextures;
	GLuint *roomTextures;
	GLuint *floorTextures;
	GLuint *outerTextures;
	GLuint *modelTexturesDiffuse;
	GLuint *modelTexturesNormals;
	GLuint *modelTexturesBump;
	GLuint box;
	GLuint FacePolyYP, FacePolyYM, FacePolyZP, FacePolyZM, FacePolyXP, FacePolyXM;
	GLuint room0Walls;
	GLuint room1Walls;
	GLuint room2WallsHall;
	GLuint room2WallsCorner;
	GLuint room3Walls;
	GLuint room4Walls;
	GLuint BuildingFloor,BuildingCeiling,BuildingOuterWall,BuildingUpperWall,BuildingInnerWall;
	GLuint BuildingOuterWallDoor,BuildingInnerWallDoor;
	GLuint BuildingUpperCorner,BuildingInnerCorner;

	std::vector<Vector3<real32>> lines;
	tBOOL generateLists();
	void setBuildingCeilingTexture(const int& type, const bool& powered);
	void setBuildingFloorTexture(const int& type, const bool& powered);
	void setBuildingInnerWallTexture(const int& type, const bool& powered);
	void setBuildingOuterWallTexture(const int& type, const bool& powered);
	void setBuildingUpperWallTexture(const int& type, const bool& powered);
};

#endif