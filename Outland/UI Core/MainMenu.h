#ifndef Menu_H
#define Menu_H

#include "Button.h"
#include <Outland\UnitAbstraction.h>
#include <Outland\Vector.h>
#include <Windows.h>
#include <gl\glew.h>
#include <gl\GLU.h>

class Menu
{
public:
	Menu();
	~Menu();
	void clicked(real32 X,real32 Y);
	void Render();
	void Startup();

	//texture for title image
	GLuint* titleTexture;

	//debug tools
	tBOOL testDraw;
	Vector3<float> color;

private:
	Button *NewU;
	Button *LoadU;
	Button *Multi;
	Button *Options;
	Button *Quit;
	real32 cX;
	real32 cY;
	real32 xP;
	real32 xM;
	real32 yP;
	real32 yM;
	real32 xT;
	real32 yT;
};

#endif