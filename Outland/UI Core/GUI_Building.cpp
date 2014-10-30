#include "GUI_Building.h"
#include <Outland\Singletons.h>

extern int RES_WIDTH;
extern int RES_HEIGHT;

/****************************
* Constructor
****************************/
GUI_Building::GUI_Building():
	curtab(1),
	curbuild(RM_HALLWAY),
	background(NULL),
	secondbackground(NULL),
	curInfo(NULL),
	Build(NULL),
	Demolish(NULL),
	Select(NULL),
	Hallway(NULL),
	CommandCenter(NULL),
	LivingQuarters(NULL),
	Bathroom(NULL),
	Airlock(NULL),
	FreightAirlock(NULL),
	Galley(NULL),
	IndustrialWorkshop(NULL),
	Laboratory(NULL),
	Library(NULL),
	MCC(NULL),
	PowerPlant(NULL),
	Warehouse(NULL),
	close(NULL),
	tabTech1(NULL),
	tabTech2(NULL),
	tabTech3(NULL),
	tabTech4(NULL),
	tabObjects(NULL),
	ObjDoor(NULL),
	xP(real32(RES_WIDTH/2.0f+(8*RES_HEIGHT/20))),
	xM(real32(RES_WIDTH/2.0f-(8*RES_HEIGHT/20))),
	yP(real32(16*(RES_HEIGHT-38)/20)),
	yM(real32(1*(RES_HEIGHT-38)/20)),
	xT(real32(xP-xM)),
	yT(real32(yP-yM)),
	info1("                    \0"),
	info2("                    \0"),
	info3("                    \0"),
	info4("                    \0")
{}

/****************************
* Destructor
****************************/
GUI_Building::~GUI_Building()
{
	delete Build;
	delete Demolish;
	delete Select;
	delete Hallway;
	delete CommandCenter;
	delete LivingQuarters;
	delete Bathroom;
	delete Airlock;
	delete FreightAirlock;
	delete Galley;
	delete IndustrialWorkshop;
	delete Laboratory;
	delete Library;
	delete MCC;
	delete PowerPlant;
	delete Warehouse;
	delete close;
	delete tabTech1;
	delete tabTech2;
	delete tabTech3;
	delete tabTech4;
	delete tabObjects;
	delete ObjDoor;
}


