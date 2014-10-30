#ifndef GUI_H
#define GUI_H

#include "Button.h"
#include <Outland\UnitAbstraction.h>
#include <Outland\Vector.h>
#include <Outland\Timer.h>
#include <Windows.h>
#include <gl\glew.h>
#include <gl\GLU.h>
#include "GUI_Building.h"
#include "GUI_Menu.h"
#include "GUI_Resources.h"
#include "GUI_Tech.h"

class GUI
{
public:
	GUI();
	~GUI();
	void clicked(const s32& X,const s32& Y);
	void Render();
	void Startup();
	void ResetUI();
	void setGUI(const s32& ui);
private:
	s32 curUI;
	Button *Resources;
	Button *Building;
	Button *Research;
	Button *View;
	Button *System;

	GLuint *background;
	GUI_Building	gBuild;
	GUI_Tech		gTech;
	GUI_Resources	gResource;
	GUI_Menu		gMenu;
	Timer			t1;

	real32 xP;
	real32 xM;
	real32 yP;
	real32 yM;
	real32 xT;
	real32 yT;
};

#endif