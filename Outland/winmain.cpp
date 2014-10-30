#include <Windows.h>
#include <WinUser.h>
#include <stdio.h>
#include <io.h>
#include <fcntl.h>
#include <string>
#include <gl\glew.h>
#include <iostream>

//Engine Core includes
#include "Singletons.h"

//Timer include
#include "Timer.h"

/***********************
Global Declarations - to be externed when needed
***********************/

//Window Variables
HINSTANCE hInst;
HWND wndHandle;

//OpenGL global variables for rendering context and device context and pixel format respectively
HGLRC	hRC;
HDC		hDC;
PIXELFORMATDESCRIPTOR pfd;

//resolution variables
int RES_WIDTH=1200;
int RES_HEIGHT=900;
long windowBits=32;
bool fullscreen=false;
int RENDER_DIST=6;
int CHUNK_SIZE=10;
int HEIGHT_CHUNK=3;
extern bool activeRender=true;
extern bool polyRender=false;
extern bool polyWireframe=false;
extern bool lowPolyRender=false;

//forward declarations
bool initWindow(HINSTANCE hInstance);
LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);


/***********************
********Winmain*********
***********************/
int WINAPI WinMain( HINSTANCE hInstance,
					HINSTANCE prevInstance,
					LPTSTR lpCmdLind,
					int nCmdShow)
{
#ifdef _DEBUG
	//Set up debug console
	AllocConsole();

	//Sets the redirect for std output
	int hConHandle;
	long lStdHandle;
	FILE *fp;

	lStdHandle=(long)GetStdHandle(STD_OUTPUT_HANDLE);
	hConHandle= _open_osfhandle(lStdHandle, _O_TEXT);
	fp=_fdopen(hConHandle,"w");
	*stdout=*fp;
	setvbuf(stdout,NULL,_IONBF,0);

	//ios::sync_with_stdio();

	std::cout << "Debugging Console:\n\n";
#endif

	//create new instances of the engine cores
	Timer time,time2(time);

	//Variables for separating the game loop for distributed timing
	real32 logicPerSec=1000/30.0f;
	real32 maxFrameRate=1000/120.0f;
	real32 counter=0;
	real32 counter2=0;
	s32 maxSkip=4;
	real32 timeThisStep=0;
	real32 startTime;
	
	logger->LogMessage(LOG_MAINCORE,"Starting Program\n");

	//initialize the window
	if(!initWindow(hInstance))
		return false;

	//sets the global hInst variable
	hInst=hInstance;

	bool initialized=false;

	//main message loop
	MSG msg;
	ZeroMemory(&msg,sizeof(msg));
	time.update();
	time2.update();



	while(msg.message!=WM_QUIT)
	{
		if(PeekMessage(&msg,NULL,0U,0U,PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		else
		{
			if(!initialized)
			{
				//start engine cores
				GUILoad->Startup();
				grCore->startup();
				uiCore->startup();
				gameCore->startup();
				initialized=true;

				uiCore->update(logicPerSec);
				gameCore->update(logicPerSec);
				grCore->update(timeThisStep);

			}

			//run Timer update function to obtain up to date information
			time.update();
			time2.update();
			startTime=time.getTime();
			timeThisStep=0;
			s32 loops=0;
			s32 logicloops=0;


			while(counter>=logicPerSec && logicloops<maxSkip)
			{
				uiCore->update(logicPerSec);
				//run core update methods of non-graphics (Order: UI, Player, AI, Audio)
				gameCore->update(logicPerSec);
				
				counter-=logicPerSec;
				timeThisStep+=logicPerSec;
				logicloops++;
				if(logicloops==maxSkip)	//ensures at LEAST 24 FPS
					counter=0;
			}

			if(uiCore->limitFPS==TRUE)
			{
				real32 total;
				while(counter2>=maxFrameRate && loops<maxSkip)
				{
					grCore->update(timeThisStep);
				

					counter2-=maxFrameRate;
					//calculate the total time it has taken for this frame
					
					time2.update();	
					total=counter2+time2.getTime()-startTime;

					if(total<maxFrameRate)
					{
						if(maxFrameRate-total>0)
						{
							//Sleep(maxFrameRate-total);		//Sleep seems to be bogging down UI controls, commented out for the moment
						}
					}				
					loops++;
					if(loops==maxSkip)	//ensures at LEAST 24 FPS
						counter2=0;
				}
				
				
			}
			else
			{
				//update the render method with the interpolation data
				grCore->update(timeThisStep);
				
				time2.update();
			}
			time2.update();
			counter+=time2.getTime()-startTime;
			counter2+=time2.getTime()-startTime;

		}

	}

	
	//Shutdown the engine cores
	uiCore->shutdown();
	gameCore->shutdown();
	grCore->shutdown();

	//Free the debug Console
	FreeConsole();

	return (int)msg.wParam;
}

/****************************
* WndProc
****************************/
LRESULT CALLBACK WndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	//variables needed for mouse movement
	POINT curPos;
	BOOL result;

	switch(message)
	{
	case WM_CREATE:
		//sets the global handle for the window
		wndHandle=hwnd;

		//Initialize OpenGL Window settings
		hDC=GetDC(wndHandle);		//creates the device context
		grCore->setPixelFormat();	//sets the pixel format
		hRC=wglCreateContext(hDC);	//creates the Render context
		wglMakeCurrent(hDC,hRC);

		break;
	case WM_LBUTTONDOWN:
		//send input to user input api
		uiCore->userInput(true, KEY_MOUSELEFT);
		break;
	case WM_LBUTTONUP:
		uiCore->userInput(false, KEY_MOUSELEFT);
		break;
	case WM_RBUTTONDOWN:
		//send input to the user input api
		uiCore->userInput(true, KEY_MOUSERIGHT);
		break;
	case WM_RBUTTONUP:
		uiCore->userInput(false, KEY_MOUSERIGHT);
		break;
	case WM_MOUSEMOVE:
		result = GetCursorPos(&curPos);
		
		//test to see if GetCursorPos suceeded
		if (result)
		{
			//Need to transform the screen coordinates into the window coordinates before transferring them to UserInput.
			ScreenToClient(wndHandle,&curPos);
			
			//transfer information to user input class
			uiCore->mouseInput(curPos.x,curPos.y);
		}
		break;

	case WM_KEYDOWN:
		
		//May move this all to a virtualization function to abstract input from the system API but that may be more complicated than its worth
		int fwKeys;
		LPARAM keyData;
		fwKeys=(int)wParam;
		keyData=lParam;

		switch(fwKeys)
		{
		case 0xC0:
			uiCore->userInput(true, KEY_TILDA);
			break;
		case 0x14:
			uiCore->userInput(true, KEY_CAPS);
			break;

			//Number keys 0-9
		case 0x30:
			uiCore->userInput(true, KEY_0);
			break;
		case 0x31:
			uiCore->userInput(true, KEY_1);
			break;
		case 0x32:
			uiCore->userInput(true, KEY_2);
			break;
		case 0x33:
			uiCore->userInput(true, KEY_3);
			break;
		case 0x34:
			uiCore->userInput(true, KEY_4);
			break;
		case 0x35:
			uiCore->userInput(true, KEY_5);
			break;
		case 0x36:
			uiCore->userInput(true, KEY_6);
			break;
		case 0x37:
			uiCore->userInput(true, KEY_7);
			break;
		case 0x38:
			uiCore->userInput(true, KEY_8);
			break;
		case 0x39:
			uiCore->userInput(true, KEY_9);
			break;

			//alphabetical keys
		case 0x41:
			uiCore->userInput(true, KEY_A);
			break;
		case 0x42:
			uiCore->userInput(true, KEY_B);
			break;
		case 0x43:
			uiCore->userInput(true, KEY_C);
			break;
		case 0x44:
			uiCore->userInput(true, KEY_D);
			break;
		case 0x45:
			uiCore->userInput(true, KEY_E);
			break;
		case 0x46:
			uiCore->userInput(true, KEY_F);
			break;
		case 0x47:
			uiCore->userInput(true, KEY_G);
			break;
		case 0x48:
			uiCore->userInput(true, KEY_H);
			break;
		case 0x49:
			uiCore->userInput(true, KEY_I);
			break;
		case 0x4A:
			uiCore->userInput(true, KEY_J);
			break;
		case 0x4B:
			uiCore->userInput(true, KEY_K);
			break;
		case 0x4C:
			uiCore->userInput(true, KEY_L);
			break;
		case 0x4D:
			uiCore->userInput(true, KEY_M);
			break;
		case 0x4E:
			uiCore->userInput(true, KEY_N);
			break;
		case 0x4F:
			uiCore->userInput(true, KEY_O);
			break;
		case 0x50:
			uiCore->userInput(true, KEY_P);
			break;
		case 0x51:
			uiCore->userInput(true, KEY_Q);
			break;
		case 0x52:
			uiCore->userInput(true, KEY_R);
			break;
		case 0x53:
			uiCore->userInput(true, KEY_S);
			break;
		case 0x54:
			uiCore->userInput(true, KEY_T);
			break;
		case 0x55:
			uiCore->userInput(true, KEY_U);
			break;
		case 0x56:
			uiCore->userInput(true, KEY_V);
			break;
		case 0x57:
			uiCore->userInput(true, KEY_W);
			break;
		case 0x58:
			uiCore->userInput(true, KEY_X);
			break;
		case 0x59:
			uiCore->userInput(true, KEY_Y);
			break;
		case 0x5A:
			uiCore->userInput(true, KEY_Z);
			break;


			//Virtual Keys
		case VK_ESCAPE:
			uiCore->userInput(true, KEY_ESC);
			break;
		case VK_SPACE:
			uiCore->userInput(true, KEY_SPACE);
			break;
		case VK_BACK:
			uiCore->userInput(true, KEY_BACKSPACE);
			break;
		case VK_TAB:
			uiCore->userInput(true, KEY_TAB);
			break;
		case VK_RETURN:
			uiCore->userInput(true, KEY_ENTER);
			break;
		case VK_SHIFT:
			uiCore->userInput(true, KEY_SHIFT);
			break;
		case VK_CONTROL:
			uiCore->userInput(true, KEY_CTRL);
			break;
		case VK_MENU:
			uiCore->userInput(true, KEY_ALT);
			break;

		//NEED CAPSLOCK, and punctuation keys

			//Number pad codes
		case VK_NUMPAD0:
			uiCore->userInput(true, KEY_NUM0);
			break;
		case VK_NUMPAD1:
			uiCore->userInput(true, KEY_NUM1);
			break;
		case VK_NUMPAD2:
			uiCore->userInput(true, KEY_NUM2);
			break;
		case VK_NUMPAD3:
			uiCore->userInput(true, KEY_NUM3);
			break;
		case VK_NUMPAD4:
			uiCore->userInput(true, KEY_NUM4);
			break;
		case VK_NUMPAD5:
			uiCore->userInput(true, KEY_NUM5);
			break;
		case VK_NUMPAD6:
			uiCore->userInput(true, KEY_NUM6);
			break;
		case VK_NUMPAD7:
			uiCore->userInput(true, KEY_NUM7);
			break;
		case VK_NUMPAD8:
			uiCore->userInput(true, KEY_NUM8);
			break;
		case VK_NUMPAD9:
			uiCore->userInput(true, KEY_NUM9);
			break;
		case VK_MULTIPLY:
			uiCore->userInput(true, KEY_NUMMULTIPLY);
			break;
		case VK_ADD:
			uiCore->userInput(true, KEY_NUMPLUS);
			break;
		case VK_DIVIDE:
			uiCore->userInput(true, KEY_NUMSLASH);
			break;
		case VK_SUBTRACT:
			uiCore->userInput(true, KEY_NUMMINUS);
			break;
		case VK_DECIMAL:
			uiCore->userInput(true, KEY_NUMPERIOD);
			break;
		case VK_NUMLOCK:
			uiCore->userInput(true, KEY_NUMLOCK);
			break;

			//Function Keys
		case VK_F1:
			uiCore->userInput(true, KEY_F1);
			break;
		case VK_F2:
			uiCore->userInput(true, KEY_F2);
			break;
		case VK_F3:
			uiCore->userInput(true, KEY_F3);
			/*Temporary keys for debug viewing of render methods*/
			activeRender=!activeRender;
			break;
		case VK_F4:
			uiCore->userInput(true, KEY_F4);
			polyRender=!polyRender;
			break;
		case VK_F5:
			uiCore->userInput(true, KEY_F5);
			polyWireframe=!polyWireframe;
			break;
		case VK_F6:
			uiCore->userInput(true, KEY_F6);
			lowPolyRender=!lowPolyRender;
			break;
		case VK_F7:
			uiCore->userInput(true, KEY_F7);
			break;
		case VK_F8:
			uiCore->userInput(true, KEY_F8);
			break;
		case VK_F9:
			uiCore->userInput(true, KEY_F9);
			break;
		case VK_F10:
			uiCore->userInput(true, KEY_F10);
			break;
		case VK_F11:
			uiCore->userInput(true, KEY_F11);
			break;
		case VK_F12:
			uiCore->userInput(true, KEY_F12);
			break;

			//Virtual Arrow Key Codes
		case VK_UP:
			uiCore->userInput(true, KEY_UPARROW);
			break;
		case VK_DOWN:
			uiCore->userInput(true, KEY_DOWNARROW);
			break;
		case VK_LEFT:
			uiCore->userInput(true, KEY_LEFTARROW);
			break;
		case VK_RIGHT:
			uiCore->userInput(true, KEY_RIGHTARROW);
			break;
		default:
			break;
		}
		break;

	case WM_KEYUP:
				//May move this all to a virtualization function to abstract input from the system API but that may be more complicated than its worth
		fwKeys=(int)wParam;
		keyData=lParam;

		switch(fwKeys)
		{
		case 0xC0:
			uiCore->userInput(false, KEY_TILDA);
			break;
		case 0x14:
			uiCore->userInput(false, KEY_CAPS);
			break;

			//Number keys 0-9
		case 0x30:
			uiCore->userInput(false, KEY_0);
			break;
		case 0x31:
			uiCore->userInput(false, KEY_1);
			break;
		case 0x32:
			uiCore->userInput(false, KEY_2);
			break;
		case 0x33:
			uiCore->userInput(false, KEY_3);
			break;
		case 0x34:
			uiCore->userInput(false, KEY_4);
			break;
		case 0x35:
			uiCore->userInput(false, KEY_5);
			break;
		case 0x36:
			uiCore->userInput(false, KEY_6);
			break;
		case 0x37:
			uiCore->userInput(false, KEY_7);
			break;
		case 0x38:
			uiCore->userInput(false, KEY_8);
			break;
		case 0x39:
			uiCore->userInput(false, KEY_9);
			break;

			//alphabetical keys
		case 0x41:
			uiCore->userInput(false, KEY_A);
			break;
		case 0x42:
			uiCore->userInput(false, KEY_B);
			break;
		case 0x43:
			uiCore->userInput(false, KEY_C);
			break;
		case 0x44:
			uiCore->userInput(false, KEY_D);
			break;
		case 0x45:
			uiCore->userInput(false, KEY_E);
			break;
		case 0x46:
			uiCore->userInput(false, KEY_F);
			break;
		case 0x47:
			uiCore->userInput(false, KEY_G);
			break;
		case 0x48:
			uiCore->userInput(false, KEY_H);
			break;
		case 0x49:
			uiCore->userInput(false, KEY_I);
			break;
		case 0x4A:
			uiCore->userInput(false, KEY_J);
			break;
		case 0x4B:
			uiCore->userInput(false, KEY_K);
			break;
		case 0x4C:
			uiCore->userInput(false, KEY_L);
			break;
		case 0x4D:
			uiCore->userInput(false, KEY_M);
			break;
		case 0x4E:
			uiCore->userInput(false, KEY_N);
			break;
		case 0x4F:
			uiCore->userInput(false, KEY_O);
			break;
		case 0x50:
			uiCore->userInput(false, KEY_P);
			break;
		case 0x51:
			uiCore->userInput(false, KEY_Q);
			break;
		case 0x52:
			uiCore->userInput(false, KEY_R);
			break;
		case 0x53:
			uiCore->userInput(false, KEY_S);
			break;
		case 0x54:
			uiCore->userInput(false, KEY_T);
			break;
		case 0x55:
			uiCore->userInput(false, KEY_U);
			break;
		case 0x56:
			uiCore->userInput(false, KEY_V);
			break;
		case 0x57:
			uiCore->userInput(false, KEY_W);
			break;
		case 0x58:
			uiCore->userInput(false, KEY_X);
			break;
		case 0x59:
			uiCore->userInput(false, KEY_Y);
			break;
		case 0x5A:
			uiCore->userInput(false, KEY_Z);
			break;


			//Virtual Keys
		case VK_ESCAPE:
			uiCore->userInput(false, KEY_ESC);
			break;
		case VK_SPACE:
			uiCore->userInput(false, KEY_SPACE);
			break;
		case VK_BACK:
			uiCore->userInput(false, KEY_BACKSPACE);
			break;
		case VK_TAB:
			uiCore->userInput(false, KEY_TAB);
			break;
		case VK_RETURN:
			uiCore->userInput(false, KEY_ENTER);
			break;
		case VK_SHIFT:
			uiCore->userInput(false, KEY_SHIFT);
			break;
		case VK_CONTROL:
			uiCore->userInput(false, KEY_CTRL);
			break;
		case VK_MENU:
			uiCore->userInput(false, KEY_ALT);
			break;

		//NEED CAPSLOCK, and punctuation keys

			//Number pad codes
		case VK_NUMPAD0:
			uiCore->userInput(false, KEY_NUM0);
			break;
		case VK_NUMPAD1:
			uiCore->userInput(false, KEY_NUM1);
			break;
		case VK_NUMPAD2:
			uiCore->userInput(false, KEY_NUM2);
			break;
		case VK_NUMPAD3:
			uiCore->userInput(false, KEY_NUM3);
			break;
		case VK_NUMPAD4:
			uiCore->userInput(false, KEY_NUM4);
			break;
		case VK_NUMPAD5:
			uiCore->userInput(false, KEY_NUM5);
			break;
		case VK_NUMPAD6:
			uiCore->userInput(false, KEY_NUM6);
			break;
		case VK_NUMPAD7:
			uiCore->userInput(false, KEY_NUM7);
			break;
		case VK_NUMPAD8:
			uiCore->userInput(false, KEY_NUM8);
			break;
		case VK_NUMPAD9:
			uiCore->userInput(false, KEY_NUM9);
			break;
		case VK_MULTIPLY:
			uiCore->userInput(false, KEY_NUMMULTIPLY);
			break;
		case VK_ADD:
			uiCore->userInput(false, KEY_NUMPLUS);
			break;
		case VK_DIVIDE:
			uiCore->userInput(false, KEY_NUMSLASH);
			break;
		case VK_SUBTRACT:
			uiCore->userInput(false, KEY_NUMMINUS);
			break;
		case VK_DECIMAL:
			uiCore->userInput(false, KEY_NUMPERIOD);
			break;
		case VK_NUMLOCK:
			uiCore->userInput(false, KEY_NUMLOCK);
			break;

			//Function Keys
		case VK_F1:
			uiCore->userInput(false, KEY_F1);
			break;
		case VK_F2:
			uiCore->userInput(false, KEY_F2);
			break;
		case VK_F3:
			uiCore->userInput(false, KEY_F3);
			break;
		case VK_F4:
			uiCore->userInput(false, KEY_F4);
			break;
		case VK_F5:
			uiCore->userInput(false, KEY_F5);
			break;
		case VK_F6:
			uiCore->userInput(false, KEY_F6);
			break;
		case VK_F7:
			uiCore->userInput(false, KEY_F7);
			break;
		case VK_F8:
			uiCore->userInput(false, KEY_F8);
			break;
		case VK_F9:
			uiCore->userInput(false, KEY_F9);
			break;
		case VK_F10:
			uiCore->userInput(false, KEY_F10);
			break;
		case VK_F11:
			uiCore->userInput(false, KEY_F11);
			break;
		case VK_F12:
			uiCore->userInput(false, KEY_F12);
			break;

			//Virtual Arrow Key Codes
		case VK_UP:
			uiCore->userInput(false, KEY_UPARROW);
			break;
		case VK_DOWN:
			uiCore->userInput(false, KEY_DOWNARROW);
			break;
		case VK_LEFT:
			uiCore->userInput(false, KEY_LEFTARROW);
			break;
		case VK_RIGHT:
			uiCore->userInput(false, KEY_RIGHTARROW);
			break;
		default:
			break;
		}
		break;
	case WM_CLOSE:
	case WM_DESTROY:
	case WM_QUIT:
		wglMakeCurrent(hDC,NULL);
		wglDeleteContext(hRC);
		PostQuitMessage(0);
		break;
	}//end switch statement


	return DefWindowProc(hwnd, message, wParam, lParam);
}

/******************************
* initWindow
******************************/
bool initWindow(HINSTANCE hInstance)
{	
	logger->LogMessage(LOG_MAINCORE,"Creating Window for program\n");
	WNDCLASSEX wcex;
	DWORD dwExStyle;
	DWORD dwStyle;
	RECT windowRect;

	windowRect.left=(long)0;
	windowRect.right=(long)RES_WIDTH;
	windowRect.top=(long)0;
	windowRect.bottom=(long)RES_HEIGHT;

	wcex.cbSize			= sizeof(WNDCLASSEX);
	wcex.style			= CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc	= WndProc;
	wcex.cbClsExtra		= 0;
	wcex.cbWndExtra		= 0;
	wcex.hInstance		= hInstance;
	wcex.hIcon			= 0;
	wcex.hCursor		= LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground	= (HBRUSH)(COLOR_WINDOW+1);
	wcex.lpszMenuName	= NULL;
	wcex.lpszClassName	= "Outland";
	wcex.hIconSm		= 0;

	if(!RegisterClassEx(&wcex))
		return false;
	
	//checks for fullscreen mode and changes display
	if(fullscreen)
	{
		DEVMODE dmScreenSettings;	//device mode

		memset(&dmScreenSettings,0,sizeof(dmScreenSettings));
		dmScreenSettings.dmSize=sizeof(dmScreenSettings);
		dmScreenSettings.dmPelsHeight=RES_HEIGHT;
		dmScreenSettings.dmPelsWidth=RES_WIDTH;
		dmScreenSettings.dmBitsPerPel=windowBits;
		dmScreenSettings.dmFields=DM_BITSPERPEL|DM_PELSWIDTH|DM_PELSHEIGHT;

		//attempts to change the display setting
		if(ChangeDisplaySettings(&dmScreenSettings,CDS_FULLSCREEN)!=DISP_CHANGE_SUCCESSFUL)
		{
			//setting new display mode failed, revert to windowed mode
			MessageBox(NULL,"Display mode change failed, reverting to windowed mode",NULL,MB_OK);
			fullscreen=false;
		}
	}

	//checks to see if fullscreen mode is successful
	if(fullscreen)
	{
		dwExStyle=WS_EX_APPWINDOW;
		dwStyle=WS_POPUP;
		ShowCursor(FALSE);
	}
	else
	{
		dwExStyle=WS_EX_APPWINDOW|WS_EX_WINDOWEDGE;
		dwStyle=WS_OVERLAPPEDWINDOW;
	}

	//Adjusts the window to requested size
	AdjustWindowRectEx(&windowRect,dwStyle,FALSE,dwExStyle);

	//creates the window
	wndHandle = CreateWindowEx(	NULL,
								"Outland",
								"Outland",
								dwStyle|WS_CLIPCHILDREN|WS_CLIPSIBLINGS,
								0,
								0,
								RES_WIDTH,
								RES_HEIGHT,
								NULL,
								NULL,
								hInstance,
								NULL);

	//ensures valid window handle
	if(!wndHandle)
		return false;



	//displays window on screen
	ShowWindow(wndHandle,SW_SHOW);
	UpdateWindow(wndHandle);
	logger->LogMessage(LOG_MAINCORE,"Window has been created successfully\n");
	return true;
}