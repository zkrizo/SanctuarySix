#ifndef messageBox_H
#define messageBox_H

#include <Outland\UnitAbstraction.h>
#include <Outland\Vector.h>
#include <string>
#include <gl\glew.h>
#include <gl\GLU.h>
#include "Button.h"

using namespace std;

class messageBox
{
public:
	messageBox();
	~messageBox();
	void Startup();
	void Render();
	void setInfo(string message,string message2,string message3,string message4,s32 posx, s32 posy, s32 sizex, s32 sizey, bool next, bool skip);
	s32 click(s32 X, s32 Y);

private:
	GLuint *texture;
	Button *next;
	Button *skip;
	bool nextShow;
	bool skipShow;
	string Message;
	string Message2;
	string Message3;
	string Message4;
	s32 posX;
	s32 posY;
	s32 sizeX;
	s32 sizeY;
};

#endif