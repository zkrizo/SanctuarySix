//Graphics Core Management Class
#ifndef GraphicsCore_H
#define GraphicsCore_H

#include "ChunkManager.h"
#include "OpenGLRenderer.h"
#include "LandGen.h"
#include <Outland\Resource Core\Bot.h>

//Externalizing variables
extern int RENDER_DIST;

class GraphicsCore
{
friend class Loader;
friend class CameraControl;
public:
	static GraphicsCore* Instance();

	//standard trifecta engine core functions
	void startup();
	void update(real32 time);
	void shutdown();
	void startSingleGameplay(bool newgame);

	OpenGLRenderer* pRenderer;
	LandGen LGen;
	void setPixelFormat();
	void setSeed(s64 sv);
	void setCenterPos(Vector3<s64> pos);
	s64 getSeed();
	Vector3<s64> getCenterPos();
	ChunkManager chunkMan;

	//Graphics Core passdown functions
	bool Check6Block(const Vector3<s64>& pos){return chunkMan.Check6Block(pos);};
protected:
	GraphicsCore();
	~GraphicsCore();
	GraphicsCore(const GraphicsCore&);
	GraphicsCore operator=(const GraphicsCore&);

	s64 seed;
	Vector3<s64> centerPos;

	void resetBuffers();
	Bot *rover;
};

#endif