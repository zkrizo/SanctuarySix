#include "GUI.h"
#include <Outland\Singletons.h>

extern int RES_WIDTH;
extern int RES_HEIGHT;

/****************************
* Constructor
****************************/
GUI::GUI():
	curUI(0),
	Resources(NULL),
	Building(NULL),
	Research(NULL),
	View(NULL),
	System(NULL),
	background(NULL),
	xP(real32(RES_WIDTH/2.0f+(7*RES_HEIGHT/20))),
	xM(real32(RES_WIDTH/2.0f-(7*RES_HEIGHT/20))),
	yP(real32(RES_HEIGHT-39)),
	yM(real32(17*(RES_HEIGHT-39)/20)),
	xT(xP-xM),
	yT(yP-yM)
{}

/****************************
* Destructor
****************************/
GUI::~GUI()
{
	delete Resources;
	delete Building;
	delete Research;
	delete View;
	delete System;
}

void GUI::Startup()
{
	//Load the main GUI files
	background=grCore->pRenderer->getTexture(2,GUI_MAIN);
	Resources=new Button("Resources",3*xT/21,0.6*yT,xM+xT/21,yP-4*yT/5,grCore->pRenderer->getTexture(2,GUI_RESOURCES));
	Building=new Button("Building",3*xT/21,0.6*yT,xM+5*xT/21,yP-4*yT/5,grCore->pRenderer->getTexture(2,GUI_BUILDING));
	Research=new Button("Research",3*xT/21,0.6*yT,xM+9*xT/21,yP-4*yT/5,grCore->pRenderer->getTexture(2,GUI_TECH));
	View=new Button("View",3*xT/21,0.6*yT,xM+13*xT/21,yP-4*yT/5,grCore->pRenderer->getTexture(2,GUI_VIEW));
	System=new Button("System",3*xT/21,0.6*yT,xM+17*xT/21,yP-4*yT/5,grCore->pRenderer->getTexture(2,GUI_SYSTEM));
	
	curUI=UI_GAME;

	gMenu.Startup();
	gBuild.startup();
	gTech.Startup();
	gResource.Startup();
	gMenu.Startup();
}

/****************************
* Clicked
****************************/
void GUI::clicked(const s32& X,const s32& Y)
{
	
	tBOOL ray=FALSE;
	
	//Invert Y
	s32 newY=(RES_HEIGHT-30)-Y;
	
	//checks to see if the click was within the GUI's bounds
	if(X>=xM && X<=xP)
	{
		if(newY>=yM && newY<=yP)
		{
			ray=TRUE;

			//Main GUI was clicked, check against buttons
			if(Resources->clicked(X,newY))	//resources button
			{
				//if the current UI is resources, toggle the window off by switching the current UI, else show the window
				if(curUI==UI_RESOURCES)
				{
					curUI=UI_GAME;
				}
				else
				{
					curUI=UI_RESOURCES;
				}
			}
			else if(Building->clicked(X,newY))	//building button
			{
				//if the current UI is building, toggle the window off by switching the current UI, else show the window
				if(curUI==UI_BUILDING)
				{
					curUI=UI_GAME;
				}
				else
				{
					curUI=UI_BUILDING;
				}
			}
			else if(Research->clicked(X,newY))	//tech button
			{
				//if the current UI is tech, toggle the window off by switching the current UI, else show the window
				if(curUI==UI_TECH)
				{
					curUI=UI_GAME;
				}
				else
				{
					curUI=UI_TECH;
				}
			}
			else if(View->clicked(X,newY))	//view button
			{
				//send view switching code
				uiCore->toggleView();
				uiCore->UI_Camera.hasMoved(TRUE);
			}
			else if(System->clicked(X,newY))	//Menu button
			{
				//if the current UI is resources, toggle the window off by switching the current UI, else show the window
				if(curUI==UI_MENU)
				{
					curUI=UI_GAME;
				}
				else
				{
					curUI=UI_MENU;
				}
			}
		}
		else if(curUI!=UI_GAME)
		{
			//additional UI is active, send call to active GUI
			switch(curUI)
			{
				case UI_RESOURCES:
					ray=gResource.clicked(X,newY);
					break;
				case UI_BUILDING:
					ray=gBuild.clicked(X,newY);
					break;
				case UI_TECH:
					ray=gTech.clicked(X,newY);
					break;
				case UI_MENU:
					ray=gMenu.clicked(X,newY);
					break;
			}
		}
	}
	else if(curUI!=UI_GAME)
	{
		//additional UI is active, send call to active GUI
		switch(curUI)
		{
			case UI_RESOURCES:
				ray=gResource.clicked(X,newY);
				break;
			case UI_BUILDING:
				ray=gBuild.clicked(X,newY);
				break;
			case UI_TECH:
				ray=gTech.clicked(X,newY);
				break;
			case UI_MENU:
				ray=gMenu.clicked(X,newY);
				break;
		}
	}

	if(uiCore->UI_Tutorial.click(X,newY))
		ray=TRUE;


	//Click was within the game world, run ray tracing
	if(ray==FALSE)
	{
		uiCore->UI_Camera.rayTrace(X,newY);
	}
}