void GUI_Building::startup()
{
	background=grCore->pRenderer->getTexture(2,GUI_WINDOW);
	secondbackground=grCore->pRenderer->getTexture(2,GUI_SECONDWINDOW);
	curInfo=gameCore->bMan.T1Hallway;

	Build=new Button("Build",(2*xT/20),(xT/20),xP-(11*xT/20),yM+(1*yT/20),grCore->pRenderer->getTexture(2,GUI_BUILD));
	Demolish=new Button("Demolish",(2*xT/20),(xT/20),xP-(8*xT/20),yM+(1*yT/20),grCore->pRenderer->getTexture(2,GUI_DEMOLISH));
	Select=new Button("Select",2*xT/20,2*xT/20,xP-(13*xT/20),yP-(5*yT/20),grCore->pRenderer->getTexture(2,GUI_SELECTICON));
	
	Hallway=new Button("Hallway",2*xT/20,2*xT/20,xP-(13*xT/20),yP-(5*yT/20),grCore->pRenderer->getTexture(2,GUI_HALLWAYICON));
	CommandCenter=new Button("Command Center",2*xT/20,2*xT/20,xP-(10*xT/20),yP-(5*yT/20),grCore->pRenderer->getTexture(2,GUI_COMMANDICON));
	LivingQuarters=new Button("Living Quarters",2*xT/20,2*xT/20,xP-(7*xT/20),yP-(5*yT/20),grCore->pRenderer->getTexture(2,GUI_LIVINGICON));
	Bathroom=new Button("Bathroom",2*xT/20,2*xT/20,xP-(4*xT/20),yP-(5*yT/20),grCore->pRenderer->getTexture(2,GUI_BATHROOMICON));
	
	Airlock=new Button("Airlock",2*xT/20,2*xT/20,xP-(13*xT/20),yP-(8*yT/20),grCore->pRenderer->getTexture(2,GUI_AIRLOCKICON));
	FreightAirlock=new Button("Freight Airlock",2*xT/20,2*xT/20,xP-(10*xT/20),yP-(8*yT/20),grCore->pRenderer->getTexture(2,GUI_FREIGHTAIRLOCKICON));
	Galley=new Button("Galley",2*xT/20,2*xT/20,xP-(7*xT/20),yP-(8*yT/20),grCore->pRenderer->getTexture(2,GUI_GALLEYICON));
	IndustrialWorkshop=	new Button("Industrial Workshop",2*xT/20,2*xT/20,xP-(4*xT/20),yP-(8*yT/20),grCore->pRenderer->getTexture(2,GUI_INDUSTRIALWORKSHOPICON));
	
	Laboratory=new Button("Laboratory",2*xT/20,2*xT/20,xP-(13*xT/20),yP-(11*yT/20),grCore->pRenderer->getTexture(2,GUI_LABORATORYICON));
	Library=new Button("Library",2*xT/20,2*xT/20,xP-(10*xT/20),yP-(11*yT/20),grCore->pRenderer->getTexture(2,GUI_LIBRARYICON));
	MCC=new Button("Mining Control Center",2*xT/20,2*xT/20,xP-(7*xT/20),yP-(11*yT/20),grCore->pRenderer->getTexture(2,GUI_MCCICON));
	PowerPlant=new Button("Power Plant",2*xT/20,2*xT/20,xP-(4*xT/20),yP-(11*yT/20),grCore->pRenderer->getTexture(2,GUI_POWERPLANTICON));

	Warehouse=new Button("Warehouse",2*xT/20,2*xT/20,xP-(13*xT/20),yP-(14*yT/20),grCore->pRenderer->getTexture(2,GUI_WAREHOUSEICON));
	
	close=new Button("Close",(xT/10),(2*xT/40),xP-(6*xT/40),yM+(yT/20),grCore->pRenderer->getTexture(2,GUI_CLOSE));
	tabTech1=new Button("Tech 1",(xT/10),(xT/20),xM+xT/20,yP-yT/10,grCore->pRenderer->getTexture(2,GUI_TECH1TAB));
	tabTech2=new Button("Tech 2",(xT/10),(xT/20),xM+3*xT/20,yP-yT/10,grCore->pRenderer->getTexture(2,GUI_TECH2TAB));
	tabTech3=new Button("Tech 3",(xT/10),(xT/20),xM+5*xT/20,yP-yT/10,grCore->pRenderer->getTexture(2,GUI_TECH3TAB));
	tabTech4=new Button("Tech 4",(xT/10),(xT/20),xM+7*xT/20,yP-yT/10,grCore->pRenderer->getTexture(2,GUI_TECH4TAB));
	tabObjects=new Button("Objects",(xT/10),(xT/20),xM+9*xT/20,yP-yT/10,grCore->pRenderer->getTexture(2,GUI_OBJECTTAB));
	ObjDoor=new Button("Door",2*xT/20,2*xT/20,xP-(13*xT/20),yP-(5*yT/20),grCore->pRenderer->getTexture(2,GUI_DOORICON));

	updateInfoOutput();
}

