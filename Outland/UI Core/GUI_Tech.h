#ifndef GUI_Tech_H
#define GUI_Tech_H

#include "Button.h"
#include <Outland\UnitAbstraction.h>
#include <Outland\Vector.h>
#include <Windows.h>
#include <gl\glew.h>
#include <gl\GLU.h>

class GUI_Tech
{
public:
	GUI_Tech();
	~GUI_Tech();
	tBOOL clicked(const s32& X,const s32& Y);
	void Render();
	void Startup();

private:
	Button *close;

	GLuint *background;

	real32 xP;
	real32 xM;
	real32 yP;
	real32 yM;
	real32 xT;
	real32 yT;
};

#endif