#include <Outland\Singletons.h>
#include <iomanip>

extern HDC hDC;

/****************************
* Constructor
****************************/
UserInputCore::UserInputCore():
	limitFPS(TRUE),
	textBase(0),
	build(0),
	buildType(0),
	gameState(0),
	firstPerson(FALSE),
	buildMark(0,7,0),
	build1(NULL),
	build2(NULL)
{}

/****************************
* Destructor
****************************/
UserInputCore::~UserInputCore()
{
	delete build1;
	delete build2;
}

UserInputCore* UserInputCore::Instance()
{
	static UserInputCore instance;
	return &instance;
}

/****************************
* Startup
****************************/
void UserInputCore::startup()
{
	logger->LogMessage(LOG_UICORE,"User Input Core initializing...\n");
	glEnable(GL_TEXTURE_2D);
	//Initialize the camera
	logger->LogMessage(LOG_UICORE,"Camera initializing...\n");
	UI_Camera.Init();
	logger->LogMessage(LOG_UICORE,"Camera successfully initialized\n");
	//if(OUTLAND_DEBUG==1)
	//{
	//	gameState=STATE_DEBUG;
	//}
	//else
	{
		//gameState=STATE_INTRO;		//sets the initial state to the Intro screens
		gameState=STATE_MAINMENU;
	}

	logger->LogMessage(1,"GUI System initializing...\n");
	UI_MMenu.Startup();
	UI_GUI.Startup();
	UI_LoadMenu.Startup();

	build1=NULL;
	build2=NULL;

	//sets up the text font
	textBase=setupText("Times New Roman",16);
}

/****************************
* update
****************************/
void UserInputCore::update(real32 time)
{
	switch(gameState)
	{
	case STATE_INTRO:
	
		break;
	case STATE_MAINMENU:
		UI_Input.Update();
		break;
	default:
		UI_Input.Update();
		break;
	}
}

/****************************
* shutdown
****************************/
void UserInputCore::shutdown()
{
	if(textBase!=0)
	{
		glDeleteLists(textBase,96);
	}
}

/****************************
* changeCam
****************************/
void UserInputCore::rotateCam(float roll, float yaw, float pitch)
{
	//passes the new data to the Camera Control class for changing
	if(roll !=0 || yaw !=0 || pitch!=0)
		UI_Camera.rotateCam(roll,yaw,pitch);
}

void UserInputCore::moveCam(float x,float y, float z)
{
	if(x !=0 || y!=0 || z!=0)
		UI_Camera.moveCam(x,y,z);
}

/****************************
* userInput
****************************/
void UserInputCore::userInput(bool value,int key)
{
	UI_Input.KeyPress(value,key);
}

/****************************
* mouseInput
****************************/
void UserInputCore::mouseInput(int x,int y)
{
	UI_Input.MouseMove(x,y);
}

/****************************
* getState -	Returns the game's current state
****************************/
s32 UserInputCore::getState()
{
	return gameState;
}

/****************************
* setState -	Sets the game's current state
****************************/
void UserInputCore::setState(s32 state)
{
	gameState=state;
}

/****************************
* setupText -	Sets up the text format for the game
****************************/
u32 UserInputCore::setupText(char* fontName,s32 fontSize)
{
	HFONT hfont;
	u32 base=glGenLists(96);

	if(_stricmp(fontName,"symbol")==0)
	{
		hfont=CreateFont(fontSize,0,0,0,FW_BOLD,FALSE,FALSE,FALSE,SYMBOL_CHARSET,OUT_TT_PRECIS,CLIP_DEFAULT_PRECIS,ANTIALIASED_QUALITY,FF_DONTCARE|DEFAULT_PITCH,fontName);
	}
	else
	{
		hfont=CreateFont(fontSize,0,0,0,FW_BOLD,FALSE,FALSE,FALSE,ANSI_CHARSET,OUT_TT_PRECIS,CLIP_DEFAULT_PRECIS,ANTIALIASED_QUALITY,FF_DONTCARE|DEFAULT_PITCH,fontName);
	}

	if(!hfont)
		return 0;

	SelectObject(hDC,hfont);
	wglUseFontBitmaps(hDC,32,96,base);

	return base;
}

/****************************
* renderText -	Renders the passed text at the specified screen coords
****************************/
void UserInputCore::renderText(s32 x,s32 y,const char* str)
{
	if(textBase==0 || !str)
		return;

	glRasterPos2i(x,y);

	glPushAttrib(GL_LIST_BIT);
		glListBase(textBase-32);
		glCallLists((s32)strlen(str),GL_UNSIGNED_BYTE,str);
	glPopAttrib();
}

