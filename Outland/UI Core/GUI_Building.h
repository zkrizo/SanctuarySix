#ifndef GUI_Building_H
#define GUI_Building_H

#include "Button.h"
#include <Outland\UnitAbstraction.h>
#include <Outland\Vector.h>
#include <Windows.h>
#include <gl\glew.h>
#include <gl\GLU.h>
#include <Outland\Player Core\BuildInfo.h>

class GUI_Building
{
public:
	GUI_Building();
	~GUI_Building();
	tBOOL clicked(const s32& X,const s32& Y);
	void Render();
	void startup();
private:
	s32 curtab;
	s32 curbuild;
	GLuint *background;
	GLuint *secondbackground;
	BuildInfo *curInfo;
	Button *Build,*Demolish,*Select,*Hallway,*CommandCenter,*LivingQuarters,*Bathroom,*Airlock,*FreightAirlock;
	Button *Galley,*IndustrialWorkshop,*Laboratory,*Library,*MCC,*PowerPlant,*Warehouse;

	Button *close;
	Button *tabTech1;
	Button *tabTech2;
	Button *tabTech3;
	Button *tabTech4;
	Button *tabObjects;

	Button *ObjDoor;

	//Variables for main GUI element
	real32 xP;
	real32 xM;
	real32 yP;
	real32 yM;
	real32 xT;
	real32 yT;

	void updateInfoOutput();
	string info1,info2,info3,info4,info5;
};

#endif