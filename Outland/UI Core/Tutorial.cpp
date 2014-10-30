#include "Tutorial.h"

extern int RES_WIDTH;
extern int RES_HEIGHT;

Tutorial::Tutorial():
	step(-1),
	xP(real32(RES_WIDTH)),
	xM(0),
	yP(real32(RES_HEIGHT)),
	yM(0)
{}

Tutorial::~Tutorial()
{

}

bool Tutorial::click(s32 X, s32 Y)
{
	if(step<8 && step!=-1)
	{
		s32 val=mBox.click(X,Y);
		if(val==1)
		{
			incStep();
			return true;
		}
		else if(val==2)
		{
			step=100;
			return true;
		}
		else if(val==3)
			return true;
	}
	return false;
}

void Tutorial::Startup()
{
	step=0;
	mBox.Startup();

	mBox.setInfo("Welcome Commander! We have set up the command center for your base", 
				"and are ready to proceed with further operations. Click next to continue ",
				"the tutorial. You can skip this tutorial at any time by clicking the ",
				"skip button",
		xM+(4*xP/20),yM,12*xP/20,yM+3*xP/20,true,true);
}

void Tutorial::Render()
{
	if(step<8 && step!=-1)
		mBox.Render();
}

void Tutorial::incStep()
{
	step++;
	switch(step)
	{
	case 1:
		mBox.setInfo("You can move around with your Weatherproof Aerial Survey ", 
				"Device - W.A.S.D. for short. You can move the view by holding",
				"the right mouse button and dragging.",
				"",
		xM+(4*xP/20),yM,12*xP/20,yM+3*xP/20,true,true);
		break;
	case 2:
		mBox.setInfo("At the top of your screen is the interaction panel. This", 
				"panel holds all the important buttons for interacting with the",
				"world. From left to right, the buttons are Resources, Building",
				"Technology, Avatar View, and the Menu.",
		xM+(4*xP/20),yM,12*xP/20,yM+3*xP/20,true,true);
		break;
	case 3:
		mBox.setInfo("Resources gives you an up to date breakdown of your current", 
				"resources. This is updated in real time as you gain or lose",
				"resources. The two most important items are Power and Air",
				"which are both shown as bars according to your use.",
		xM+(4*xP/20),yM,12*xP/20,yM+3*xP/20,true,true);
		break;
	case 4:
		mBox.setInfo("Be careful of how much you build as power is a precious resource. ", 
				"Without it the base will begin experiencing power losses and could ",
				"even begin to lose air pressure!",
				"",
		xM+(4*xP/20),yM,12*xP/20,yM+3*xP/20,true,true);
		break;
	case 5:
		mBox.setInfo("Under the building icon, you will find the various buildings", 
				"that we can build. Simply select one and click the build button to",
				"enter build mode. While in build mode, click and drag to select the size",
				"of your building and release the left mouse button to build.",
		xM+(4*xP/20),yM,12*xP/20,yM+3*xP/20,true,true);
		break;
	case 6:
		mBox.setInfo("Unfortunately, we do not yet have access to research technology", 
				"but the mothership should be sending the plans for a laboratory down",
				"soon so we can get started on researching!","",
		xM+(4*xP/20),yM,12*xP/20,yM+3*xP/20,true,true);
		break;
	case 7:
		mBox.setInfo("Good luck with running the base commander! We are all trusting ", 
				"that you will be able to do a great job!",
				"","",
		xM+(4*xP/20),yM,12*xP/20,yM+3*xP/20,true,true);
		break;
	}
}