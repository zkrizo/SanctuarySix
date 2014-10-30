#include "GUI_Tech.h"
#include <Outland\Singletons.h>

extern int RES_WIDTH;
extern int RES_HEIGHT;

/****************************
* Constructor
****************************/
GUI_Tech::GUI_Tech():
	xP(real32(RES_WIDTH/2.0f+(8*RES_HEIGHT/20))),
	xM(real32(RES_WIDTH/2.0f-(8*RES_HEIGHT/20))),
	yP(real32(16*(RES_HEIGHT-38)/20)),
	yM(real32(1*(RES_HEIGHT-38)/20)),
	xT(real32(xP-xM)),
	yT(real32(yP-yM))
{}

/****************************
* Destructor
****************************/
GUI_Tech::~GUI_Tech()
{
	delete close;
}

void GUI_Tech::Startup()
{
	//sets the textures
	background=grCore->pRenderer->getTexture(2,GUI_WINDOW);

	close=new Button("Close",(xT/10),(2*xT/40),xP-(6*xT/40),yM+(yT/20),grCore->pRenderer->getTexture(2,GUI_CLOSE));

}

/****************************
* Render
****************************/
void GUI_Tech::Render()
{
	glColor4f(1.0f,1.0f,1.0f,0.8f);

	//Bind the secondary window background texture
	glBindTexture(GL_TEXTURE_2D,*background);

	//Draw the window background
	glBegin(GL_QUADS);
		glTexCoord2f(0.0f,1.0f);		glVertex2f(xM,yM); //LL
		glTexCoord2f((float)480.0f/512.0f,1.0f);		glVertex2f(xP,yM); //LR
		glTexCoord2f((float)480.0f/512.0f,1.0f-(float)421.0f/512.0f);	glVertex2f(xP,yP); //UR
		glTexCoord2f(0.0f,1.0f-(float)421.0f/512.0f);		glVertex2f(xM,yP); //UL
	glEnd();

	//Switch to Close button texture
	glBindTexture(GL_TEXTURE_2D,*close->texture);

	close->Render();
	
	glColor4f(1.0f,1.0f,1.0f,1.0f);
}

/****************************
* clicked -	checks for additional clicks against the secondary GUI window
****************************/
tBOOL GUI_Tech::clicked(const s32& X,const s32& Y)
{	
	//Check if click was within the GUI's window
	if((X>=xM && X<=xP) && (Y>=yM && Y<=yP))
	{
		//check for click on the close button
		if(close->clicked(X,Y))
		{
			//resets the current UI to UI_GAME thus closing the window
			uiCore->UI_GUI.ResetUI();
		}
			
		return TRUE;
	}

	return FALSE;
}