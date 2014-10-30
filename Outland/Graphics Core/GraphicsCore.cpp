#include <Windows.h>
#include <gl\glew.h>
#include <iostream>
#include <Outland\Singletons.h>

/****************************
* External Declarations
****************************/

//Window Variables
extern HINSTANCE hInst;
extern HWND wndHandle;

//OpenGL global variables for rendering context and device context and pixel format respectively
extern HGLRC	hRC;
extern HDC		hDC;
extern PIXELFORMATDESCRIPTOR pfd;

//resolution variables
extern int RES_WIDTH;
extern int RES_HEIGHT;
extern bool fullscreen;

GraphicsCore* GraphicsCore::Instance()
{
	static GraphicsCore instance;
	return &instance;
}

GraphicsCore::GraphicsCore():
	seed(0),
	centerPos(0,0,0),
	rover(NULL)
{}

GraphicsCore::~GraphicsCore()
{
	if(rover)
		delete rover;
}

/****************************
* Startup
****************************/
void GraphicsCore::startup()
{
	/*	Startup List:
		0. Initialize OpenGL
		{ - LEVEL LOADING CLASS NOT GRAPHIC CORE STARTUP! -
		1. Determine Player Position from last save 
			a. if New Game, determine starting position (Full Version - pull up planet selection, then map overview, then the landing site
		2. Load chunks around player location and cull progressively
		3. Load scenery objects like resource nodes and plants
		4. Load building information
		}
	*/

	logger->LogMessage(LOG_OPENGL,"Graphics Core starting...\nOpenGL Initializing...\n");

	//initialize openGL functions
	glEnable(GL_TEXTURE_2D);
	glShadeModel(GL_SMOOTH);
	glDepthFunc(GL_LEQUAL);	
	glEnable(GL_DEPTH_TEST);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_BLEND);

	logger->LogMessage(LOG_OPENGL,"OpenGL Initialized. GLEW Library starting initialization.\n");

	//Sets up the GLEW extension library
	GLenum err=glewInit();
	if(GLEW_OK !=err)
	{
		std::cout << "Error loading GLEW library!\n";
		logger->LogMessage(LOG_OPENGL,"Critical Error! GLEW library failed to initialize correctly!\n");
	}
	else
	{
		std::cout << "GLEW library loaded successfully!\n";
		logger->LogMessage(LOG_OPENGL,"GLEW library has initialized correctly!\n");
	}

	//set up OpenGL Renderer
	pRenderer=new OpenGLRenderer;
	pRenderer->startup();


	resetBuffers();	
	pRenderer->ready3D();

	SwapBuffers(hDC);
}

/****************************
* Update
****************************/
void GraphicsCore::update(real32 time)
{
	//reset the buffers
	resetBuffers();	
	
	//calls the camera's update function to get new positioning matrices
	uiCore->UI_Camera.Update();	
	
	/*	Update List:
		1. Call Chunk Manager's update function to check the chunks and see if anything needs loaded or changed.
		2. Render chunks that are loaded (call Chunk Manager's render function)
	*/
	//Testing, real functions not called

	//Checks what game mode the game is currently in
	switch(uiCore->getState())
	{
	case STATE_INTRO:

		break;
	case STATE_MAINMENU:
		uiCore->UI_MMenu.Render();
		SwapBuffers(hDC);
		break;
	case STATE_FIRSTPERSON:
	case STATE_GAME:
		if(uiCore->UI_Input.blockChanged()==TRUE)
		{
			//run active culling since active blocks have changed

		}
	


		//Reverse the camera matrix and pushes back to world coordinates - Matrix Stack: Camera move, reverse camera move
		uiCore->UI_Camera.ReverseCamMove();
		glPushMatrix();
		if(uiCore->UI_Camera.getMove()==TRUE)
		{
			//run occlusion culling
			chunkMan.VCull();

			//Run recalculation of viewport dimensions for picking
			uiCore->UI_Camera.CalcPicking();
			uiCore->UI_Camera.debugUpdate();
		}
		
#ifdef _DEBUG
		//Renders any debug elements (objects, not text!) from the OpenGLRenderer class
		pRenderer->debugDraw();
#endif

		//calls the Chunk manager's render function to draw active chunks onto the screen
		chunkMan.Render();
		rover->Render();
		//calls the Building Manager's render function to draw the building components
		gameCore->bMan.Render();
		if(uiCore->build)
		{
			uiCore->Render();
		}

		//readies the screen to draw the GUI elements
		pRenderer->ready2D();

		//render the GUI on screen
		uiCore->UI_GUI.Render();

		//readies the screen for the next pass of 3D rendering
		pRenderer->ready3D();

		//swap the buffers to display contents on screen
		SwapBuffers(hDC);


		//pops the camera matrix off the stack in prep for the next cycle - back to world coordinates
		glPopMatrix();
		glPopMatrix();
		glPopMatrix();

		//resets the UI_Camera routine flag
		uiCore->UI_Camera.hasMoved(false);
		break;
	case STATE_INVENTORY:

		break;
	case STATE_GAMEMENU:
		SwapBuffers(hDC);
		break;
	case STATE_DEBUG:
		if(uiCore->UI_Input.blockChanged()==TRUE)
		{
			//run active culling since active blocks have changed

		}
	
		if(uiCore->UI_Camera.getMove()==TRUE)
		{
			//run occlusion culling since camera has moved
		
		}

		//Reverse the camera matrix and pushes back to world coordinates - Matrix Stack: Camera move, reverse camera move
		uiCore->UI_Camera.ReverseCamMove();
		glPushMatrix();

		pRenderer->RenderTestBuilding();

		//calls the Chunk manager's render function to draw active chunks onto the screen
		chunkMan.Render();

		//readies the screen to draw the GUI elements
		pRenderer->ready2D();

		//render the GUI on screen
		uiCore->UI_GUI.Render();

		//swap the buffers to display contents on screen
		SwapBuffers(hDC);

		//readies the screen for the next pass of 3D rendering
		pRenderer->ready3D();

		//pops the camera matrix off the stack in prep for the next cycle - back to world coordinates
		glPopMatrix();
		glPopMatrix();
		glPopMatrix();

		//resets the UI_Camera routine flag
		uiCore->UI_Camera.hasMoved(false);
		break;
	}
	
}

