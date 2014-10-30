#include "MainMenu.h"
#include <Outland\Singletons.h>

extern int RES_WIDTH;
extern int RES_HEIGHT;

/****************************
* Constructor
****************************/
Menu::Menu():
	cX(real32(RES_WIDTH/2)),
	cY(real32((RES_HEIGHT-39)/2)),
	xP(real32(RES_WIDTH/2.0f+(7*RES_HEIGHT/20))),
	xM(real32(RES_WIDTH/2.0f-(7*RES_HEIGHT/20))),
	yP(real32(RES_HEIGHT-39)),
	yM(real32(0)),
	xT(real32(RES_WIDTH/5)),
	yT(real32(RES_WIDTH/10))
{
}

/****************************
* Destructor
****************************/
Menu::~Menu()
{
	delete NewU;
	delete LoadU;
	delete Multi;
	delete Options;
	delete Quit;
}

/****************************
* Startup
****************************/
void Menu::Startup()
{
	titleTexture=grCore->pRenderer->getTexture(1,0);
	NewU=new Button("New Universe",2*xT,yT,cX-xT,yP-2.5*yT,grCore->pRenderer->getTexture(1,1));
	LoadU=new Button("Load Universe",2*xT,yT,cX-xT,yP-3.5*yT,grCore->pRenderer->getTexture(1,2));
	Multi=new Button("Multiplayer",2*xT,yT,cX-xT,yP-4.5*yT,grCore->pRenderer->getTexture(1,3));
	Options=new Button("Options",2*xT,yT,cX-xT,yP-5.5*yT,grCore->pRenderer->getTexture(1,4));
	Quit=new Button("Quit",2*xT,yT,cX-xT,yP-6.5*yT,grCore->pRenderer->getTexture(1,5));
}

/****************************
* Clicked
****************************/
void Menu::clicked(real32 X, real32 Y)
{
	//Invert Y
	Y=(RES_HEIGHT-30)-Y;

	//checks to see if NewU button was clicked
	if(NewU->clicked(X,Y))
	{
		gameCore->levMan.startGame(true);
		//testDraw=TRUE;
		color.x=1;
		color.y=0;
		color.z=0;
	}
	//checks to see if LoadU button was clicked
	else if(LoadU->clicked(X,Y))
	{
		gameCore->levMan.startGame(false);

		testDraw=TRUE;
		color.x=0;
		color.y=1;
		color.z=0;
	}
	//checks to see if Multi button was clicked
	else if(Multi->clicked(X,Y))
	{
		//Brings up menu for saved IP addresses for connecting to a multiplayer server. Not in this version yet, button will be disabled and greyed
		//out to show that.
		testDraw=TRUE;
		color.x=0;
		color.y=0;
		color.z=1;
	}
	//checks to see if Quit button was clicked
	else if(Quit->clicked(X,Y))
	{
		//Sends quit message
		testDraw=TRUE;
		color.x=0;
		color.y=1;
		color.z=1;

		//Posts quit message to windows handler
		PostQuitMessage(0);
	}
	//checks to see if Options button was clicked
	else if(Options->clicked(X,Y))
	{
		//Brings up a list of worldwide options. These affect ALL worlds so it will mainly be graphical options like render distance, texture quality
		//antialiasing, etc.
		testDraw=TRUE;
		color.x=1;
		color.y=1;
		color.z=0;
	}
	else
	{
		testDraw=FALSE;
	}
}


/****************************
* Render - Render's the game's menu
****************************/
void Menu::Render()
{

	uiCore->UI_Camera.ReverseCamMove();
	glPushMatrix();
	//switches to 2D mode
	grCore->pRenderer->ready2D();
	
	//enables texturing and filling modes
	glEnable(GL_TEXTURE_2D);
	glPolygonMode(GL_FRONT,GL_FILL);
	glColor3f(1.0f,1.0f,1.0f);
	
	//Binds the texture for the title
	glBindTexture(GL_TEXTURE_2D,*titleTexture);

	//draw the title bar
	glBegin(GL_QUADS);
		glNormal3f(0.0f,0.0f,-1.0f);
		glTexCoord2f(1.0f,0.0f);	glVertex2f(GLfloat(cX+(3*RES_WIDTH/10)),GLfloat(yP-(1.5*RES_WIDTH/10)));		//Lower Right
		glTexCoord2f(1.0f,1.0f);	glVertex2f(GLfloat(cX+(3*RES_WIDTH/10)),GLfloat(yP));				//Upper Right
		glTexCoord2f(0.0f,1.0f);	glVertex2f(GLfloat(cX-(3*RES_WIDTH/10)),GLfloat(yP));				//Upper Left
		glTexCoord2f(0.0f,0.0f);	glVertex2f(GLfloat(cX-(3*RES_WIDTH/10)),GLfloat(yP-(1.5*RES_WIDTH/10)));		//Lower Left
	glEnd();

	NewU->Render();
	LoadU->Render();
	
	//Multi->Render();
	//Options->Render();

	Quit->Render();

	glBindTexture(GL_TEXTURE_2D,0);

	/*if(testDraw)
	{
		
		glColor3f(color.x,color.y,color.z);
		glBegin(GL_QUADS);
			glNormal3f(0.0f,0.0f,-1.0f);
			//glTexCoord2f(1.0f,0.0f);	glVertex3f(epX,epY-(3*sY),-1);	//Lower Right
			//glTexCoord2f(1.0f,1.0f);	glVertex3f(epX,epY-(2*sY),-1);	//Upper Right
			//glTexCoord2f(0.0f,1.0f);	glVertex3f(epX-(sX),epY-(2*sY),-1);	//Upper Left
			//glTexCoord2f(0.0f,0.0f);	glVertex3f(epX-(sX),epY-(3*sY),-1);	//Lower Left
		glEnd();
	}*/
	
	//switches back to 3D
	grCore->pRenderer->ready3D();
}
