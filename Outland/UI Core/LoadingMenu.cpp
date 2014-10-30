#include "LoadingMenu.h"
#include <Outland\Singletons.h>
#include <iostream>

extern int RES_WIDTH;
extern int RES_HEIGHT;

/****************************
* Constructor
****************************/
LoadingMenu::LoadingMenu()
{
}

/****************************
* Destructor
****************************/
LoadingMenu::~LoadingMenu()
{
	clearSaves();
}

void LoadingMenu::Startup()
{
	updateSaves();
}

void LoadingMenu::updateSaves()
{
	std::string directory;
	WIN32_FIND_DATA fileData;
	real32 xP(real32(RES_WIDTH/2.0f+(6*RES_HEIGHT/20))),
	xM(real32(RES_WIDTH/2.0f-(6*RES_HEIGHT/20))),
	yP(real32(14*(RES_HEIGHT-38)/20)),
	xT(real32(xP-xM)),
	yT(real32(RES_HEIGHT-38)/20),
	entries(0);

	//clears the current save list
	clearSaves();

	HANDLE hr=FindFirstFile("D:/Outland SVN/Outland/saves/*",&fileData);
	FindNextFile(hr,&fileData);

	if(hr!=INVALID_HANDLE_VALUE)
	{
		while(FindNextFile(hr,&fileData)!=0)
		{
			if(fileData.dwFileAttributes==FILE_ATTRIBUTE_DIRECTORY)
			{
				directory=fileData.cFileName;
				std::cout << "World data found! World name: " << directory << endl;
				Button newButton(directory,xT,yT,xM,yP-yT*entries,0);
				entries++;
				saves.push_back(newButton);
			}
		}
	}
	else
	{
		cout << "No file data found in saves folder!\n";
	}
}
