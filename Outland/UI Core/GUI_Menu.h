#ifndef GUI_Menu_H
#define GUI_Menu_H

#include "Button.h"
#include <Outland\UnitAbstraction.h>
#include <Outland\Vector.h>
#include <Windows.h>
#include <gl\glew.h>
#include <gl\GLU.h>

class GUI_Menu
{
public:
	GUI_Menu();
	~GUI_Menu();
	tBOOL clicked(const s32& X,const s32& Y);
	void Render();
	void Startup();

private:
	Button *Quit;
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