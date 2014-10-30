#include "GUI_Menu.h"
#include <Outland\Singletons.h>

extern int RES_WIDTH;
extern int RES_HEIGHT;

/****************************
* Constructor
****************************/
GUI_Menu::GUI_Menu():
	xP(real32(RES_WIDTH/2.0f+(8*RES_HEIGHT/20))),
	xM(real32(RES_WIDTH/2.0f-(8*RES_HEIGHT/20))),
	yP(real32(16*(RES_HEIGHT-38)/20)),
	yM(real32(1*(RES_HEIGHT-38)/20)),
	xT(xP-xM),
	yT(yP-yM)
{}

/****************************
* Destructor
****************************/
GUI_Menu::~GUI_Menu()
{
	delete Quit;
	delete close;
}

void GUI_Menu::Startup()
{
	close=new Button("Close",(xT/10),(2*xT/40),xP-(6*xT/40),yM+(yT/20),grCore->pRenderer->getTexture(2,GUI_CLOSE));
	background=grCore->pRenderer->getTexture(2,GUI_WINDOW);
	Quit=new Button("Quit",0.25*xT,0.1*xT,xP-(0.65*xT),yM+(0.3*yT),grCore->pRenderer->getTexture(2,GUI_QUIT));
}

/****************************
* Render
****************************/
void GUI_Menu::Render()
{
	//Sets the alpha to semi transparent
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

	//Switch to Quit button texture
	glBindTexture(GL_TEXTURE_2D,*Quit->texture);
	Quit->Render();

	//Switch to Close button texture
	glBindTexture(GL_TEXTURE_2D,*close->texture);
	close->Render();

	glColor4f(1.0f,1.0f,1.0f,1.0f);
}

/****************************
* clicked -	checks for additional clicks against the secondary GUI window
****************************/
tBOOL GUI_Menu::clicked(const s32& X,const s32& Y)
{		
	//Variables for main GUI element
	real32 xP=RES_WIDTH/2.0f+(8*RES_HEIGHT/20);
	real32 xM=RES_WIDTH/2.0f-(8*RES_HEIGHT/20);
	real32 yP=(real32)(16*(RES_HEIGHT-38)/20);
	real32 yM=(real32)(1*(RES_HEIGHT-38)/20);
	real32 xT=xP-xM;
	real32 yT=yP-yM;

	if(X>=xM && X<=xP)
	{
		if(Y>=yM && Y<=yP)
		{
			//Click was within the GUI's window

			//check for click on the close button
			if(X>=xP-(6*xT/40) && X<=xP-(2*xT/40))
			{
				if(Y<=yM+(yT/20)+(2*xT/40) && Y>=yM+(yT/20))
				{
					//resets the current UI to UI_GAME thus closing the window
					uiCore->UI_GUI.ResetUI();
				}

				
			}

			//check for click with the Quit button
			if(X<=xP-(7*xT/20) && X>=xP-(13*xT/20))
			{
				if(Y<=yM+(3*yT/10)+(2*xT/20) && Y>=yM+(3*yT/10))
				{
					gameCore->levMan.Save();
					PostQuitMessage(0);
				}
			}
			return TRUE;
		}
	}

	return FALSE;
}