/****************************
* intToString -	Converts integers into strings for displaying numerical data
****************************/
std::string UserInputCore::intToString(const s64& data)
{
	std::string val="";
	std::stringstream convert;
	convert << std::dec << data;
	
	val=convert.str();
	return val;
}

/****************************
* floatToString -	Converts floats into strings for displaying numerical data
****************************/
std::string UserInputCore::floatToString(const real64& data,const s32& precision)
{
	std::string val="";
	std::stringstream convert;
	convert << std::dec << std::fixed << std::setprecision(precision) << data;
	
	val=convert.str();
	return val;
}

/****************************
* toggleView -	Switches between 1st and 3rd person views
****************************/
void UserInputCore::toggleView()
{
	firstPerson=!firstPerson;
}

/****************************
* getView - Gets whether the view is in first person (true) or not
****************************/
bool UserInputCore::getView()
{
	return firstPerson;
}

/****************************
* setBuildMark -	Sets the building marker to a position (usually going to be player position)
****************************/
void UserInputCore::setBuildMark(Vector3<s64> pos)
{
	buildMark=pos;
}

/****************************
* moveBuildMark -	Moves the building marker by the passed position
****************************/
void UserInputCore::moveBuildMark(Vector3<s64> pos)
{
	buildMark+=pos;
}

/****************************
* getBuildMark -	Returns the position of the building marker
****************************/
Vector3<s64> UserInputCore::getBuildMark()
{
	return buildMark;
}

Vector3<s64> UserInputCore::getBuildMark1()
{
	return *build1;
}

Vector3<s64> UserInputCore::getBuildMark2()
{
	return *build2;
}

void UserInputCore::enterPos()
{
	if(build1==NULL)
	{
		build1=new Vector3<s64>(buildMark);
	}
	else if(build2==NULL)
	{
		build2=new Vector3<s64>(buildMark);
	}
	else
	{
		//sends build call to the building manager
		if(build==1)
		{
			gameCore->bMan.buildRoomGenerateGraph(*build1,*build2,buildType,TRUE,false);
		}
		else if(build==2)
		{
			gameCore->bMan.RemoveRect(*build1,*build2);
		}
		else if(build==3)
		{
			//Determine the differences in position
			Vector3<s64> pos(*build2-*build1);

			if(pos.x>0)
			{
				gameCore->bMan.addDoor(*build1,RM_DOOR6);
			}
			else if(pos.x<0)
			{
				gameCore->bMan.addDoor(*build1,RM_DOOR4);
			}
			else if(pos.z>0)
			{
				gameCore->bMan.addDoor(*build1,RM_DOOR2);
			}
			else if(pos.z<0)
			{
				gameCore->bMan.addDoor(*build1,RM_DOOR8);
			}
		}

		//clears the building markers
		clearMarks();

		//Clears the ghost room
		gameCore->bMan.clearGhostRoom();

		//sets the build mode to false
		build=FALSE;
	}
}

void UserInputCore::Render()
{
	if(build1&&build2)
		gameCore->bMan.buildRoomGenerateGraph(*build1,*build2,RM_GHOST,TRUE,true);
	//Block block(BlockType_MARKER,TRUE,Vector3<s32>(buildMark.x,buildMark.y,buildMark.z));

	//grCore->pRenderer->RenderTestCube(&block,buildMark.x,buildMark.y,buildMark.z);
}

tBOOL UserInputCore::getBuildMarkState(s32 marker)
{
	if(marker==1)
	{
		if(build1==NULL)
			return FALSE;
		else
			return TRUE;
	}
	else
	{
		if(build2==NULL)
			return FALSE;
		else
			return TRUE;
	}
}

void UserInputCore::setBuild2(Vector3<s64> npos)
{
	if(build2)
	{
		delete build2;
		build2=new Vector3<s64>(npos);
	}
	else
		build2=new Vector3<s64>(npos);
}

void UserInputCore::setBuild1(Vector3<s64> npos)
{
	if(build1)
	{
		delete build1;
		build1=new Vector3<s64>(npos);
	}
	else
		build1=new Vector3<s64>(npos);
}

void UserInputCore::clearMarks()
{
	if(build1)
	{
		delete build1;
		build1=NULL;
	}
	if(build2)
	{
		delete build2;
		build2=NULL;
	}
}