/****************************
* Render
****************************/
void GUI_Building::Render()
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

	glColor4f(0.7f,0.7f,0.7f,0.6f);

	//Draw the secondary background
	glBindTexture(GL_TEXTURE_2D,*secondbackground);
	glBegin(GL_QUADS);
		glTexCoord2f(0.0f,0.0f);		glVertex2f(xM+xT/20,yM+1.5*yT/10); //LL
		glTexCoord2f(1.0f,0.0f);		glVertex2f(xP-xT/20,yM+1.5*yT/10); //LR
		glTexCoord2f(1.0f,1.0f);		glVertex2f(xP-xT/20,yP-yT/10); //UR
		glTexCoord2f(0.0f,1.0f);		glVertex2f(xM+xT/20,yP-yT/10); //UL
	glEnd();

	
	glColor4f(1.0f,1.0f,1.0f,0.8f);

	//Switch to Close button texture
	glBindTexture(GL_TEXTURE_2D,*close->texture);

	//Draw the close button
	close->Render();

	Build->Render();
	Demolish->Render();

	if(curtab==1)
	{
		//Calls the renderers for the buttons
		CommandCenter->Render();
		Bathroom->Render();
		Hallway->Render();
		LivingQuarters->Render();
		Airlock->Render();
		FreightAirlock->Render();
		Galley->Render();
		IndustrialWorkshop->Render();
		Laboratory->Render();
		Library->Render();
		MCC->Render();
		PowerPlant->Render();
		Warehouse->Render();

		//Renders the selection box
		Select->Render();

		tabTech1->Render();

		glColor4f(0.6f,0.6f,0.6f,0.8f);

		tabTech2->Render();
		tabTech3->Render();
		tabTech4->Render();
		tabObjects->Render();
		
		glBindTexture(GL_TEXTURE_2D,0);
		//Renders the building info
		//uiCore->renderText(xM+xT/20,yM+yT/2,info1.c_str());
		//uiCore->renderText(xM+xT/20,yM+yT/2-15,info2.c_str());
		//uiCore->renderText(xM+xT/20,yM+yT/2-30,info3.c_str());
		//uiCore->renderText(xM+xT/20,yM+yT/2-45,info4.c_str());
	}
	else if(curtab==2)
	{
		tabTech2->Render();

		glColor4f(0.6f,0.6f,0.6f,0.8f);
		tabTech1->Render();
		tabTech3->Render();
		tabTech4->Render();
		tabObjects->Render();
	}
	else if(curtab==3)
	{
		tabTech3->Render();

		glColor4f(0.6f,0.6f,0.6f,0.8f);

		tabTech1->Render();
		tabTech2->Render();
		tabTech4->Render();
		tabObjects->Render();
	}
	else if(curtab==4)
	{
		tabTech4->Render();

		glColor4f(0.6f,0.6f,0.6f,0.8f);

		tabTech1->Render();
		tabTech2->Render();
		tabTech3->Render();
		tabObjects->Render();
	}
	else if(curtab==5)
	{
		ObjDoor->Render();
		Select->Render();

		tabObjects->Render();

		glColor4f(0.6f,0.6f,0.6f,0.8f);

		tabTech1->Render();
		tabTech2->Render();
		tabTech3->Render();
		tabTech4->Render();
	}

	//resets the alpha
	glColor4f(1.0f,1.0f,1.0f,1.0f);
}

