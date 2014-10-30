#ifndef UICore_H
#define UICore_H

#include "CameraControl.h"
#include "UserInput.h"
#include "MainMenu.h"
#include "LoadingMenu.h"
#include "GUI.h"
#include <string>
#include "Tutorial.h"


class UserInputCore
{
public:
	static UserInputCore* Instance();

	//Trifecta core
	void startup();
	void update(real32 time);
	void shutdown();

	//public functions to call functions within the private classes
	void			rotateCam(float roll, float yaw, float pitch);
	void			moveCam(float x, float y, float z);
	void			userInput(bool value,int key);
	void			mouseInput(int x, int y);
	void			setState(s32);
	void			renderText(s32 xPos,s32 yPos, const char*string);
	void			toggleView();
	void			setBuildMark(Vector3<s64> pos);
	void			moveBuildMark(Vector3<s64> pos);
	void			enterPos();
	void			Render();
	void			setBuild1(Vector3<s64> pos);
	void			setBuild2(Vector3<s64> pos);

	s32				getState();
	u32				setupText(char* fontName,s32 fontSize);
	std::string		intToString(const s64& num);
	std::string		floatToString(const real64& data,const s32& precision);
	bool			getView();
	Vector3<s64>	getBuildMark();
	Vector3<s64>	getBuildMark1();
	Vector3<s64>	getBuildMark2();
	tBOOL			getBuildMarkState(s32 marker);
	void			clearMarks();
	
	//classes of UI
	UserInput		UI_Input;
	CameraControl	UI_Camera;
	Menu			UI_MMenu;
	GUI				UI_GUI;
	Tutorial		UI_Tutorial;
	LoadingMenu		UI_LoadMenu;

	//temp options
	tBOOL			limitFPS;
	u32				textBase;
	s32				build;
	s32				buildType;

private:
	s32				gameState;
	bool			firstPerson;
	Vector3<s64>	buildMark;
	Vector3<s64>	*build1;
	Vector3<s64>	*build2;

	UserInputCore();
	~UserInputCore();
	UserInputCore(const UserInputCore&);
	UserInputCore* operator=(const UserInputCore&);
};



#endif