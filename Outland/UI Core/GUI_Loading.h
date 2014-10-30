#ifndef GUI_Loading_H
#define GUI_Loading_H

#include "Button.h"
#include <Outland\UnitAbstraction.h>
#include <Outland\Vector.h>
#include <Windows.h>
#include <gl\glew.h>
#include <gl\GLU.h>

class GUI_Loading
{
public:
	static GUI_Loading* Instance();
	int getID(){return ID;}
	void incrementAndRender();
	void Render();
	void Startup();
private:
	int ID;
	GUI_Loading():ID(0),numMenuTextures(0),
				numBlockTextures(0),
				numWallTextures(0),
				numFloorTextures(0),
				numCeilingTextures(0),
				numModels(0),
				numChunks(0),
				total(0),current(0),
				background(NULL),barBG(NULL),barFG(NULL),
				xP(0),xM(0),yP(0),yM(0),xT(0),yT(0),yText(0){};
	~GUI_Loading();
	GUI_Loading(const GUI_Loading&);
	GUI_Loading& operator=(const GUI_Loading&);

	int numMenuTextures;
	int numBlockTextures;
	int numWallTextures;
	int numFloorTextures;
	int numCeilingTextures;
	int numModels;
	int numChunks;
	int total,current;

	GLuint *background;
	GLuint *barBG;
	GLuint *barFG;

	real32 xP;
	real32 xM;
	real32 yP;
	real32 yM;
	real32 xT;
	real32 yT;
	real32 yText;
};

#endif