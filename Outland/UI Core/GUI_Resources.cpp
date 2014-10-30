#include "GUI_Resources.h"
#include <Outland\Singletons.h>
#include <math.h>
#include <string>

extern int RES_WIDTH;
extern int RES_HEIGHT;

/****************************
* Constructor
****************************/
GUI_Resources::GUI_Resources():
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
GUI_Resources::~GUI_Resources()
{
	delete close;
}

void GUI_Resources::Startup()
{
	close=new Button("Close",(xT/10),(2*xT/40),xP-(6*xT/40),yM+(yT/20),grCore->pRenderer->getTexture(2,GUI_CLOSE));
	background=grCore->pRenderer->getTexture(2,GUI_WINDOW);
}

/****************************
* Render
****************************/
void GUI_Resources::Render()
{
	GLfloat power=gameCore->player.inventory.getResource(RES_CURPOWER)/gameCore->player.inventory.getResource(RES_MAXPOWER);
	GLfloat air=gameCore->player.inventory.getResource(RES_CURAIR)/gameCore->player.inventory.getResource(RES_MAXAIR);
	std::string pow,strair,steel,wood,iron,food,water,coal,copper,gold,platinum,quartz,silicon,uranium,glass,wiring;

	pow="Power: " + uiCore->floatToString(gameCore->player.inventory.getResource(RES_CURPOWER),2);
	
	food="Food: " + uiCore->floatToString(gameCore->player.inventory.getResource(RES_FOOD),2);
	water="Water: " + uiCore->floatToString(gameCore->player.inventory.getResource(RES_WATER),2);
	coal="Coal: " + uiCore->floatToString(gameCore->player.inventory.getResource(RES_COAL),2);
	copper="Copper: " + uiCore->floatToString(gameCore->player.inventory.getResource(RES_COPPER),2);
	iron="Iron: " + uiCore->floatToString(gameCore->player.inventory.getResource(RES_IRON),2);
	gold="Gold: " + uiCore->floatToString(gameCore->player.inventory.getResource(RES_GOLD),2);
	platinum="Platinum: " + uiCore->floatToString(gameCore->player.inventory.getResource(RES_PLATINUM),2);
	quartz="Quartz: " + uiCore->floatToString(gameCore->player.inventory.getResource(RES_QUARTZ),2);
	silicon="Silicon: " + uiCore->floatToString(gameCore->player.inventory.getResource(RES_SILICON),2);
	uranium="Uranium: " + uiCore->floatToString(gameCore->player.inventory.getResource(RES_URANIUM),2);
	steel="Steel: " + uiCore->floatToString(gameCore->player.inventory.getResource(RES_STEEL),2);
	glass="Glass: " + uiCore->floatToString(gameCore->player.inventory.getResource(RES_GLASS),2);
	wiring="Wiring: " + uiCore->floatToString(gameCore->player.inventory.getResource(RES_WIRING),2);

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

	//Draw the close button
	close->Render();

	glBindTexture(GL_TEXTURE_2D,0);
	glColor3f(GLfloat(abs(power-1)),GLfloat(power),0);
	//Draw the power bar
	glBegin(GL_QUADS);
		glTexCoord2f(0.0f,1.0f);										glVertex2f(xM+30,yM+50); //LL
		glTexCoord2f((float)480.0f/512.0f,1.0f);						glVertex2f(xM+50,yM+50); //LR
		glTexCoord2f((float)480.0f/512.0f,1.0f-(float)421.0f/512.0f);	glVertex2f(xM+50,yM+51+(power*200)); //UR
		glTexCoord2f(0.0f,1.0f-(float)421.0f/512.0f);					glVertex2f(xM+30,yM+51+(power*200)); //UL
	glEnd();

	glColor3f(GLfloat(abs(air-1)),GLfloat(air),0);
	//Draw the air bar
	glBegin(GL_QUADS);
		glTexCoord2f(0.0f,1.0f);										glVertex2f(xM+80,yM+50); //LL
		glTexCoord2f((float)480.0f/512.0f,1.0f);						glVertex2f(xM+100,yM+50); //LR
		glTexCoord2f((float)480.0f/512.0f,1.0f-(float)421.0f/512.0f);	glVertex2f(xM+100,yM+51+(air*200)); //UR
		glTexCoord2f(0.0f,1.0f-(float)421.0f/512.0f);					glVertex2f(xM+80,yM+51+(air*200)); //UL
	glEnd();

	glColor4f(1.0f,1.0f,1.0f,1.0f);

	uiCore->renderText(xM+30,yM+260,"Power");
	uiCore->renderText(xM+80,yM+260,"Air");

	
	uiCore->renderText((xM+xP-50)/2,yP-25,"Resources");
	uiCore->renderText(6*(xP-xM)/10+xM,(yP+yM)/2+80,pow.c_str());
	uiCore->renderText(6*(xP-xM)/10+xM,(yP+yM)/2+60,food.c_str());
	uiCore->renderText(6*(xP-xM)/10+xM,(yP+yM)/2+40,water.c_str());
	uiCore->renderText(6*(xP-xM)/10+xM,(yP+yM)/2+20,coal.c_str());
	uiCore->renderText(6*(xP-xM)/10+xM,(yP+yM)/2,copper.c_str());
	uiCore->renderText(6*(xP-xM)/10+xM,(yP+yM)/2-20,iron.c_str());
	uiCore->renderText(6*(xP-xM)/10+xM,(yP+yM)/2-40,gold.c_str());
	uiCore->renderText(6*(xP-xM)/10+xM,(yP+yM)/2-60,platinum.c_str());
	uiCore->renderText(6*(xP-xM)/10+xM,(yP+yM)/2-80,quartz.c_str());
	uiCore->renderText(6*(xP-xM)/10+xM,(yP+yM)/2-100,silicon.c_str());
	uiCore->renderText(6*(xP-xM)/10+xM,(yP+yM)/2-120,steel.c_str());
	
}

/****************************
* clicked -	checks for additional clicks against the secondary GUI window
****************************/
tBOOL GUI_Resources::clicked(const s32& X,const s32& Y)
{	
	//Click was within the GUI's window
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