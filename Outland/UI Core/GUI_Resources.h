#ifndef GUI_Resources_H
#define GUI_Resources_H

#include "Button.h"
#include <Outland\UnitAbstraction.h>
#include <Outland\Vector.h>
#include <Windows.h>
#include <gl\glew.h>
#include <gl\GLU.h>

class GUI_Resources
{
public:
	GUI_Resources();
	~GUI_Resources();
	tBOOL clicked(const s32& X,const s32& Y);
	void Render();
	void Startup();

private:
	Button *close;
	GLuint *background;

	//Variables for main GUI element
	real32 xP;
	real32 xM;
	real32 yP;
	real32 yM;
	real32 xT;
	real32 yT;
};

#endif