/****************************
* clicked -	checks for additional clicks against the secondary GUI window
****************************/
tBOOL GUI_Building::clicked(const s32& X,const s32& Y)
{
	//Click was within the GUI's window
	if((X>=xM && X<=xP) && (Y>=yM && Y<=yP))
	{
		//check for click on the close button
		if(close->clicked(X,Y))
		{
			//resets the current UI to UI_GAME thus closing the window
			uiCore->UI_GUI.ResetUI();
			return TRUE;
		}

		//check for click on the Build button
		else if(Build->clicked(X,Y))
		{
			uiCore->UI_GUI.ResetUI();
			gameCore->bMan.clearGhostRoom();
			uiCore->clearMarks();
					
			uiCore->buildType=curbuild;
			if(curtab==1)
			{
				//Turn on the build mode
				uiCore->build=1;
				return TRUE;
			}
			else if(curtab==5)
			{
				//Turn on special build mode
				if(Select->posX==ObjDoor->posX && Select->posY==ObjDoor->posY)
				{
					uiCore->build=3;
					return TRUE;
				}
			}
		}
			
		//check for click on Demolish button
		else if(Demolish->clicked(X,Y))
		{
			uiCore->UI_GUI.ResetUI();
			gameCore->bMan.clearGhostRoom();

			uiCore->build=2;
			return TRUE;
		}

		if(tabTech1->clicked(X,Y))
		{
			curtab=1;
			Select->posX=Hallway->posX;
			Select->posY=Hallway->posY;
		}
		else if(tabTech2->clicked(X,Y))
		{
			curtab=2;
		}
		else if(tabTech3->clicked(X,Y))
		{
			curtab=3;
		}
		else if(tabTech4->clicked(X,Y))
		{
			curtab=4;
		}
		else if(tabObjects->clicked(X,Y))
		{
			curtab=5;
			Select->posX=ObjDoor->posX;
			Select->posY=ObjDoor->posY;
		}

		//Check against selecting the tabs
		else if(curtab==1)
		{
			if(Hallway->clicked(X,Y))
			{
				curbuild=RM_HALLWAY;
				curInfo=gameCore->bMan.T1Hallway;
				updateInfoOutput();
				Select->posX=Hallway->posX;
				Select->posY=Hallway->posY;
				return TRUE;
			}
			else if(LivingQuarters->clicked(X,Y))
			{
				curbuild=RM_LIVINGQUARTERS;
				curInfo=gameCore->bMan.T1LivingQuarters;
				updateInfoOutput();
				Select->posX=LivingQuarters->posX;
				Select->posY=LivingQuarters->posY;
				return TRUE;
			}
			else if(Bathroom->clicked(X,Y))
			{
				curbuild=RM_BATHROOM;
				curInfo=gameCore->bMan.T1Bathrooms;
				updateInfoOutput();
				Select->posX=Bathroom->posX;
				Select->posY=Bathroom->posY;
				return TRUE;
			}
			else if(CommandCenter->clicked(X,Y))
			{
				curbuild=RM_COMMANDCENTER;
				curInfo=gameCore->bMan.T1CommandCenter;
				updateInfoOutput();
				Select->posX=CommandCenter->posX;
				Select->posY=CommandCenter->posY;
				return TRUE;
			}
			else if(Airlock->clicked(X,Y))
			{
				curbuild=RM_AIRLOCK;
				curInfo=gameCore->bMan.T1Airlock;
				updateInfoOutput();
				Select->posX=Airlock->posX;
				Select->posY=Airlock->posY;
				return TRUE;
			}
			else if(FreightAirlock->clicked(X,Y))
			{
				curbuild=RM_FREIGHTAIRLOCK;
				curInfo=gameCore->bMan.T1FreightAirlock;
				updateInfoOutput();
				Select->posX=FreightAirlock->posX;
				Select->posY=FreightAirlock->posY;
				return TRUE;
			}
			else if(Galley->clicked(X,Y))
			{
				curbuild=RM_GALLEY;
				curInfo=gameCore->bMan.T1Galley;
				updateInfoOutput();
				Select->posX=Galley->posX;
				Select->posY=Galley->posY;
				return TRUE;
			}
			else if(IndustrialWorkshop->clicked(X,Y))
			{
				curbuild=RM_INDUSTRIALWORKSHOP;
				curInfo=gameCore->bMan.T1IndustrialWorkshop;
				updateInfoOutput();
				Select->posX=IndustrialWorkshop->posX;
				Select->posY=IndustrialWorkshop->posY;
				return TRUE;
			}
			else if(Laboratory->clicked(X,Y))
			{
				curbuild=RM_LABORATORY;
				curInfo=gameCore->bMan.T1Laboratory;
				updateInfoOutput();
				Select->posX=Laboratory->posX;
				Select->posY=Laboratory->posY;
				return TRUE;
			}
			else if(Library->clicked(X,Y))
			{
				curbuild=RM_LIBRARY;
				curInfo=gameCore->bMan.T1Library;
				updateInfoOutput();
				Select->posX=Library->posX;
				Select->posY=Library->posY;
				return TRUE;
			}
			else if(MCC->clicked(X,Y))
			{
				curbuild=RM_MININGCONTROLCENTER;
				curInfo=gameCore->bMan.T1MiningControlCenter;
				updateInfoOutput();
				Select->posX=MCC->posX;
				Select->posY=MCC->posY;
				return TRUE;
			}
			else if(PowerPlant->clicked(X,Y))
			{
				curbuild=RM_POWERFUSION;
				curInfo=gameCore->bMan.T1PowerFusion;
				updateInfoOutput();
				Select->posX=PowerPlant->posX;
				Select->posY=PowerPlant->posY;
				return TRUE;
			}
			else if(Warehouse->clicked(X,Y))
			{
				curbuild=RM_WAREHOUSE;
				curInfo=gameCore->bMan.T1Warehouse;
				updateInfoOutput();
				Select->posX=Warehouse->posX;
				Select->posY=Warehouse->posY;
				return TRUE;
			}
		}
		else if(curtab==2)
		{

		}
		else if(curtab==3)
		{

		}
		else if(curtab==4)
		{

		}
		else if(curtab==5)
		{
			if(ObjDoor->clicked(X,Y))
			{
				Select->posX=ObjDoor->posX;
				Select->posY=ObjDoor->posY;

			}
		}

		return TRUE;
	}

	return FALSE;
}

void GUI_Building::updateInfoOutput()
{
	int i=0;
	int size=curInfo->description.size();
	for(i=0;i<80;i++)
	{
		if(i<20)
		{
			info1[i]=i<size? curInfo->description[i] : ' ';
		}
		else if(i<40)
		{
			info2[i-20]=i<size? curInfo->description[i] : ' ';
		}
		else if(i<60)
		{
			
			info3[i-40]=i<size? curInfo->description[i] : ' ';
		}
		else
		{
			
			info4[i-60]=i<size? curInfo->description[i] : ' ';
		}
	}
}