/****************************
* Shutdown
****************************/
void GraphicsCore::shutdown()
{
	/*	Shutdown List:
		1. Call Chunk Manager's unload/shutdown function for all chunks active for player
		2. Shut down OpenGL
	*/

	chunkMan.UnloadAll();




	if(fullscreen)
	{
		ChangeDisplaySettings(NULL,0);
		ShowCursor(TRUE);
	}

	//clears the OpenGL Renderer pointer
	delete pRenderer;
}

/****************************
* startSingleGameplay
****************************/
void GraphicsCore::startSingleGameplay(bool newgame)
{
	//sets the world seed
	seed=-102894838;

	//Draws the loading screen so the player knows it is moving

	//Load the textures for the actual game now
	pRenderer->BatchBMPLoad();

	
	rover=new Bot(Vector3<real64>(0,6,-1),"Models/Testing/ArrowBot.dae",pRenderer->getTexture(4,MODEL_TEST),NULL,NULL);

	//sets up the land generator
	LGen.setup(0);

	if(newgame)
		//calls the chunk manager's load code to load the chunks around the player
		//This needs to be separated into the event that starts gameplay in final version.
		chunkMan.Load();	
}

void GraphicsCore::setPixelFormat()
{

	logger->LogMessage(LOG_OPENGL,"Setting pixel format for OGL\n");
	//set the pixel format
	memset(&pfd,0,sizeof(PIXELFORMATDESCRIPTOR));
	pfd.nSize=sizeof(PIXELFORMATDESCRIPTOR);
	pfd.nVersion=1;
	pfd.dwFlags=PFD_SUPPORT_OPENGL | PFD_DRAW_TO_WINDOW | PFD_DOUBLEBUFFER;
	pfd.iPixelType=PFD_TYPE_RGBA;
	pfd.cColorBits=32;
	pfd.cDepthBits=24;
	pfd.iLayerType=PFD_MAIN_PLANE;

	//sets the pixel format to the window
	int pixelFormat=ChoosePixelFormat(hDC,&pfd);
	SetPixelFormat(hDC,pixelFormat,&pfd);
}

/****************************
* setSeed
****************************/
void GraphicsCore::setSeed(s64 newseed)
{
	seed=newseed;
}

/****************************
* getSeed
****************************/
s64 GraphicsCore::getSeed()
{
	return seed;
}

/****************************
* resetBuffers
****************************/
void GraphicsCore::resetBuffers()
{	
	glClearColor(0.0f,0.0f,0.0f,0.0f);

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();
}

/****************************
* getCenterPos
****************************/
Vector3<s64> GraphicsCore::getCenterPos()
{
	return centerPos;
}

/****************************
* setCenterPos
****************************/
void GraphicsCore::setCenterPos(Vector3<s64> pos)
{
	centerPos=pos;
}