/****************************
* Render - Render's the game's GUI
****************************/
void GUI::Render()
{
	glColor3f(1.0f,1.0f,1.0f);
	
	//Binds the texture for the title
	glBindTexture(GL_TEXTURE_2D,*background);

	//Position of Main GUI element on top of screen
	glBegin(GL_QUADS);
		glNormal3f(0.0f,0.0f,-1.0f);
		glTexCoord2i(1,0);	glVertex2f(xP,yM);		//Lower Right
		glTexCoord2i(1,1);	glVertex2f(xP,yP);		//Upper Right
		glTexCoord2i(0,1);	glVertex2f(xM,yP);		//Upper Left
		glTexCoord2i(0,0);	glVertex2f(xM,yM);		//Lower Left
	glEnd();

	//Binds the texture for the button
	Resources->Render();

	//Binds the texture for the button
	Building->Render();

	//Binds the texture for the button
	Research->Render();

	//Binds the texture for the button
	View->Render();

	//Binds the texture for the button
	System->Render();
	
	switch(curUI)
	{
	case UI_GAME:
		break;
	case UI_RESOURCES:
		gResource.Render();
		break;
	case UI_BUILDING:
		gBuild.Render();
		break;
	case UI_TECH:
		gTech.Render();
		break;
	case UI_MENU:
		gMenu.Render();
		break;
	}
	
	if(uiCore->build==1)
	{
		//Enables window transparency
		glColor4f(1.0f,1.0f,1.0f,0.8f);

		//Bind the secondary window background texture
		glBindTexture(GL_TEXTURE_2D,*background);

		//Draw the window background
		glBegin(GL_QUADS);
			glTexCoord2f(0.0f,1.0f);		glVertex2f(GLfloat(RES_WIDTH-(2*RES_WIDTH/10)),0); //LL
			glTexCoord2f((float)480.0f/512.0f,1.0f);		glVertex2f(GLfloat(RES_WIDTH),0); //LR
			glTexCoord2f((float)480.0f/512.0f,1.0f-(float)421.0f/512.0f);	glVertex2f(GLfloat(RES_WIDTH),GLfloat((3.5*RES_WIDTH/20))); //UR
			glTexCoord2f(0.0f,1.0f-(float)421.0f/512.0f);		glVertex2f(RES_WIDTH-(2*RES_WIDTH/10),GLfloat((3.5*RES_WIDTH/20))); //UL
		glEnd();

		glColor4f(1.0f,1.0f,1.0f,1.0f);
		glBindTexture(GL_TEXTURE_2D,0);

		//Render text context
		if(uiCore->getBuildMarkState(1)==FALSE)
		{
			uiCore->renderText(RES_WIDTH-(2*RES_WIDTH/10)+25,0+(3.5*RES_WIDTH/20)-15,"Place First Corner");
		}
		else if(uiCore->getBuildMarkState(2)==FALSE)
		{

		}
		else
		{
			int size,rneeded;
			Vector3<s64> total=uiCore->getBuildMark2()-uiCore->getBuildMark1();
			
			if(total.x<0)
				total.x=total.x*-1;
			if(total.y<0)
				total.y=total.y*-1;
			if(total.z<0)
				total.z=total.z*-1;

			total=total+Vector3<s64>(1,1,1);
			size=total.x*total.y*total.z;
			rneeded=gameCore->resourceMan.calcResources(uiCore->buildType,size,RES_STEEL);
			std::string rn,sz;

			rn="Max Resources Needed: ";
			sz="Size: " + uiCore->intToString(size);

			uiCore->renderText(RES_WIDTH-(2*RES_WIDTH/10)+25,0+(3.5*RES_WIDTH/20)-15,"Release to build room");
			uiCore->renderText(RES_WIDTH-(2*RES_WIDTH/10)+10,0+(3.5*RES_WIDTH/20)-35,sz.c_str());
			uiCore->renderText(RES_WIDTH-(2*RES_WIDTH/10)+10,0+(3.5*RES_WIDTH/20)-55,rn.c_str());

			if(gameCore->resourceMan.checkResource(RES_STEEL,"Default")>=rneeded)
				glColor3f(0.0f,1.0f,0.0f);
			else
				glColor3f(1.0f,0.0f,0.0f);

			uiCore->renderText(RES_WIDTH-(2*RES_WIDTH/10)+10,0+(3.5*RES_WIDTH/20)-75,uiCore->intToString(rneeded).c_str());

			glColor3f(1.0f,1.0f,1.0f);
		}
	}
	else if(uiCore->build==2)
	{
		//Enables window transparency
		glColor4f(1.0f,1.0f,1.0f,0.8f);

		//Bind the secondary window background texture
		glBindTexture(GL_TEXTURE_2D,*grCore->pRenderer->getTexture(2,6));

		//Draw the window background
		glBegin(GL_QUADS);
			glTexCoord2f(0.0f,1.0f);		glVertex2f(GLfloat(RES_WIDTH-(2*RES_WIDTH/10)),0); //LL
			glTexCoord2f((float)480.0f/512.0f,1.0f);		glVertex2f(GLfloat(RES_WIDTH),0); //LR
			glTexCoord2f((float)480.0f/512.0f,1.0f-(float)421.0f/512.0f);	glVertex2f(GLfloat(RES_WIDTH),GLfloat((RES_WIDTH/20))); //UR
			glTexCoord2f(0.0f,1.0f-(float)421.0f/512.0f);		glVertex2f(GLfloat(RES_WIDTH-(2*RES_WIDTH/10)),GLfloat((RES_WIDTH/20))); //UL
		glEnd();

		glColor4f(1.0f,1.0f,1.0f,1.0f);
	}

	//Sends render call to the tutorial
	uiCore->UI_Tutorial.Render();

	//unbinds the textures   
	glBindTexture(GL_TEXTURE_2D,0);

#ifdef _DEBUG
	//render debug text
	uiCore->renderText(0,RES_HEIGHT-39-14,"Test Debug String:");
	uiCore->renderText(0,RES_HEIGHT-39-28,"Camera pos: ");
	uiCore->renderText(0,RES_HEIGHT-39-42,uiCore->UI_Camera.cstr);
	
	uiCore->renderText(0,RES_HEIGHT-39-56,uiCore->UI_Camera.upV);
	uiCore->renderText(0,RES_HEIGHT-39-70,uiCore->UI_Camera.viewV);

	string mouseX("MouseX: " + uiCore->intToString(uiCore->UI_Input.getMouseX()));
	string mouseY("MouseY: " + uiCore->intToString(uiCore->UI_Input.getMouseY()));

	uiCore->renderText(0,RES_HEIGHT-39-84,mouseX.c_str());
	uiCore->renderText(0,RES_HEIGHT-39-98,mouseY.c_str());


#endif
}

/****************************
* resetUI -	Resets the current UI to game mode
****************************/
void GUI::ResetUI()
{
	curUI=UI_GAME;
}

/****************************
* setGUI -	Sets the current UI to passed GUI or toggles it closed
****************************/
void GUI::setGUI(const s32& UI)
{
	switch(UI)
	{
	case UI_RESOURCES:
		if(curUI==UI)
			curUI=UI_GAME;
		else
			curUI=UI;
		break;
	case UI_BUILDING:
		if(curUI==UI)
			curUI=UI_GAME;
		else
			curUI=UI;
		break;
	case UI_TECH:
		if(curUI==UI)
			curUI=UI_GAME;
		else
			curUI=UI;
		break;
	case UI_MENU:
		if(curUI==UI)
			curUI=UI_GAME;
		else
			curUI=UI;
		break;
	}
}