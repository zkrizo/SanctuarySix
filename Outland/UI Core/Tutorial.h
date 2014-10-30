#ifndef Tutorial_H
#define Tutorial_H

#include <Outland\UnitAbstraction.h>
#include <Outland\Vector.h>
#include "MessageBox.h"

class Tutorial
{
public:
	Tutorial();
	~Tutorial();
	bool click(s32 X, s32 Y);
	void Startup();
	void Render();
	void incStep();
private:
	messageBox mBox;
	s32 step;
	real32 xP;
	real32 xM;
	real32 yP;
	real32 yM;
	real32 xT;
	real32 yT;
};

#endif